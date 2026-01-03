#include "ham_bindings.h"
#include "ham_const.h"
#include "ham_manager.h"
#include "../util/convert.hpp"
#include "extdll.h"
#include <filesystem>
#include <dlfcn.h>

extern enginefuncs_t g_engfuncs;

// Called during reload to clean up Ham hooks
void shutdownHamManager() {
    Ham::HamManager::instance().shutdown();
}

namespace Ham {

static std::string getHamDllPath() {
    Dl_info dl_info;
    if (dladdr((void*)getHamDllPath, &dl_info) != 0) {
        std::filesystem::path dll_path = dl_info.dli_fname;
        return dll_path.parent_path().string();
    }
    return ".";
}

static void RegisterHamHook(const v8::FunctionCallbackInfo<v8::Value>& args) {
    // DEBUG: Set to true to skip ALL ham binding code - just return immediately
    static bool skipBinding = false;
    if (skipBinding) {
        printf("[HAM] DEBUG: Skipping ham binding entirely\n");
        args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), 9999));
        return;
    }

    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    if (args.Length() < 4) {
        isolate->ThrowException(v8::Exception::TypeError(
            convert::str2js(isolate, "ham.register requires 4 arguments: functionId, entityClass, callback, isPre")));
        return;
    }

    if (!args[0]->IsNumber()) {
        isolate->ThrowException(v8::Exception::TypeError(
            convert::str2js(isolate, "First argument must be a Ham function ID (number)")));
        return;
    }

    if (!args[1]->IsString()) {
        isolate->ThrowException(v8::Exception::TypeError(
            convert::str2js(isolate, "Second argument must be an entity class name (string)")));
        return;
    }

    if (!args[2]->IsFunction()) {
        isolate->ThrowException(v8::Exception::TypeError(
            convert::str2js(isolate, "Third argument must be a callback function")));
        return;
    }

    if (!args[3]->IsBoolean()) {
        isolate->ThrowException(v8::Exception::TypeError(
            convert::str2js(isolate, "Fourth argument must be isPre (boolean)")));
        return;
    }

    int functionId = args[0]->Int32Value(context).FromMaybe(-1);
    v8::String::Utf8Value entityClass(isolate, args[1]);
    v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[2]);
    bool isPre = args[3]->BooleanValue(isolate);

    if (functionId < 0 || functionId >= Ham_EndMarker) {
        isolate->ThrowException(v8::Exception::RangeError(
            convert::str2js(isolate, "Invalid Ham function ID")));
        return;
    }

    // DEBUG: Set to true to skip manager call (test if arg parsing alone causes issues)
    static bool skipManagerCall = false;
    if (skipManagerCall) {
        printf("[HAM] DEBUG: Parsed args - functionId=%d entityClass=%s isPre=%d - skipping manager call\n",
               functionId, *entityClass, isPre);
        args.GetReturnValue().Set(v8::Integer::New(isolate, 8888));
        return;
    }

    HamManager& mgr = HamManager::instance();
    int hookId = mgr.registerHook(isolate, context, static_cast<HamType>(functionId), *entityClass, callback, isPre);

    if (hookId < 0) {
        isolate->ThrowException(v8::Exception::Error(
            convert::str2js(isolate, "Failed to register Ham hook")));
        return;
    }

    args.GetReturnValue().Set(v8::Integer::New(isolate, hookId));
}

static void UnregisterHamHook(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        isolate->ThrowException(v8::Exception::TypeError(
            convert::str2js(isolate, "ham.unregister requires a hook ID (number)")));
        return;
    }

    int hookId = args[0]->Int32Value(context).FromMaybe(-1);
    HamManager::instance().unregisterHook(hookId);
}

static void SetHamReturn(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();

    if (args.Length() < 1) {
        isolate->ThrowException(v8::Exception::TypeError(
            convert::str2js(isolate, "ham.setReturn requires a value")));
        return;
    }

    HamManager::instance().setReturnValue(isolate, args[0]);
    HamManager::instance().setCurrentResult(HAM_OVERRIDE);
}

