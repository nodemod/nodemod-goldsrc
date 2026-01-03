#ifndef HAM_TRAMPOLINE_H
#define HAM_TRAMPOLINE_H

#include <cstddef>
#include <cstdint>
#include <cstring>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace Ham {

class TrampolineMaker {
private:
    unsigned char* m_buffer;
    size_t m_size;
    size_t m_capacity;
    int m_paramCount;
    bool m_isThiscall;

    void append(const unsigned char* src, size_t size) {
        if (m_buffer == nullptr) {
            m_capacity = 512;
            m_buffer = new unsigned char[m_capacity];
        } else if (m_size + size > m_capacity) {
            m_capacity = m_size + size + 512;
            unsigned char* newBuf = new unsigned char[m_capacity];
            std::memcpy(newBuf, m_buffer, m_size);
            delete[] m_buffer;
            m_buffer = newBuf;
        }
        std::memcpy(m_buffer + m_size, src, size);
        m_size += size;
    }

    void appendByte(unsigned char b) {
        append(&b, 1);
    }

    void appendDword(uint32_t val) {
        unsigned char bytes[4];
        bytes[0] = val & 0xFF;
        bytes[1] = (val >> 8) & 0xFF;
        bytes[2] = (val >> 16) & 0xFF;
        bytes[3] = (val >> 24) & 0xFF;
        append(bytes, 4);
    }

#if defined(__x86_64__) || defined(_M_X64)
    void appendQword(uint64_t val) {
        unsigned char bytes[8];
        for (int i = 0; i < 8; i++) {
            bytes[i] = (val >> (i * 8)) & 0xFF;
        }
        append(bytes, 8);
    }
#endif

public:
    TrampolineMaker() : m_buffer(nullptr), m_size(0), m_capacity(0),
                         m_paramCount(0), m_isThiscall(false) {}

    ~TrampolineMaker() {
        if (m_buffer) {
            delete[] m_buffer;
        }
    }

#if defined(__i386__) || defined(_M_IX86)
    // x86 32-bit implementation
    void prologue(bool thiscall) {
        m_isThiscall = thiscall;
        appendByte(0x55);           // push ebp
        appendByte(0x89);           // mov ebp, esp
        appendByte(0xE5);
    }

    void pushThis() {
        if (!m_isThiscall) return;
#if defined(_WIN32)
        appendByte(0x51);           // push ecx (this pointer in Windows thiscall)
#else
        // Linux: this is first param at [ebp+8]
        appendByte(0xFF);           // push [ebp+8]
        appendByte(0x75);
        appendByte(0x08);
#endif
    }

    void pushParam(int paramNum) {
#if defined(__linux__) || defined(__APPLE__)
        if (m_isThiscall) paramNum++;
#endif
        int offset = 4 + (paramNum * 4);  // ebp+4 is return addr
        appendByte(0xFF);           // push [ebp+offset]
        appendByte(0x75);
        appendByte(static_cast<unsigned char>(offset));
    }

    void pushPointer(void* ptr) {
        appendByte(0x68);           // push imm32
        appendDword(reinterpret_cast<uint32_t>(ptr));
    }

    void call(void* func) {
        appendByte(0xB8);           // mov eax, imm32
        appendDword(reinterpret_cast<uint32_t>(func));
        appendByte(0xFF);           // call eax
        appendByte(0xD0);
    }

    void freeStack(int bytes) {
        appendByte(0x81);           // add esp, imm32
        appendByte(0xC4);
        appendDword(bytes);
    }

    void epilogue() {
        appendByte(0x89);           // mov esp, ebp
        appendByte(0xEC);
        appendByte(0x5D);           // pop ebp
        appendByte(0xC3);           // ret
    }

    void epilogueN(int stackBytes) {
        appendByte(0x89);           // mov esp, ebp
        appendByte(0xEC);
        appendByte(0x5D);           // pop ebp
        appendByte(0xC2);           // ret imm16
        appendByte(stackBytes & 0xFF);
        appendByte((stackBytes >> 8) & 0xFF);
    }

#elif defined(__x86_64__) || defined(_M_X64)
    // x86-64 implementation
    // System V AMD64 ABI: rdi, rsi, rdx, rcx, r8, r9, then stack
    // Microsoft x64: rcx, rdx, r8, r9, then stack (with shadow space)

    void prologue(bool thiscall) {
        m_isThiscall = thiscall;  // In x64, this is just first arg (rdi/rcx)

        // push rbp; mov rbp, rsp
        appendByte(0x55);
        appendByte(0x48); appendByte(0x89); appendByte(0xE5);

#if defined(_WIN32)
        // Reserve shadow space (32 bytes) + alignment
        appendByte(0x48); appendByte(0x83); appendByte(0xEC); appendByte(0x40);
#else
        // Align stack to 16 bytes, reserve space for saved regs
        appendByte(0x48); appendByte(0x83); appendByte(0xEC); appendByte(0x40);
#endif
    }

