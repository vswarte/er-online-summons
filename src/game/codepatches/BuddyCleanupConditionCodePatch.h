#pragma once

#include "core/CodePatch.h"

namespace EROnlineSummons {
    class BuddyCleanupConditionCodePatch : public CodePatch {
    public:
        BuddyCleanupConditionCodePatch(uintptr_t target);
    };
}
