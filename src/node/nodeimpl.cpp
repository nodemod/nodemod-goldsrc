#include <cstring>
#include <unordered_map>
#include <map>
#include <vector>
#include "nodeimpl.hpp"
#include "resource.hpp"
#include "events.hpp"
#include "structures/structures.hpp"
#include <hlsdk/engine/custom.h>

#include <sstream>

// Global callback storage for custom engine functions
std::unordered_map<std::string, v8::Global<v8::Function>> serverCommandCallbacks;
std::unordered_map<std::string, v8::Global<v8::Context>> serverCommandContexts;
std::unordered_map<std::string, v8::Global<v8::Function>> deltaEncoderCallbacks;
std::unordered_map<std::string, v8::Global<v8::Context>> deltaEncoderContexts;

// Storage for player customizations (persists across reload for catch-up events)
std::map<int, std::vector<customization_t>> playerCustomizations;

// Called from postDllPlayerCustomization to store customization data
void storePlayerCustomization(edict_t* player, customization_t* custom) {
	if (!player || !custom) return;
	int slot = (*g_engfuncs.pfnIndexOfEdict)(player);
	if (slot < 1) return;

	// Make a copy - clear pointers that won't be valid after reload
	customization_t copy = *custom;
	copy.pNext = nullptr;
	copy.pInfo = nullptr;
	copy.pBuffer = nullptr;
	playerCustomizations[slot].push_back(copy);
}

// Called from postDllClientDisconnect to clear stored customizations
void clearPlayerCustomizations(edict_t* player) {
	if (!player) return;
	int slot = (*g_engfuncs.pfnIndexOfEdict)(player);
	if (slot < 1) return;
	playerCustomizations.erase(slot);
}

bool isRun = false;

void continueHandler(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Locker locker(info.GetIsolate());
	v8::HandleScope scope(info.GetIsolate());
	auto context = info.GetIsolate()->GetCurrentContext();

	isRun = true;
}

void OnMessage(v8::Local<v8::Message> message, v8::Local<v8::Value> error)
{
	auto isolate = nodeImpl.GetIsolate();
	v8::Locker locker(isolate);
	v8::Isolate::Scope isolateScope(isolate);
	v8::HandleScope handleScope(isolate);
	v8::String::Utf8Value messageStr(isolate, message->Get());
	v8::String::Utf8Value errorStr(isolate, error);

	std::stringstream stack;
	auto stackTrace = message->GetStackTrace();

	for (int i = 0; i < stackTrace->GetFrameCount(); i++)
	{
		auto frame = stackTrace->GetFrame(isolate, i);

		v8::String::Utf8Value sourceStr(isolate, frame->GetScriptNameOrSourceURL());
		v8::String::Utf8Value functionStr(isolate, frame->GetFunctionName());

		stack << *sourceStr << "(" << frame->GetLineNumber() << "," << frame->GetColumn() << "): " << (*functionStr ? *functionStr : "") << "\n";
	}

	printf("%s\n%s\n%s\n", *messageStr, stack.str().c_str(), *errorStr);
}

NodeImpl nodeImpl;
Resource *resource;

NodeImpl::NodeImpl()
{
}

NodeImpl::~NodeImpl()
{
}

void NodeImpl::Tick()
{
	v8::Locker locker(v8Isolate);
	v8::Isolate::Scope isolateScope(v8Isolate);
	v8::HandleScope hs(v8Isolate);

	{
		v8::Local<v8::Context> _context = resource->GetContext().Get(v8Isolate);
		v8::Context::Scope contextScope(_context);

		uv_run(nodeLoop->GetLoop(), UV_RUN_NOWAIT);
		v8Isolate->PerformMicrotaskCheckpoint();
	}
}

