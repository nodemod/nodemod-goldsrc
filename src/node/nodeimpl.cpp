#include <cstring>
#include <unordered_map>
#include "nodeimpl.hpp"
#include "resource.hpp"

#include <sstream>

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
	}
}

void NodeImpl::Initialize()
{
	v8Platform = node::MultiIsolatePlatform::Create(4);
	v8::V8::InitializePlatform(v8Platform.get());

	const char *flags = "";
	v8::V8::SetFlagsFromString(flags, strlen(flags));

	v8::V8::Initialize();

	arrayBufferAllocator = node::ArrayBufferAllocator::Create();

	nodeLoop = std::make_unique<UvLoop>("mainNode");

	v8Isolate = node::NewIsolate(arrayBufferAllocator.get(), nodeLoop->GetLoop(), v8Platform.get());
	v8Isolate->SetFatalErrorHandler([](const char *location, const char *message)
																	{
				printf("V8 FATAL [%s]: %s\n", location, message);
				exit(0); });

	v8Isolate->SetCaptureStackTraceForUncaughtExceptions(true);
	v8Isolate->AddMessageListener(OnMessage);

	v8::Locker locker(v8Isolate);
	v8::Isolate::Scope isolateScope(v8Isolate);

	std::vector<std::string> args{"--trace-uncaught", "--inspect"};
	node::InitializeOncePerProcess(args, node::ProcessInitializationFlags::kNoFlags);

	auto isolateData = node::CreateIsolateData(v8Isolate, nodeLoop->GetLoop(), v8Platform.get(), arrayBufferAllocator.get());

	nodeData.reset(isolateData);
}

bool NodeImpl::loadScript()
{
	resource = new Resource("main", ".");
	resource->Init();

	while (!isRun)
	{
		nodeImpl.Tick();
	}

	return true;
}

void NodeImpl::Stop()
{
	resource->Stop();
	node::FreeIsolateData(nodeData.get());
	v8::V8::Dispose();
	// v8::V8::ShutdownPlatform() - removed in V8 v24
}