    void saveArgs(int paramCount) {
        m_paramCount = paramCount;
        // Save register arguments to stack for later access
#if defined(_WIN32)
        // Save rcx, rdx, r8, r9 to shadow space
        if (paramCount >= 1 || m_isThiscall) {
            appendByte(0x48); appendByte(0x89); appendByte(0x4C); appendByte(0x24); appendByte(0x08); // mov [rsp+8], rcx
        }
        if (paramCount >= 2) {
            appendByte(0x48); appendByte(0x89); appendByte(0x54); appendByte(0x24); appendByte(0x10); // mov [rsp+16], rdx
        }
        if (paramCount >= 3) {
            appendByte(0x4C); appendByte(0x89); appendByte(0x44); appendByte(0x24); appendByte(0x18); // mov [rsp+24], r8
        }
        if (paramCount >= 4) {
            appendByte(0x4C); appendByte(0x89); appendByte(0x4C); appendByte(0x24); appendByte(0x20); // mov [rsp+32], r9
        }
#else
        // Save rdi, rsi, rdx, rcx, r8, r9 to local space
        if (paramCount >= 1 || m_isThiscall) {
            appendByte(0x48); appendByte(0x89); appendByte(0x7D); appendByte(0xF8); // mov [rbp-8], rdi
        }
        if (paramCount >= 2) {
            appendByte(0x48); appendByte(0x89); appendByte(0x75); appendByte(0xF0); // mov [rbp-16], rsi
        }
        if (paramCount >= 3) {
            appendByte(0x48); appendByte(0x89); appendByte(0x55); appendByte(0xE8); // mov [rbp-24], rdx
        }
        if (paramCount >= 4) {
            appendByte(0x48); appendByte(0x89); appendByte(0x4D); appendByte(0xE0); // mov [rbp-32], rcx
        }
        if (paramCount >= 5) {
            appendByte(0x4C); appendByte(0x89); appendByte(0x45); appendByte(0xD8); // mov [rbp-40], r8
        }
        if (paramCount >= 6) {
            appendByte(0x4C); appendByte(0x89); appendByte(0x4D); appendByte(0xD0); // mov [rbp-48], r9
        }
#endif
    }

    void pushThis() {
        // In x64, this is just the first register arg - handled by saveArgs
    }

    void pushParam(int paramNum) {
        // Not used in x64 - we set up register args directly
    }

    void loadThisToFirstArg() {
#if defined(_WIN32)
        // mov rcx, [rsp+8]
        appendByte(0x48); appendByte(0x8B); appendByte(0x4C); appendByte(0x24); appendByte(0x08);
#else
        // mov rdi, [rbp-8]
        appendByte(0x48); appendByte(0x8B); appendByte(0x7D); appendByte(0xF8);
#endif
    }

    void loadHookPtrToArg(void* hookPtr) {
#if defined(_WIN32)
        // mov rcx, imm64 (hook ptr as first arg)
        appendByte(0x48); appendByte(0xB9);
        appendQword(reinterpret_cast<uint64_t>(hookPtr));
#else
        // mov rdi, imm64 (hook ptr as first arg)
        appendByte(0x48); appendByte(0xBF);
        appendQword(reinterpret_cast<uint64_t>(hookPtr));
#endif
    }

    void loadSecondArg(void* thisPtr) {
#if defined(_WIN32)
        // mov rdx, [rsp+8] (this was saved here)
        appendByte(0x48); appendByte(0x8B); appendByte(0x54); appendByte(0x24); appendByte(0x08);
#else
        // mov rsi, [rbp-8] (this was saved here)
        appendByte(0x48); appendByte(0x8B); appendByte(0x75); appendByte(0xF8);
#endif
    }

    void pushPointer(void* ptr) {
        // In x64, we use registers not push for args
    }

    void call(void* func) {
        // mov rax, imm64
        appendByte(0x48); appendByte(0xB8);
        appendQword(reinterpret_cast<uint64_t>(func));
        // call rax
        appendByte(0xFF); appendByte(0xD0);
    }

    void freeStack(int bytes) {
        // Not typically needed in x64
    }

    void epilogue() {
        // leave; ret
        appendByte(0xC9);  // leave
        appendByte(0xC3);  // ret
    }

    void epilogueN(int stackBytes) {
        appendByte(0xC9);  // leave
        appendByte(0xC3);  // ret (x64 uses caller cleanup)
    }
#endif

    void* finish(size_t* outSize = nullptr) {
        if (outSize) *outSize = m_size;

        void* execMem = nullptr;
#if defined(_WIN32)
        execMem = VirtualAlloc(nullptr, m_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
        execMem = mmap(nullptr, m_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

        if (execMem) {
            std::memcpy(execMem, m_buffer, m_size);
        }

        delete[] m_buffer;
        m_buffer = nullptr;
        m_size = 0;
        m_capacity = 0;

        return execMem;
    }

    static void freeTrampoline(void* tramp, size_t size) {
#if defined(_WIN32)
        VirtualFree(tramp, 0, MEM_RELEASE);
#else
        munmap(tramp, size);
#endif
    }
};

void* createThiscallTrampoline(int paramCount, void* hookPtr, void* callback, size_t* outSize);

} // namespace Ham

#endif // HAM_TRAMPOLINE_H
