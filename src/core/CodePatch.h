#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <windows.h>

namespace EROnlineSummons {
    class CodePatch {
    public:
        CodePatch(uintptr_t target, const char* replacementBuffer, uint32_t length);
        ~CodePatch();
        void Install();
        void Uninstall();

    private:
        uintptr_t _target;
        const char *_replacementBuffer;
        const char *_originalBuffer;
        size_t _length;
    };
}
