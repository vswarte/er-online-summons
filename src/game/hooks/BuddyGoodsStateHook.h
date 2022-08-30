#pragma once

#include "core/Logging.h"
#include "core/JumpHook.h"
#include "statemachine/SummonBuddyStateMachine.h"

namespace EROnlineSummons {
    class BuddyGoodsStateHook : public JumpHook {
    public:
        BuddyGoodsStateHook(uintptr_t target, SummonBuddyStateMachine *stateMachine);

    private:
        enum BuddyGoodsStates {
            Unusable = -1,
            Usable = 0,
            AlreadySpawned = 2,
        };

        static BuddyGoodsStateHook *_instance;
        static BuddyGoodsStates onInvoke(uintptr_t worldChrMan, int buddyGoodsId);

        SummonBuddyStateMachine *_stateMachine = nullptr;
    };
}
