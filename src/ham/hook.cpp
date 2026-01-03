#include "hook.h"
#include <cstring>
#include <cerrno>

#if defined(__linux__) || defined(__APPLE__)
#define ALIGN_PAGE(addr) ((void*)((uintptr_t)(addr) & ~(sysconf(_SC_PAGESIZE) - 1)))
#endif

namespace Ham {

Hook::Hook(void** vtable, int entry, void* target, int paramCount, const char* entityName)
    : m_vtable(vtable)
    , m_entry(entry)
    , m_originalFunc(nullptr)
    , m_trampoline(nullptr)
    , m_trampolineSize(0)
    , m_entityName(entityName)
{
    // Save original function from vtable
    m_originalFunc = vtable[entry];

    printf("[HAM] Creating hook: vtable=%p entry=%d originalFunc=%p paramCount=%d entity=%s\n",
           vtable, entry, m_originalFunc, paramCount, entityName);

    // Dump nearby VTable entries for context
    printf("[HAM] VTable context (entries %d to %d):\n",
           (entry > 3 ? entry - 3 : 0), entry + 3);
    for (int i = (entry > 3 ? entry - 3 : 0); i <= entry + 3; i++) {
        printf("  vtable[%d] = %p%s\n", i, vtable[i], (i == entry) ? " <-- target" : "");
    }

    // IMPORTANT: Dump more vtable entries to help identify function layout
    printf("[HAM] Extended VTable dump (0-20):\n");
    for (int i = 0; i <= 20 && i < 100; i++) {
        printf("  vtable[%d] = %p\n", i, vtable[i]);
    }

    // DEBUG: Set to true to skip trampoline creation
    static bool skipTrampoline = false;
    if (skipTrampoline) {
        printf("[HAM] DEBUG: Skipping trampoline creation\n");
        return;
    }

    // Create trampoline
    m_trampoline = createThiscallTrampoline(paramCount, this, target, &m_trampolineSize);

    printf("[HAM] Trampoline created: addr=%p size=%zu target=%p\n",
           m_trampoline, m_trampolineSize, target);
    printf("[HAM] Hook object 'this' = %p\n", (void*)this);

    // Patch vtable
    patchVTable(m_trampoline);

    printf("[HAM] VTable patched: vtable[%d] now = %p (was %p)\n",
           entry, vtable[entry], m_originalFunc);

    // Verify patch was successful
    if (vtable[entry] != m_trampoline) {
        printf("[HAM] ERROR: VTable patch failed! vtable[%d] = %p, expected %p\n",
               entry, vtable[entry], m_trampoline);
    }
}

Hook::~Hook() {
    // Restore original vtable entry
    restoreVTable();

    // Free trampoline memory
    if (m_trampoline) {
        TrampolineMaker::freeTrampoline(m_trampoline, m_trampolineSize);
    }

    // Clear callbacks
    m_preCallbacks.clear();
    m_postCallbacks.clear();
}

void Hook::patchVTable(void* newFunc) {
    // DEBUG: Set to true to completely disable VTable patching for testing
    static bool skipPatch = false;
    if (skipPatch) {
        printf("[HAM] DEBUG: Skipping VTable patch (disabled for testing)\n");
        return;
    }

#if defined(_WIN32)
    DWORD oldProtect;
    VirtualProtect(&m_vtable[m_entry], sizeof(void*), PAGE_READWRITE, &oldProtect);
    m_vtable[m_entry] = newFunc;
    VirtualProtect(&m_vtable[m_entry], sizeof(void*), oldProtect, &oldProtect);
#else
    void* pageAddr = ALIGN_PAGE(&m_vtable[m_entry]);
    printf("[HAM] patchVTable: vtable=%p entry=%d pageAddr=%p newFunc=%p\n",
           m_vtable, m_entry, pageAddr, newFunc);
    fflush(stdout);

    int ret = mprotect(pageAddr, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE);
    if (ret != 0) {
        printf("[HAM] ERROR: mprotect(WRITE) failed: %s\n", strerror(errno));
        fflush(stdout);
        return;
    }

    m_vtable[m_entry] = newFunc;

    // Leave page writable - something else on this page needs write access
    printf("[HAM] VTable patched successfully\n");
    fflush(stdout);
#endif
}

void Hook::restoreVTable() {
#if defined(_WIN32)
    DWORD oldProtect;
    VirtualProtect(&m_vtable[m_entry], sizeof(void*), PAGE_READWRITE, &oldProtect);
    m_vtable[m_entry] = m_originalFunc;
    VirtualProtect(&m_vtable[m_entry], sizeof(void*), oldProtect, &oldProtect);
#else
    void* pageAddr = ALIGN_PAGE(&m_vtable[m_entry]);
    mprotect(pageAddr, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE);
    m_vtable[m_entry] = m_originalFunc;
    // Leave page writable - something else on this page needs write access
    printf("[HAM] VTable restored successfully\n");
    fflush(stdout);
#endif
}

int Hook::addCallback(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Function> callback, bool isPre) {
    HamCallback cb;
    cb.callback.Reset(isolate, callback);
    cb.context.Reset(isolate, context);
    cb.isPre = isPre;
    cb.id = m_nextCallbackId++;

    if (isPre) {
        m_preCallbacks.push_back(std::move(cb));
    } else {
        m_postCallbacks.push_back(std::move(cb));
    }

    return cb.id;
}

void Hook::addPreCallback(int id, v8::Global<v8::Function>&& callback) {
    HamCallback cb;
    cb.callback = std::move(callback);
    cb.isPre = true;
    cb.id = id;
    m_preCallbacks.push_back(std::move(cb));
}

void Hook::addPostCallback(int id, v8::Global<v8::Function>&& callback) {
    HamCallback cb;
    cb.callback = std::move(callback);
    cb.isPre = false;
    cb.id = id;
    m_postCallbacks.push_back(std::move(cb));
}

void Hook::removeCallback(int callbackId) {
    auto removeFrom = [callbackId](std::vector<HamCallback>& vec) {
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            if (it->id == callbackId) {
                it->callback.Reset();
                it->context.Reset();
                vec.erase(it);
                return true;
            }
        }
        return false;
    };

    if (!removeFrom(m_preCallbacks)) {
        removeFrom(m_postCallbacks);
    }
}

} // namespace Ham
