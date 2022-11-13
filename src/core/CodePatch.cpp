#include "core/CodePatch.h"

namespace EROnlineSummons {

    CodePatch::CodePatch(uintptr_t target, const char *replacementBuffer, uint32_t length) {
        _target = target;
        _replacementBuffer = replacementBuffer;
        _length = length;

        char originalBuffer[0x100] = "";
        _originalBuffer = &originalBuffer[0];
    }

    CodePatch::~CodePatch() {

    }

    void CodePatch::Install() {
        unsigned long oldProtection;
        VirtualProtect((void *)_target, _length, PAGE_EXECUTE_READWRITE, &oldProtection);

        // Copy original values to original buffer
        memcpy((void *)_originalBuffer, (void *)_target, _length);

        // Write copy replacement to patch location
        memcpy((void*)_target, _replacementBuffer, _length);

        VirtualProtect((void*)_target, _length, oldProtection, nullptr);
    }

    void CodePatch::Uninstall() {
        unsigned long oldProtection;
        VirtualProtect((void*)_target, _length, PAGE_EXECUTE_READWRITE, &oldProtection);

        // Restore original contents of memory region
        memcpy((void *)_target, _originalBuffer, _length);

        VirtualProtect((void*)_target, _length, oldProtection, nullptr);
    }
}
