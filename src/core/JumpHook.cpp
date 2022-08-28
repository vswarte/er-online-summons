#include "core/JumpHook.h"

namespace EROnlineSummons {
    JumpHook::JumpHook(uintptr_t target, uintptr_t replacement) {
        _target = target;
        _replacement = replacement;
    }

    JumpHook::~JumpHook() {
    }

    bool JumpHook::Install() {
        if (MH_CreateHook((void *)_target, (void *)_replacement, &_original)) {
            return false;
        }

        if (MH_EnableHook((void *)_target) != MH_OK) {
            return false;
        }

        return true;
    }

    bool JumpHook::Uninstall() {
        if (MH_DisableHook((void *)_target) != MH_OK) {
            return false;
        }

        return true;
    }

    uintptr_t JumpHook::GetOriginal() {
        return (uintptr_t) _original;
    }
}
