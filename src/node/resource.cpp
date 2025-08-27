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

		for (auto& flag : args)
		{
			L_DEBUG << "node flags: " << flag;
		}

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
		node::LoadEnvironment(env, node::StartExecutionCallback{});
		
		std::filesystem::current_path(old_cwd);

		nodeEnvironment.reset(env);

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