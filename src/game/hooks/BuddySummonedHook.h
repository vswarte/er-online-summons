#pragma once

#include "core/Logging.h"
#include "core/JumpHook.h"
#include "networking/SummonNetworking.h"
#include "statemachine/SummonBuddyStateFactory.h"

namespace EROnlineSummons {
    class BuddySummonedHook : public JumpHook {
    public:
        BuddySummonedHook(
            uintptr_t target,
            SummonBuddyStateMachine *stateMachine,
            SummonBuddyStateFactory *stateFactory,
            SummonNetworking *summonNetworking
        );

    private:
        static BuddySummonedHook *_instance;
        static bool onInvoke(uintptr_t worldChrMan, int buddyGoodsId);

        SummonBuddyStateMachine *_stateMachine = nullptr;
        SummonBuddyStateFactory *_stateFactory = nullptr;
        SummonNetworking *_summonNetworking = nullptr;
    };
}
