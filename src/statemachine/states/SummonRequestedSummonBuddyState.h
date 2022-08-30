#pragma once

#include "networking/SummonNetworking.h"
#include "statemachine/SummonBuddyStateMachine.h"

namespace EROnlineSummons {
    class SummonRequestedSummonBuddyState : public SummonBuddyState {
    public:
        SummonRequestedSummonBuddyState(
                SummonBuddyManager *summonBuddyManager,
                SummonNetworking *summonNetworking,
                int buddyGoodsId
        );
        SummonBuddyStateMachine::State GetStateEnum() override;

        bool ValidatePreviousState(SummonBuddyStateMachine::State previousState) override;
        void Enter() override;
        void Exit() override;

    private:
        SummonBuddyManager *_summonBuddyManager = nullptr;
        SummonNetworking *_summonNetworking = nullptr;
        int _buddyGoodsId;
    };
}
