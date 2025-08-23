#pragma once

#include <unordered_map>
#include "node.h"
#include "node_api.h"
#include "v8.h"
#include "uv.h"
#include "libplatform/libplatform.h"
#include "env-inl.h"
#include "node_internals.h"
#include "node_v8_platform-inl.h"
#include "uvloop.hpp"

class NodeImpl
{
public:
	NodeImpl();
	~NodeImpl();

	void Initialize();

	inline v8::Platform* GetPlatform()
	{
		return v8Platform.get();
	}

	inline v8::Isolate* GetIsolate()
	{
		return v8Isolate;
	}

	inline node::IsolateData* GetNodeIsolate()
	{
		return nodeData.get();
	}

	inline UvLoop* GetUVLoop()
	{
		return nodeLoop.get();
	}

	void Tick();
	void Stop();
	bool loadScript();
	bool reload();

private:
	struct IsolateDataDeleter
	{
		using pointer = node::IsolateData*;
		void operator()(node::IsolateData* p) const { 
			node::FreeIsolateData(p); 
		}
	};

	v8::Isolate* v8Isolate;
	std::unique_ptr<node::IsolateData, IsolateDataDeleter> nodeData;
	std::unique_ptr<node::MultiIsolatePlatform> v8Platform;
	std::unique_ptr<node::ArrayBufferAllocator> arrayBufferAllocator;
	std::unique_ptr<UvLoop> nodeLoop;
};

extern NodeImpl nodeImpl;