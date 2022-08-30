#pragma once

#include "game/SummonBuddyManager.h"
#include "statemachine/SummonBuddyStateMachine.h"

namespace EROnlineSummons {
    class NoSummonSummonBuddyState : public SummonBuddyState {
    public:
        SummonBuddyStateMachine::State GetStateEnum() override;

        bool ValidatePreviousState(SummonBuddyStateMachine::State previousState) override;
        void Enter() override;
        void Exit() override;
    };
}
