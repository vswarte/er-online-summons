#include "core/Memory.h"

namespace EROnlineSummons {
    uintptr_t GetBaseAddress() {
        return (uintptr_t) GetModuleHandleA(MODULE_NAME);
    }
}
