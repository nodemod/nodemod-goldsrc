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

    // Create trampoline
    m_trampoline = createThiscallTrampoline(paramCount, this, target, &m_trampolineSize);

    // Patch vtable
    patchVTable(m_trampoline);
}

Hook::~Hook() {
    // Restore original vtable entry (unless skipped during shutdown)
    if (!m_skipRestore) {
        restoreVTable();
    }

    // Free trampoline memory
    if (m_trampoline) {
        TrampolineMaker::freeTrampoline(m_trampoline, m_trampolineSize);
    }

    // Clear callbacks
    m_preCallbacks.clear();
    m_postCallbacks.clear();
}

void Hook::patchVTable(void* newFunc) {
#if defined(_WIN32)
    DWORD oldProtect;
    VirtualProtect(&m_vtable[m_entry], sizeof(void*), PAGE_READWRITE, &oldProtect);
    m_vtable[m_entry] = newFunc;
    VirtualProtect(&m_vtable[m_entry], sizeof(void*), oldProtect, &oldProtect);
#else
    void* pageAddr = ALIGN_PAGE(&m_vtable[m_entry]);

    int ret = mprotect(pageAddr, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE);
    if (ret != 0) {
        return;
    }

    m_vtable[m_entry] = newFunc;
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
