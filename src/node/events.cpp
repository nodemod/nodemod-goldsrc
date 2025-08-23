#include <algorithm>
#include <vector>
#include "uv.h"
#include "node.h"
#include "events.hpp"
#include "common/logger.hpp"
#include "utils.hpp"

	eventsContainer events = eventsContainer();

	bool event::register_event(const std::string& eventName, const std::string& param_types)
	{
		if (events.find(eventName) != events.end()) return false;
		events.insert({ eventName, new event(eventName, param_types) });
		return true;
	}

	void event::register_event(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		if (info.Length() > 1)
		{
			auto isolate = info.GetIsolate();
			v8::Locker locker(isolate);
			v8::Isolate::Scope isolateScope(isolate);
			v8::HandleScope scope(isolate);

			v8::Local<v8::Context> context = isolate->GetCurrentContext();
			v8::Context::Scope contextScope(context);

			if (!info[0]->IsString() || !info[1]->IsString())
			{
				info.GetReturnValue().Set(false);
				return;
			}
			else
			{
				std::string eventName = utils::js_to_string(isolate, info[0]);
				std::string paramTypes = utils::js_to_string(isolate, info[1]);
				if (events.find(eventName) != events.end())
				{
					info.GetReturnValue().Set(false);
					return;
				}
				events.insert({ eventName, new event(eventName, paramTypes) });
				info.GetReturnValue().Set(true);
			}
		}
	}

	void event::on(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		if (info.Length() > 0)
		{
			auto isolate = info.GetIsolate();
			v8::Locker locker(isolate);
			v8::Isolate::Scope isolateScope(isolate);
			v8::HandleScope scope(isolate);

			v8::Local<v8::Context> context = isolate->GetCurrentContext();
			v8::Context::Scope contextScope(context);

			if (!info[0]->IsString())
				return;

			int funcArgIndex = info.Length() - 1;
			std::string eventName = utils::js_to_string(isolate, info[0]);

			if (events.find(eventName) == events.end()) return;
			event* _event = events[eventName];

			if ((funcArgIndex >= 0) && (info[funcArgIndex]->IsFunction()))
			{
				v8::Local<v8::Function> function = info[funcArgIndex].As<v8::Function>();
				_event->append(context, function);
			}
		}
	}

	void event::clearListeners(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
			/*auto isolate = info.GetIsolate();
			v8::Locker locker(isolate);
			v8::Isolate::Scope isolateScope(isolate);
			v8::HandleScope scope(isolate);

			v8::Local<v8::Context> _context = isolate->GetCurrentContext();

			v8::Context::Scope contextScope(_context);*/

			events.clear();
	}

	void event::remove_listener(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		if (info.Length() > 0)
		{
			auto isolate = info.GetIsolate();
			v8::Locker locker(isolate);
			v8::Isolate::Scope isolateScope(isolate);
			v8::HandleScope scope(isolate);

			v8::Local<v8::Context> _context = isolate->GetCurrentContext();

			v8::Context::Scope contextScope(_context);

			if (!info[0]->IsString())
				return;

			std::string eventName = utils::js_to_string(isolate, info[0]);

			if (events.find(eventName) == events.end()) return;

			event* _event = events[eventName];

			if (info.Length() > 1)
			{
				if (info[1]->IsArray())
				{
					v8::Local<v8::Array> funcArray = v8::Local<v8::Array>::Cast(info[1]);
					for (unsigned int i = 0; i < funcArray->Length(); i++)
					{
						const v8::Local<v8::Function>& function = funcArray->Get(_context, i).ToLocalChecked().As<v8::Function>();
						for (auto& element : _event->functionList)
						{
							if (element.context == _context && element.function.Get(isolate) == function)
							{
								_event->remove(element);
								break;
							}
						}

					}
				}
				else
				{
					v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(info[1]);
					for (auto& element : _event->functionList) {
						if (element.context == _context && element.function.Get(isolate) == function)
						{
							_event->remove(element);
							break;
						}
					}
				}
			}
			else if (info.Length() == 1)
			{
				auto copiedFunctionList = _event->functionList;

				for (auto& element : copiedFunctionList) {
					if (element.context == _context)
					{
						_event->remove(element);
					}
				}
			}
		}
	}

	void event::fire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		if (info.Length() > 0)
		{
			auto isolate = info.GetIsolate();
			v8::Locker locker(isolate);
			v8::Isolate::Scope isolateScope(isolate);
			v8::HandleScope scope(isolate);

			if (!info[0]->IsString())
				return;

			std::string eventName = utils::js_to_string(isolate, info[0]);

			if (events.find(eventName) == events.end()) return;
			event* _event = events[eventName];

			v8::Local<v8::Value>* argv = NULL;
			unsigned int argc = info.Length() - 1;
			argv = new v8::Local<v8::Value>[argc];

			for (int i = 0; i < argc; i++)
			{
				argv[i] = info[i + 1];
			}

			_event->call(argv, argc);
		}
	}

	void event::findAndCall(const std::string& eventName, v8::Local<v8::Value>* args, int argCount) {
		auto iter = events.find(eventName);
		if (iter != events.end())
			iter->second->call(args, argCount);
	}

	void event::findAndCall(const std::string& eventName, argument_collector_t collectArguments) {
		auto iter = events.find(eventName);
		if (iter != events.end())
			iter->second->call(collectArguments);
	}

	event::event(const std::string& eventName, const std::string& param_types)
		: name(eventName),
		paramTypes(param_types),
		functionList(std::vector<EventListener_t>())
	{
	}

	event::event()
	{

	}

	event::~event()
	{

	}

	void event::append(const v8::Local<v8::Context>& context, const v8::Local<v8::Function>& function)
	{
		v8::Isolate* isolate = function->GetIsolate();

		bool result = std::any_of(functionList.cbegin(), functionList.cend(),
			[&function, &isolate](const EventListener_t& listener)
			{
				return listener.function.Get(isolate) == function;
			});

		if (result)
		{
			return;
		}

		functionList.push_back(
			EventListener_t(
				isolate,
				context,
				function
			)
		);
	}

	void event::remove(const EventListener_t& eventListener)
	{
		functionList.erase(std::remove(functionList.begin(), functionList.end(), eventListener), functionList.end());
	}

	void event::remove_all()
	{
		functionList.clear();
	}

	void event::call(v8::Local<v8::Value>* args, int argCount)
	{
		std::vector<EventListener_t> copiedFunctionList = functionList;

		for (auto& listener : copiedFunctionList)
		{
			if (std::find(functionList.begin(), functionList.end(), listener) == functionList.end()) {
				continue;
			}

			v8::Isolate* isolate = listener.isolate;
			v8::Locker v8Locker(isolate);
			v8::Isolate::Scope isolateScope(isolate);
			v8::HandleScope hs(isolate);
			v8::Local<v8::Context> ctx = listener.context.Get(isolate);
			v8::Context::Scope cs(ctx);

			isolate->CancelTerminateExecution();

			v8::TryCatch eh(isolate);

			v8::Local<v8::Function> function = listener.function.Get(isolate);
			function->Call(ctx, ctx->Global(), argCount, args);


			if (eh.HasCaught())
			{
				v8::String::Utf8Value str(isolate, eh.Exception());
				v8::String::Utf8Value stack(isolate, eh.StackTrace(listener.context.Get(isolate)).ToLocalChecked());

				L_ERROR << "Event handling function in resource: " << *str << "\nstack:\n" << *stack << "\n";
			}
		}

		if (argCount > 0) delete[] args;
	}

