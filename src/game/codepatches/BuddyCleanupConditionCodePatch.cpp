#include "game/codepatches/BuddyCleanupConditionCodePatch.h"

namespace EROnlineSummons {
    BuddyCleanupConditionCodePatch::BuddyCleanupConditionCodePatch(uintptr_t target)
        : CodePatch(target, "\xeb\x18\x90\x90", 4) {
    }
}
