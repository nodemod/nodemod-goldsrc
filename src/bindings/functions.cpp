#include <utility>
#include <string>
#include <vector>
#include "v8.h"
#include "node/events.hpp"

extern void getUserMsgId(const v8::FunctionCallbackInfo<v8::Value>& info);
extern void getUserMsgName(const v8::FunctionCallbackInfo<v8::Value>& info);
extern void setMetaResult(const v8::FunctionCallbackInfo<v8::Value>& info);
extern void getMetaResult(const v8::FunctionCallbackInfo<v8::Value>& info);
extern void continueHandler(const v8::FunctionCallbackInfo<v8::Value>& info);

std::vector<std::pair<std::string, v8::FunctionCallback>> nodemodFunctions =
{
	{ "on", event::on },
	{ "addEventListener", event::on },
	{ "addListener", event::on },
	{ "clearListeners", event::clearListeners },
	{ "removeListener", event::remove_listener },
	{ "removeEventListener", event::remove_listener },
	{ "fire", event::fire },
	{ "getUserMsgId", getUserMsgId },
	{ "getUserMsgName", getUserMsgName },
	{ "setMetaResult", setMetaResult },
	{ "getMetaResult", getMetaResult },
	{ "continueServer", continueHandler }
};
