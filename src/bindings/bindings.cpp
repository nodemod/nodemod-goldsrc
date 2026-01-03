#include "bindings.hpp"
#include "util/convert.hpp"
#include "structures/structures.hpp"
#include "ham/ham_bindings.h"
#include <v8.h>
#include <string>
#include <vector>
#include <filesystem>
#include "extdll.h"
#include "enginecallback.h"

extern std::vector<std::pair<std::string, v8::FunctionCallback>> nodemodFunctions;
extern enginefuncs_t g_engfuncs;
extern v8::Local<v8::ObjectTemplate> registerEngineFunctions(v8::Isolate *isolate);
extern v8::Local<v8::ObjectTemplate> registerDllFunctions(v8::Isolate *isolate);
namespace players
{
  extern void getPlayers(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info);
}

extern globalvars_t *gpGlobals;

namespace bindings
{
  void init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global)
  {
    v8::Locker locker(isolate);
    v8::Local<v8::ObjectTemplate> nodemodObject = v8::ObjectTemplate::New(isolate);

    // add base functions
    for (auto &routine : nodemodFunctions)
    {
      nodemodObject->Set(
          convert::str2js(isolate, routine.first.c_str()),
          v8::FunctionTemplate::New(isolate, routine.second));
    }

    // add engine functions
    nodemodObject->Set(
        convert::str2js(isolate, "eng"),
        registerEngineFunctions(isolate));

    // add dll functions
    nodemodObject->Set(
        convert::str2js(isolate, "dll"),
        registerDllFunctions(isolate));

    // add ham functions
    nodemodObject->Set(
        convert::str2js(isolate, "ham"),
        Ham::createHamBindings(isolate));

    auto cwd = std::filesystem::current_path();

    nodemodObject->Set(
        convert::str2js(isolate, "cwd"),
        convert::str2js(isolate, cwd.string().c_str()));

    // add getters
    nodemodObject->SetNativeDataProperty(
        convert::str2js(isolate, "gameDir"),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info)
        {
          v8::Isolate* isolate = info.GetIsolate();
          v8::Locker locker(isolate);
          v8::HandleScope scope(isolate);

          char gameDir[256];
          (*g_engfuncs.pfnGetGameDir)(gameDir);
          info.GetReturnValue().Set(convert::str2js(isolate, gameDir));
        });
    nodemodObject->SetNativeDataProperty(
        convert::str2js(isolate, "players"),
        players::getPlayers);

    nodemodObject->SetNativeDataProperty(
        convert::str2js(isolate, "mapname"),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info)
        {
          v8::Isolate* isolate = info.GetIsolate();
          v8::Locker locker(isolate);
          v8::HandleScope scope(isolate);

          info.GetReturnValue().Set(convert::strid2js(isolate, gpGlobals->mapname));
        });

    nodemodObject->SetNativeDataProperty(
        convert::str2js(isolate, "time"),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info)
        {
          v8::Isolate* isolate = info.GetIsolate();
          v8::Locker locker(isolate);
          v8::HandleScope scope(isolate);

          info.GetReturnValue().Set(v8::Number::New(isolate, gpGlobals->time));
        });

    nodemodObject->SetNativeDataProperty(
        convert::str2js(isolate, "frametime"),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info)
        {
          v8::Isolate* isolate = info.GetIsolate();
          v8::Locker locker(isolate);
          v8::HandleScope scope(isolate);

          info.GetReturnValue().Set(v8::Number::New(isolate, gpGlobals->frametime));
        });

    global->Set(convert::str2js(isolate, "nodemod"), nodemodObject);
    structures::createEntityTemplate(isolate, global);
  }
}
