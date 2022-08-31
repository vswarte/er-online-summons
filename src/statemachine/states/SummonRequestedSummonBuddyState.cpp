#include "statemachine/states/SummonRequestedSummonBuddyState.h"

namespace EROnlineSummons {
    SummonRequestedSummonBuddyState::SummonRequestedSummonBuddyState(
            SummonNetworking *summonNetworking,
            int buddyGoodsId
    ) {
        _summonNetworking = summonNetworking;
        _buddyGoodsId = buddyGoodsId;
    }

    SummonBuddyStateMachine::State SummonRequestedSummonBuddyState::GetStateEnum() {
        return SummonBuddyStateMachine::SUMMON_REQUESTED;
    }

    bool SummonRequestedSummonBuddyState::ValidatePreviousState(SummonBuddyStateMachine::State previousState) {
        return stateEquals(SummonBuddyStateMachine::NO_SUMMONS, previousState);
    }

    void SummonRequestedSummonBuddyState::Enter() {
        Logging::WriteLine("Entering Summon Requested State with buddy goods %i", _buddyGoodsId);
        _summonNetworking->SendRequestSummonSpawn(_buddyGoodsId);
    }

    void SummonRequestedSummonBuddyState::Exit() {
        Logging::WriteLine("Exiting Summon Requested State");
    }
}
