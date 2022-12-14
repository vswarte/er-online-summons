#pragma once

#include "networking/SummonNetworking.h"
#include "statemachine/SummonBuddyStateMachine.h"

namespace EROnlineSummons {
    class SummonSpawnRequestedSummonBuddyState : public SummonBuddyState {
    public:
        SummonSpawnRequestedSummonBuddyState(
            SummonNetworking *summonNetworking,
            int buddyGoodsId
        );
        SummonBuddyStateMachine::State GetStateEnum() override;

        bool ValidatePreviousState(SummonBuddyStateMachine::State previousState) override;
        void Enter() override;
        void Exit() override;

    private:
        SummonNetworking *_summonNetworking = nullptr;
        int _buddyGoodsId;
    };
}
