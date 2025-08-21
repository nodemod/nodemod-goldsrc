#pragma once
#include <unordered_map>
#include <map>
#include "node.h"
#include "v8.h"
#include "uv.h"

using argument_collector_t = std::function<
	std::pair<unsigned int, v8::Local<v8::Value>*>(v8::Isolate* isolate)
>;

	class event
	{
	public:
		struct EventListener_t
		{
			v8::Isolate* isolate;
			v8::Global<v8::Context> context;
			v8::Global<v8::Function> function;

			EventListener_t(const EventListener_t &listener) {
				isolate = listener.isolate;
				context.Reset(isolate, listener.context);
				function.Reset(isolate, listener.function);
			}

			EventListener_t(
				v8::Isolate* _isolate,
				const v8::Global<v8::Context>& _context,
				const v8::Global<v8::Function>& _function
			)
			{
				isolate = _isolate;
				v8::Locker locker(isolate);
				context.Reset(_isolate, _context);
				function.Reset(_isolate, _function);
			}

			EventListener_t(
				v8::Isolate* _isolate,
				const v8::Local<v8::Context>& _context,
				const v8::Local<v8::Function>& _function
			)
			{
				isolate = _isolate;
				v8::Locker locker(isolate);
				context.Reset(_isolate, _context);
				function.Reset(_isolate, _function);
			}

			~EventListener_t() {
				context.Reset();
				function.Reset();
			}
			
			EventListener_t& operator=(const EventListener_t& other) {
				if (this != &other) {
					isolate = other.isolate;
					context.Reset(isolate, other.context);
					function.Reset(isolate, other.function);
				}
				return *this;
			}

			EventListener_t& operator=(EventListener_t&& other) noexcept {
				if (this != &other) {
					isolate = other.isolate;
					context = std::move(other.context);
					function = std::move(other.function);
					other.isolate = nullptr;
				}
				return *this;
			}

			bool operator==(const EventListener_t& a) const {
				return (this->function == a.function && this->context == a.context);
			}

		};

		static void on(const v8::FunctionCallbackInfo<v8::Value>& info);
		static void remove_listener(const v8::FunctionCallbackInfo<v8::Value>& info);
		static void fire(const v8::FunctionCallbackInfo<v8::Value>& info);
		static void register_event(const v8::FunctionCallbackInfo<v8::Value>& info);
		static bool register_event(const std::string& eventName, const std::string& param_types);
		static void findAndCall(const std::string& eventName, v8::Local<v8::Value>* args, int argCount);
		static void findAndCall(const std::string& eventName, argument_collector_t collectArguments);
		static void clearListeners(const v8::FunctionCallbackInfo<v8::Value>& info);

		event(const std::string& eventName, const std::string& param_types);
		event();
		~event();

		void append(const v8::Local<v8::Context>& context, const v8::Local<v8::Function>& function);
		void remove(const EventListener_t& eventListener);
		void remove_all();
		void call(v8::Local<v8::Value>* args, int argCount);
		void call(argument_collector_t collectArguments);

		std::string get_param_types()
		{
			return paramTypes;
		}

	private:
		std::string name;
		std::string paramTypes;
		std::vector<EventListener_t> functionList;
		v8::Global<v8::Function> listener;
	};

	typedef std::unordered_map<std::string, event*> eventsContainer;
	extern eventsContainer events;
