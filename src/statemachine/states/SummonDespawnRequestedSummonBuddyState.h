#pragma once

#include "networking/SummonNetworking.h"
#include "statemachine/SummonBuddyStateMachine.h"

namespace EROnlineSummons {
    class SummonDespawnRequestedSummonBuddyState : public SummonBuddyState {
    public:
        SummonDespawnRequestedSummonBuddyState(SummonNetworking *summonNetworking);

        SummonBuddyStateMachine::State GetStateEnum() override;

        bool ValidatePreviousState(SummonBuddyStateMachine::State previousState) override;
        void Enter() override;
        void Exit() override;

    private:
        SummonNetworking *_summonNetworking = nullptr;
    };
}