void event::call(argument_collector_t collectArguments)
	{
		std::vector<EventListener_t> copiedFunctionList = functionList;

		for (auto& listener : copiedFunctionList)
		{
			if (std::find(functionList.begin(), functionList.end(), listener) == functionList.end()) {
				continue;
			}

			v8::Isolate* isolate = listener.isolate;
			v8::Locker v8Locker(isolate);
			v8::Isolate::Scope isolateScope(isolate);
			v8::HandleScope hs(isolate);
			v8::Local<v8::Context> ctx = listener.context.Get(isolate);
			v8::Context::Scope cs(ctx);

			isolate->CancelTerminateExecution();

			v8::TryCatch eh(isolate);


			//	L_DEBUG << "PreCollected: " << "\n";
			std::pair<unsigned int, v8::Local<v8::Value>*> arguments = collectArguments(isolate);
			unsigned int argCount = arguments.first;
			v8::Local<v8::Value>* args = arguments.second;
			//	L_DEBUG << "Collected: " << argCount << "\n";

			v8::Local<v8::Function> function = listener.function.Get(isolate);
			function->Call(ctx, ctx->Global(), argCount, args);


			if (eh.HasCaught())
			{
				v8::String::Utf8Value str(isolate, eh.Exception());
				v8::String::Utf8Value stack(isolate, eh.StackTrace(listener.context.Get(isolate)).ToLocalChecked());

				L_ERROR << "Event handling function in resource: " << *str << "\nstack:\n" << *stack << "\n";
			}
		}
	}