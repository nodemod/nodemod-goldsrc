#include "resource.hpp"
#include "nodeimpl.hpp"
#include "common/logger.hpp"
#include "bindings/bindings.hpp"
#include "util/convert.hpp"
#include <filesystem>
#include <dlfcn.h>

extern void registerDllEvents();
extern void registerEngineEvents();

static std::string getDllPath() {
	Dl_info dl_info;
	if (dladdr((void*)getDllPath, &dl_info) != 0) {
		std::filesystem::path dll_path = dl_info.dli_fname;
		return dll_path.parent_path().string();
	}
	return ".";
}

v8::Isolate* GetV8Isolate()
{
	return nodeImpl.GetIsolate();
}

static v8::Platform* GetV8Platform()
{
	return nodeImpl.GetPlatform();
}

static node::IsolateData* GetNodeIsolate()
{
	return nodeImpl.GetNodeIsolate();
}


	Resource::Resource(const std::string& name, const std::string& path) : name(name), path(path), nodeEnvironment(nullptr, node::FreeEnvironment)
	{}

	Resource::Resource() : nodeEnvironment(nullptr, node::FreeEnvironment)
	{}

	Resource::~Resource()
	{
		if (nodeEnvironment.get()) {
			Stop();
		}
	}

	void Resource::Init()
	{
		std::string entryFile;
		std::vector<std::string> node_flags;
		//Props_t& mainConfig = nodeImpl.GetMainConfig();

		bool useInspector;

			entryFile = path;
			useInspector = true;
		

		std::vector<std::string> args;
		args.emplace_back("node");

		for (auto& flag : node_flags)
		{
			args.emplace_back(flag.c_str());
		}

		args.emplace_back(entryFile.c_str());

		std::vector<std::string> exec_args;

		v8::Locker locker(GetV8Isolate());
		v8::HandleScope handleScope(GetV8Isolate());

		v8::Isolate::Scope isolateScope(GetV8Isolate());


		v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(GetV8Isolate());
		bindings::init(GetV8Isolate(), global);
		registerDllEvents();
		registerEngineEvents();
		//sampnode::callback::add_event_definitions(GetV8Isolate(), global);

		v8::Local<v8::Context> _context = node::NewContext(GetV8Isolate(), global);
		context.Reset(GetV8Isolate(), _context);
		v8::Context::Scope scope(_context);

		node::EnvironmentFlags::Flags flags = node::EnvironmentFlags::kOwnsProcessState;

		if (useInspector) {
			flags = static_cast<node::EnvironmentFlags::Flags>(flags | node::EnvironmentFlags::kOwnsInspector);
		} else {
			flags = static_cast<node::EnvironmentFlags::Flags>(flags | node::EnvironmentFlags::kNoCreateInspector);
		}

		// Get the actual game directory from engine and change to it BEFORE creating environment
		auto old_cwd = std::filesystem::current_path();
		
		// Get the DLL directory and construct plugins path relative to it
		std::string dll_path = getDllPath();
		std::filesystem::path plugins_path = std::filesystem::path(dll_path) / ".." / "plugins";
		std::string nodemod_path = plugins_path.string();
		
		// Check if the plugins path exists, fallback to current directory if not
		if (!std::filesystem::exists(nodemod_path)) {
			nodemod_path = ".";
		}
		
		// Change directory BEFORE creating the Node.js environment so module resolution works correctly
		std::filesystem::current_path(nodemod_path);
		
		// Create environment with correct working directory for ES module resolution
		auto env = node::CreateEnvironment(GetNodeIsolate(), _context, args, exec_args, flags);

		// Load module and return it via StartExecutionCallback
		std::string loaderScript = R"(
			const { createRequire } = require('module');
			const path = require('path');
			const customRequire = createRequire(path.join(process.cwd(), 'package.json'));
			const resolved = customRequire.resolve(')" + entryFile + R"(');
			return customRequire(resolved);
		)";

		v8::MaybeLocal<v8::Value> loadResult = node::LoadEnvironment(env, [&](const node::StartExecutionCallbackInfo& info) -> v8::MaybeLocal<v8::Value> {
			v8::Local<v8::Value> script_arg = v8::String::NewFromUtf8(GetV8Isolate(), loaderScript.c_str()).ToLocalChecked();
			return info.run_cjs->Call(_context, v8::Null(GetV8Isolate()), 1, &script_arg);
		});

		nodeEnvironment.reset(env);

		// Run the UV loop to allow modules to load
		uv_loop_t* loop = nodeImpl.GetUVLoop()->GetLoop();
		uv_run(loop, UV_RUN_NOWAIT);
		GetV8Isolate()->PerformMicrotaskCheckpoint();

		// Check if we got a module back and look for default export
		if (!loadResult.IsEmpty()) {
			v8::Local<v8::Value> mod = loadResult.ToLocalChecked();

			if (mod->IsObject()) {
				v8::Local<v8::Object> modObj = mod.As<v8::Object>();
				v8::Local<v8::String> defaultKey = v8::String::NewFromUtf8(GetV8Isolate(), "default").ToLocalChecked();
				v8::MaybeLocal<v8::Value> maybeDefault = modObj->Get(_context, defaultKey);

				if (!maybeDefault.IsEmpty()) {
					v8::Local<v8::Value> defaultExport = maybeDefault.ToLocalChecked();

					// Handle double-wrapped default (TypeScript interop)
					if (defaultExport->IsObject() && !defaultExport->IsFunction()) {
						v8::Local<v8::Object> defaultObj = defaultExport.As<v8::Object>();
						v8::MaybeLocal<v8::Value> maybeInnerDefault = defaultObj->Get(_context, defaultKey);
						if (!maybeInnerDefault.IsEmpty()) {
							v8::Local<v8::Value> innerDefault = maybeInnerDefault.ToLocalChecked();
							if (innerDefault->IsFunction()) {
								defaultExport = innerDefault;
							}
						}
					}

					if (defaultExport->IsFunction()) {
						v8::Local<v8::Function> initFunc = defaultExport.As<v8::Function>();
						v8::MaybeLocal<v8::Value> callResult = initFunc->Call(_context, v8::Undefined(GetV8Isolate()), 0, nullptr);

						if (!callResult.IsEmpty()) {
							v8::Local<v8::Value> retVal = callResult.ToLocalChecked();

							if (retVal->IsPromise()) {
								v8::Local<v8::Promise> promise = retVal.As<v8::Promise>();

								while (promise->State() == v8::Promise::kPending) {
									uv_run(loop, UV_RUN_ONCE);
									GetV8Isolate()->PerformMicrotaskCheckpoint();
								}

								if (promise->State() == v8::Promise::kRejected) {
									v8::String::Utf8Value error(GetV8Isolate(), promise->Result());
									L_ERROR << "Async initialization rejected: " << *error;
								}
							}
						}
					}
				}
			}
		}

		std::filesystem::current_path(old_cwd);

		return;
	}

	void Resource::Stop()
	{
		if (!nodeEnvironment.get()) {
			return;
		}
		
		v8::Locker locker(GetV8Isolate());
		v8::Isolate::Scope isolateScope(GetV8Isolate());
		v8::HandleScope handleScope(GetV8Isolate());

		node::Stop(nodeEnvironment.get());
		node::FreeEnvironment(nodeEnvironment.get());
		nodeEnvironment.release(); // Release without calling deleter
		
		// Force context cleanup to prevent inspector references from surviving
		context.Reset();
		GetV8Isolate()->PerformMicrotaskCheckpoint();
	}

	void Resource::RunCode(const std::string& source)
	{
		v8::Locker locker(GetV8Isolate());
		v8::Isolate::Scope isolateScope(GetV8Isolate());

		auto _context = context.Get(GetV8Isolate());

		v8::Context::Scope contextScope(_context);

		const v8::Local<v8::String>& sourceV8String = v8::String::NewFromUtf8(GetV8Isolate(), source.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
		v8::Local<v8::Script> script = v8::Script::Compile(_context, sourceV8String).ToLocalChecked();
		v8::MaybeLocal<v8::Value> result = script->Run(_context);
		if (result.IsEmpty()) {
			L_ERROR << "Script execution failed in RunCode()";
		}
	}

	v8::Local<v8::Value> Resource::AddModule(const std::string& source, const std::string& name)
	{
		v8::Isolate* isolate = GetV8Isolate();
		v8::Locker v8Locker(isolate);
		v8::Isolate::Scope isolate_scope(isolate);
		v8::HandleScope hs(isolate);
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::Context> ctx = v8::Local<v8::Context>::New(isolate, context);
		v8::Context::Scope context_scope(ctx);

		auto scriptname = v8::String::NewFromUtf8(isolate, name.c_str()).ToLocalChecked();
		v8::ScriptOrigin origin(scriptname);
		auto sourceCode = v8::String::NewFromUtf8(isolate, source.c_str());

		v8::TryCatch try_catch(isolate);

		auto script = v8::Script::Compile(ctx, sourceCode.ToLocalChecked(), &origin);

		if (script.IsEmpty())
		{
			isolate->CancelTerminateExecution();
			v8::String::Utf8Value exception(isolate, try_catch.Exception());
			const char* exception_string = *exception;
			v8::Local<v8::Message> message = try_catch.Message();

			if (message.IsEmpty())
			{
				L_ERROR << exception_string;
			}
			else
			{
				v8::String::Utf8Value filename(isolate, message->GetScriptOrigin().ResourceName());
				const char* filename_string = *filename;
				int linenum = message->GetLineNumber(ctx).ToChecked();

				L_ERROR << filename_string << ":" << linenum << ": " << exception_string;
				v8::String::Utf8Value sourceline(isolate, message->GetSourceLine(ctx).ToLocalChecked());
				const char* sourceline_string = *sourceline;
				L_INFO << sourceline_string;
			}
		}
		else
		{
			try_catch.Reset();
			v8::Local<v8::Value> result = script.ToLocalChecked()->Run(ctx).ToLocalChecked();
			if (try_catch.HasCaught())
			{
				isolate->CancelTerminateExecution();
				v8::String::Utf8Value exception(isolate, try_catch.Exception());
				const char* exception_string = *exception;
				v8::Local<v8::Message> message = try_catch.Message();

				if (message.IsEmpty())
				{
					L_ERROR << exception_string;
				}
				else
				{
					v8::String::Utf8Value filename(isolate, message->GetScriptOrigin().ResourceName());
					const char* filename_string = *filename;
					int linenum = message->GetLineNumber(ctx).ToChecked();

					L_ERROR << filename_string << ":" << linenum << ": " << exception_string;
					v8::String::Utf8Value sourceline(isolate, message->GetSourceLine(ctx).ToLocalChecked());
					const char* sourceline_string = *sourceline;
					L_INFO << sourceline_string;
				}
				v8::Local<v8::Value> ret;
				return ret;
			}

			return handle_scope.Escape(result);
		}
		return v8::Local<v8::Value>();
	}

	/*void v8val::add_definition(const std::string& name, const std::string& value, v8::Local<v8::ObjectTemplate>& global)
	{
		v8::Local<v8::Value> test = v8::String::NewFromUtf8(GetV8Isolate(), value.c_str(), v8::NewStringType::kNormal, static_cast<int>(value.length())).ToLocalChecked();
		global->Set(v8::String::NewFromUtf8(GetV8Isolate(), name.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), test, v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete));
	}
*/