void NodeImpl::Initialize()
{
	// Initialize Node.js process following HL1 metamod approach - prevent Node.js from initializing V8
	std::vector<std::string> args{"node"};
	auto result = node::InitializeOncePerProcess(
		args,
		static_cast<node::ProcessInitializationFlags::Flags>(
			node::ProcessInitializationFlags::kNoStdioInitialization |
			node::ProcessInitializationFlags::kNoInitializeV8 |
			node::ProcessInitializationFlags::kNoInitializeNodeV8Platform
		)
	);

	// Handle initialization errors
	for (const std::string& error : result->errors()) {
		printf("Node.js init error: %s\n", error.c_str());
	}
	if (result->early_return() != 0) {
		printf("Node.js early return with code: %d\n", result->exit_code());
		return;
	}

	// Create our own V8 platform and initialize V8 manually
	v8Platform = node::MultiIsolatePlatform::Create(4);
	v8::V8::InitializePlatform(v8Platform.get());
	v8::V8::Initialize();

	arrayBufferAllocator = node::ArrayBufferAllocator::Create();

	nodeLoop = std::make_unique<UvLoop>("mainNode");

	// Create IsolateSettings like HL1 metamod does
	node::IsolateSettings isolate_settings;
	v8Isolate = node::NewIsolate(arrayBufferAllocator.get(), nodeLoop->GetLoop(), v8Platform.get(), nullptr, isolate_settings);
	v8Isolate->SetFatalErrorHandler([](const char *location, const char *message)
																	{
				printf("V8 FATAL [%s]: %s\n", location, message);
				exit(0); });

	v8Isolate->SetCaptureStackTraceForUncaughtExceptions(true);
	v8Isolate->AddMessageListener(OnMessage);

	v8::Locker locker(v8Isolate);
	v8::Isolate::Scope isolateScope(v8Isolate);

	auto isolateData = node::CreateIsolateData(v8Isolate, nodeLoop->GetLoop(), v8Platform.get(), arrayBufferAllocator.get());

	nodeData.reset(isolateData);
}

bool NodeImpl::loadScript()
{
	resource = new Resource("main", ".");
	resource->Init();

	return true;
}

void NodeImpl::Stop()
{
	// Shutdown Ham hooks while game DLL is still loaded
	// This must happen before static destructors run
	{
		extern void shutdownHamManager();
		shutdownHamManager();
	}

	resource->Stop();
	node::FreeIsolateData(nodeData.get());
	v8::V8::Dispose();
	// v8::V8::ShutdownPlatform() - removed in V8 v24
}

