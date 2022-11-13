#pragma once

#include <memory>
#include "minhook/MinHook.h"

namespace EROnlineSummons {
    class JumpHook {
    public:
        JumpHook(uintptr_t original, uintptr_t replacement);
        ~JumpHook();

        uintptr_t GetTarget();
        uintptr_t GetOriginal();
        virtual bool Uninstall();
        virtual bool Install();

    private:
        uintptr_t _target;
        uintptr_t _replacement;
        static void *_original;
    };
}
