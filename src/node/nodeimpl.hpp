#pragma once

#include <unordered_map>
#include "node.h"

// Forward declarations for customization storage functions
struct edict_s;
typedef struct edict_s edict_t;
struct customization_s;
typedef struct customization_s customization_t;
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

// Player customization storage for reload catch-up events
void storePlayerCustomization(edict_t* player, customization_t* custom);
void clearPlayerCustomizations(edict_t* player);