bool NodeImpl::reload()
{
	if (resource) {
		{
			v8::Locker locker(v8Isolate);
			v8::Isolate::Scope isolateScope(v8Isolate);
			resource->Stop();
		}
		delete resource;
		resource = nullptr;
		
		// Clear all event listeners to prevent stale context references
		extern eventsContainer events;
		for (auto& pair : events) {
			if (pair.second) {
				pair.second->remove_all();
			}
		}
		events.clear();
		
		// Clear Ham hooks to restore vtables and free trampolines
		{
			extern void shutdownHamManager();
			shutdownHamManager();
		}

		// Clear server command callbacks to prevent crashes after reload
		// Note: These extern declarations match the static variables in customs.js
		{
			extern std::unordered_map<std::string, v8::Global<v8::Function>> serverCommandCallbacks;
			extern std::unordered_map<std::string, v8::Global<v8::Context>> serverCommandContexts;
			extern std::unordered_map<std::string, v8::Global<v8::Function>> deltaEncoderCallbacks;
			extern std::unordered_map<std::string, v8::Global<v8::Context>> deltaEncoderContexts;
			
			// Clear all callback references
			for (auto& pair : serverCommandCallbacks) {
				pair.second.Reset();
			}
			for (auto& pair : serverCommandContexts) {
				pair.second.Reset();
			}
			for (auto& pair : deltaEncoderCallbacks) {
				pair.second.Reset();
			}
			for (auto& pair : deltaEncoderContexts) {
				pair.second.Reset();
			}
			
			serverCommandCallbacks.clear();
			serverCommandContexts.clear();
			deltaEncoderCallbacks.clear();
			deltaEncoderContexts.clear();
		}
	}

	bool result = loadScript();

	// Fire catch-up events if reload was successful and server is active
	// This ensures plugins don't miss dllServerActivate and connected players
	if (result && gpGlobals && gpGlobals->maxClients > 0) {
		v8::Locker locker(v8Isolate);
		v8::Isolate::Scope isolateScope(v8Isolate);
		v8::HandleScope handleScope(v8Isolate);

		// Fire dllServerActivate to let plugins know server is ready
		// Note: We skip precache events (dllSpawn for worldspawn) since that would crash
		// Use worldspawn (entity 0) as the edict list base
		edict_t* worldspawn = (*g_engfuncs.pfnPEntityOfEntIndex)(0);
		int edictCount = gpGlobals->maxEntities;
		int clientMax = gpGlobals->maxClients;

		event::findAndCall("dllServerActivate", [=](v8::Isolate* isolate) {
			v8::Local<v8::Value>* args = new v8::Local<v8::Value>[3];
			args[0] = structures::wrapEntity(isolate, worldspawn);
			args[1] = v8::Number::New(isolate, edictCount);
			args[2] = v8::Number::New(isolate, clientMax);
			return std::pair<unsigned int, v8::Local<v8::Value>*>(3, args);
		});

		// Fire dllClientConnect + dllClientPutInServer for each connected player
		// Order matters: Connect fires before UserInfoChanged fires before PutInServer
		int playerCount = 0;
		int spectatorCount = 0;
		for (int i = 1; i <= clientMax; i++) {
			edict_t* player = (*g_engfuncs.pfnPEntityOfEntIndex)(i);
			if (player && (*g_engfuncs.pfnGetPlayerUserId)(player) > 0) {
				// Check if this is a spectator (HLTV proxy or spectator client)
				int flags = player->v.flags;
				bool isSpectator = (flags & FL_PROXY) || (flags & FL_SPECTATOR);

				if (isSpectator) {
					spectatorCount++;
					// Fire dllSpectatorConnect for spectators
					event::findAndCall("dllSpectatorConnect", [=](v8::Isolate* isolate) {
						v8::Local<v8::Value>* args = new v8::Local<v8::Value>[1];
						args[0] = structures::wrapEntity(isolate, player);
						return std::pair<unsigned int, v8::Local<v8::Value>*>(1, args);
					});
				} else {
					playerCount++;

					// Get player info for dllClientConnect
					const char* playerName = (*g_engfuncs.pfnInfoKeyValue)(
						(*g_engfuncs.pfnGetInfoKeyBuffer)(player), "name");
					// Note: We can't get the original IP after connection, use placeholder
					const char* playerAddress = "reload";

					// Fire dllClientConnect (already connected, so this is informational)
					event::findAndCall("dllClientConnect", [=](v8::Isolate* isolate) {
						v8::Local<v8::Value>* args = new v8::Local<v8::Value>[4];
						args[0] = structures::wrapEntity(isolate, player);
						args[1] = v8::String::NewFromUtf8(isolate, playerName ? playerName : "").ToLocalChecked();
						args[2] = v8::String::NewFromUtf8(isolate, playerAddress).ToLocalChecked();
						args[3] = v8::String::NewFromUtf8(isolate, "").ToLocalChecked(); // rejectReason
						return std::pair<unsigned int, v8::Local<v8::Value>*>(4, args);
					});

					// Fire dllClientUserInfoChanged (player info like name, model, etc.)
					char* infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)(player);
					event::findAndCall("dllClientUserInfoChanged", [=](v8::Isolate* isolate) {
						v8::Local<v8::Value>* args = new v8::Local<v8::Value>[2];
						args[0] = structures::wrapEntity(isolate, player);
						args[1] = v8::String::NewFromUtf8(isolate, infobuffer ? infobuffer : "").ToLocalChecked();
						return std::pair<unsigned int, v8::Local<v8::Value>*>(2, args);
					});

					// Fire dllClientPutInServer
					event::findAndCall("dllClientPutInServer", [=](v8::Isolate* isolate) {
						v8::Local<v8::Value>* args = new v8::Local<v8::Value>[1];
						args[0] = structures::wrapEntity(isolate, player);
						return std::pair<unsigned int, v8::Local<v8::Value>*>(1, args);
					});
				}
			}
		}

		// Fire dllPlayerCustomization for stored customizations
		int customizationCount = 0;
		for (const auto& pair : playerCustomizations) {
			int slot = pair.first;
			edict_t* player = (*g_engfuncs.pfnPEntityOfEntIndex)(slot);
			if (!player || (*g_engfuncs.pfnGetPlayerUserId)(player) <= 0) continue;

			for (const customization_t& custom : pair.second) {
				customizationCount++;
				// Create a non-const copy for the event
				customization_t customCopy = custom;
				event::findAndCall("dllPlayerCustomization", [=](v8::Isolate* isolate) mutable {
					v8::Local<v8::Value>* args = new v8::Local<v8::Value>[2];
					args[0] = structures::wrapEntity(isolate, player);
					args[1] = structures::wrapCustomization(isolate, &customCopy);
					return std::pair<unsigned int, v8::Local<v8::Value>*>(2, args);
				});
			}
		}

		char msg[128];
		snprintf(msg, sizeof(msg), "NodeMod: Fired catch-up events (dllServerActivate + %d players + %d spectators + %d customizations)\n",
			playerCount, spectatorCount, customizationCount);
		g_engfuncs.pfnServerPrint(msg);
	}

	return result;
}