#pragma once

#include "core/Logging.h"
#include "core/JumpHook.h"
#include "SummonNetworking.h"

namespace EROnlineSummons {
    class BuddySummonedHook : public JumpHook {
    public:
        BuddySummonedHook(uintptr_t target, SummonNetworking *summonNetworking);

    private:
        static BuddySummonedHook *_instance;
        static bool onInvoke(uintptr_t worldChrMan, int summonGoodsId, bool local);

        SummonNetworking *_summonNetworking = nullptr;
    };
}
