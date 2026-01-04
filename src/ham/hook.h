#ifndef HAM_HOOK_H
#define HAM_HOOK_H

#include "ham_const.h"
#include "trampoline.h"
#include <v8.h>
#include <vector>
#include <string>
#include <functional>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace Ham {

struct HamCallback {
    v8::Global<v8::Function> callback;
    v8::Global<v8::Context> context;
    bool isPre;
    int id;
};

class Hook {
public:
    Hook(void** vtable, int entry, void* target, int paramCount, const char* entityName);
    ~Hook();

    int addCallback(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Function> callback, bool isPre);
    void addPreCallback(int id, v8::Global<v8::Function>&& callback);
    void addPostCallback(int id, v8::Global<v8::Function>&& callback);
    void removeCallback(int callbackId);

    void* getOriginalFunction() const { return m_originalFunc; }
    void** getVTable() const { return m_vtable; }
    int getEntry() const { return m_entry; }
    const std::string& getEntityName() const { return m_entityName; }

    std::vector<HamCallback>& getPreCallbacks() { return m_preCallbacks; }
    std::vector<HamCallback>& getPostCallbacks() { return m_postCallbacks; }

    bool isExecuting() const { return m_executing; }
    void setExecuting(bool val) { m_executing = val; }

    bool shouldDelete() const { return m_pendingDelete; }
    void markForDeletion() { m_pendingDelete = true; }

    void skipVTableRestore() { m_skipRestore = true; }

private:
    void** m_vtable;
    int m_entry;
    void* m_originalFunc;
    void* m_trampoline;
    size_t m_trampolineSize;
    std::string m_entityName;

    std::vector<HamCallback> m_preCallbacks;
    std::vector<HamCallback> m_postCallbacks;

    bool m_executing = false;
    bool m_pendingDelete = false;
    bool m_skipRestore = false;  // Skip vtable restoration in destructor
    int m_nextCallbackId = 1;

    void patchVTable(void* newFunc);
    void restoreVTable();
};

} // namespace Ham

#endif // HAM_HOOK_H
