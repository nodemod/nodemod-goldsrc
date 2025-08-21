#pragma once
#define NODE_WANT_INTERNALS 1

// Include HLSDK headers first to define enginefuncs_t
#include "extdll.h"
#include "enginecallback.h"

// Include Node.js headers after HLSDK
#include "node.h"
#include "uv.h"
#include "v8.h"
#include "libplatform/libplatform.h"

// Node internals - include carefully
#include "env.h"
//#include "utils.hpp"

	class Resource
	{
	public:
		Resource(const std::string& name, const std::string& path);
		Resource();
		~Resource();

		void Init();
		void Stop();

		void RunCode(const std::string& source);
		v8::Local<v8::Value> AddModule(const std::string& source, const std::string& name);

		inline v8::UniquePersistent<v8::Context>& GetContext()
		{
			return context;
		}

		node::Environment* GetEnv()
		{
			return nodeEnvironment.get();
		}

	private:
		v8::UniquePersistent<v8::Context> context;
		std::unique_ptr<node::Environment, decltype(&node::FreeEnvironment)> nodeEnvironment;
		std::string path;
		std::string name;
	};
/*
	namespace v8val
	{
		inline std::string to_string(v8::Isolate *isolate, const v8::Local<v8::Value>& val) { return utils::js_to_string(isolate, val); }
		inline const char* to_cstring(v8::Isolate *isolate, const v8::Local<v8::Value>& val) { return utils::js_to_cstr(isolate, val); }
		void add_definition(const std::string& name, const std::string& value, v8::Local<v8::ObjectTemplate>& global);
	}
*/