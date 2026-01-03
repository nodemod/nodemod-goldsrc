#include <cstring>
#include <unordered_map>
#include "nodeimpl.hpp"
#include "resource.hpp"
#include "events.hpp"

#include <sstream>

// Global callback storage for custom engine functions
std::unordered_map<std::string, v8::Global<v8::Function>> serverCommandCallbacks;
std::unordered_map<std::string, v8::Global<v8::Context>> serverCommandContexts;
std::unordered_map<std::string, v8::Global<v8::Function>> deltaEncoderCallbacks;
std::unordered_map<std::string, v8::Global<v8::Context>> deltaEncoderContexts;

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
	
	return loadScript();
}