static void GetHamReturn(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(HamManager::instance().getReturnValue(isolate));
}

static void GetOrigHamReturn(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(HamManager::instance().getOrigReturnValue(isolate));
}

static void Supercede(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HamManager::instance().setCurrentResult(HAM_SUPERCEDE);
}

static void Override(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HamManager::instance().setCurrentResult(HAM_OVERRIDE);
}

static void Handled(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HamManager::instance().setCurrentResult(HAM_HANDLED);
}

static void Ignored(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HamManager::instance().setCurrentResult(HAM_IGNORED);
}

v8::Local<v8::ObjectTemplate> createHamBindings(v8::Isolate* isolate) {
    v8::EscapableHandleScope handleScope(isolate);

    v8::Local<v8::ObjectTemplate> hamObject = v8::ObjectTemplate::New(isolate);

    // Register functions
    hamObject->Set(
        convert::str2js(isolate, "register"),
        v8::FunctionTemplate::New(isolate, RegisterHamHook));

    hamObject->Set(
        convert::str2js(isolate, "unregister"),
        v8::FunctionTemplate::New(isolate, UnregisterHamHook));

    hamObject->Set(
        convert::str2js(isolate, "setReturn"),
        v8::FunctionTemplate::New(isolate, SetHamReturn));

    hamObject->Set(
        convert::str2js(isolate, "getReturn"),
        v8::FunctionTemplate::New(isolate, GetHamReturn));

    hamObject->Set(
        convert::str2js(isolate, "getOrigReturn"),
        v8::FunctionTemplate::New(isolate, GetOrigHamReturn));

    hamObject->Set(
        convert::str2js(isolate, "supercede"),
        v8::FunctionTemplate::New(isolate, Supercede));

    hamObject->Set(
        convert::str2js(isolate, "override"),
        v8::FunctionTemplate::New(isolate, Override));

    hamObject->Set(
        convert::str2js(isolate, "handled"),
        v8::FunctionTemplate::New(isolate, Handled));

    hamObject->Set(
        convert::str2js(isolate, "ignored"),
        v8::FunctionTemplate::New(isolate, Ignored));

    // HAM_FUNC and HAM_RESULT are TypeScript const enums - they compile to literal
    // numbers, so no C++ bindings needed (same pattern as META_RES)

    // Store isolate reference for callbacks
    HamManager::instance().setIsolate(isolate);

    // Initialize HamManager with gamedata
    char gameDir[256];
    (*g_engfuncs.pfnGetGameDir)(gameDir);

    // Extract just the game name from the path (e.g., "/path/to/ts" -> "ts")
    std::filesystem::path gamePath(gameDir);
    std::string gameName = gamePath.filename().string();

    // Construct path to gamedata file
    std::string dllPath = getHamDllPath();
    std::filesystem::path gamedataPath = std::filesystem::path(dllPath)
        / ".." / "data" / "gamedata" / "common.games" / "virtual.games" / gameName / "offsets-common.txt";

    std::string gamedataFile = std::filesystem::weakly_canonical(gamedataPath).string();

    printf("[HAM] Attempting to load gamedata: %s\n", gamedataFile.c_str());
    if (!HamManager::instance().initialize(gamedataFile)) {
        // Try #default (valve) as fallback
        gamedataPath = std::filesystem::path(dllPath)
            / ".." / "data" / "gamedata" / "common.games" / "virtual.games" / "valve" / "offsets-common.txt";
        gamedataFile = std::filesystem::weakly_canonical(gamedataPath).string();
        printf("[HAM] Fallback to valve gamedata: %s\n", gamedataFile.c_str());
        HamManager::instance().initialize(gamedataFile);
    } else {
        printf("[HAM] Loaded gamedata successfully. Base offset: 0x%x\n", HamManager::instance().getBaseOffset());
    }

    return handleScope.Escape(hamObject);
}

} // namespace Ham
