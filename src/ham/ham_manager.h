#ifndef HAM_MANAGER_H
#define HAM_MANAGER_H

#include "ham_const.h"
#include "hook.h"
#include "gamedata.h"
#include <v8.h>
#include <map>
#include <vector>
#include <string>
#include <memory>

struct edict_s;
typedef struct edict_s edict_t;

namespace Ham {

struct HamFunctionInfo {
    const char* name;
    HamReturnType returnType;
    int paramCount;
    HamParamType params[8];
};

extern HamFunctionInfo g_hamFunctions[];

class HamManager {
public:
    static HamManager& instance();

    bool initialize(const std::string& gameDataPath);
    void shutdown();

    int registerHook(
        v8::Isolate* isolate,
        v8::Local<v8::Context> context,
        HamType function,
        const char* entityClass,
        v8::Local<v8::Function> callback,
        bool isPre
    );

    void unregisterHook(int hookId);

    int getVTableOffset(HamType function) const;
    const HamFunctionInfo* getFunctionInfo(HamType function) const;

    void* getOriginalFunction(HamType function, void** vtable) const;

    // For callbacks to access current hook state
    HamResult getCurrentResult() const { return m_currentResult; }
    void setCurrentResult(HamResult result) { m_currentResult = result; }

    void setReturnValue(v8::Isolate* isolate, v8::Local<v8::Value> value);
    v8::Local<v8::Value> getReturnValue(v8::Isolate* isolate);
    v8::Local<v8::Value> getOrigReturnValue(v8::Isolate* isolate);

    void setOrigReturnValue(v8::Isolate* isolate, v8::Local<v8::Value> value);

    v8::Isolate* getIsolate() const { return m_isolate; }
    void setIsolate(v8::Isolate* isolate) { m_isolate = isolate; }

    int getBaseOffset() const { return m_baseOffset; }

private:
    HamManager() = default;
    ~HamManager() = default;
    HamManager(const HamManager&) = delete;
    HamManager& operator=(const HamManager&) = delete;

    void** getEntityVTable(edict_t* ent) const;
    Hook* findOrCreateHook(HamType function, const char* entityClass, edict_t* ent);
    edict_t* createEntityByClass(const char* className);
    void removeEntity(edict_t* ent);

    GameData m_gameData;
    std::map<std::string, std::unique_ptr<Hook>> m_hooks;  // key: "entityClass:functionId"
    std::map<int, Hook*> m_hookIdMap;
    int m_nextHookId = 1;

    HamResult m_currentResult = HAM_UNSET;
    v8::Global<v8::Value> m_returnValue;
    v8::Global<v8::Value> m_origReturnValue;
    v8::Isolate* m_isolate = nullptr;

    int m_baseOffset = 0;
};

} // namespace Ham

#endif // HAM_MANAGER_H
