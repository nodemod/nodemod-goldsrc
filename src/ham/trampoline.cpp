#include "trampoline.h"

namespace Ham {

void* createThiscallTrampoline(int paramCount, void* hookPtr, void* callback, size_t* outSize) {
    TrampolineMaker tramp;

#if defined(__i386__) || defined(_M_IX86)
    // x86 32-bit: Create trampoline that calls callback(Hook*, this, params...)
    tramp.prologue(true);

    // Push params in reverse order
    for (int i = paramCount; i >= 1; i--) {
        tramp.pushParam(i);
    }

    // Push this pointer
    tramp.pushThis();

    // Push Hook pointer
    tramp.pushPointer(hookPtr);

    // Call the callback
    tramp.call(callback);

    // Free the stack (hook ptr + this + params)
    int stackToFree = (2 + paramCount) * sizeof(void*);
    tramp.freeStack(stackToFree);

#if defined(_WIN32)
    // Windows thiscall: callee cleans this from stack
    tramp.epilogueN(paramCount * 4);
#else
    // Linux: caller cleans stack
    tramp.epilogue();
#endif

#elif defined(__x86_64__) || defined(_M_X64)
    // x86-64: Use register calling convention
    // callback(Hook*, this, param1, param2, ...)
    tramp.prologue(true);
    tramp.saveArgs(paramCount);

    // Set up args for callback call
    // First arg (rdi/rcx): Hook pointer
    tramp.loadHookPtrToArg(hookPtr);

    // Second arg (rsi/rdx): this pointer (was saved from first reg)
    tramp.loadSecondArg(nullptr);

    // For additional params, we'd need to shift them but for now
    // the callback signature is simplified

    tramp.call(callback);
    tramp.epilogue();
#endif

    return tramp.finish(outSize);
}

} // namespace Ham
