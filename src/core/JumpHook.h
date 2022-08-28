#pragma once

#include <memory>
#include "minhook/MinHook.h"

namespace EROnlineSummons {
    class JumpHook {
    public:
        JumpHook(uintptr_t original, uintptr_t replacement);
        ~JumpHook();

        uintptr_t GetOriginal();
        bool Uninstall();
        bool Install();

    private:
        uintptr_t _target;
        uintptr_t _replacement;
        void *_original;
    };
}
