#pragma once

#include "core/Logging.h"
#include "core/JumpHook.h"
#include "networking/SummonNetworking.h"

namespace EROnlineSummons {
    class BuddySummonedHook : public JumpHook {
    public:
        BuddySummonedHook(uintptr_t target, SummonNetworking *summonNetworking);

    private:
        static BuddySummonedHook *_instance;
        static bool onInvoke(uintptr_t worldChrMan, int buddyGoodsId, bool local);
        static bool invokeOriginal(uintptr_t worldChrMan, int buddyGoodsId);

        static void handleSummonSpawn(int buddyGoodsId, bool isHost, bool isLocalInvocation);
        static void handleSummonDespawn(int buddyGoodsId, bool isHost, bool isLocalInvocation);

        SummonNetworking *_summonNetworking = nullptr;
    };
}
