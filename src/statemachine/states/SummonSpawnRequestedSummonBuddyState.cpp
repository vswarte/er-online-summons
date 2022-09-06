#include "statemachine/states/SummonSpawnRequestedSummonBuddyState.h"

namespace EROnlineSummons {
    SummonSpawnRequestedSummonBuddyState::SummonSpawnRequestedSummonBuddyState(
            SummonNetworking *summonNetworking,
            int buddyGoodsId
    ) {
        _summonNetworking = summonNetworking;
        _buddyGoodsId = buddyGoodsId;
    }

    SummonBuddyStateMachine::State SummonSpawnRequestedSummonBuddyState::GetStateEnum() {
        return SummonBuddyStateMachine::SUMMON_SPAWN_REQUESTED;
    }

    bool SummonSpawnRequestedSummonBuddyState::ValidatePreviousState(SummonBuddyStateMachine::State previousState) {
        return stateEquals(SummonBuddyStateMachine::NO_SUMMONS, previousState);
    }

    void SummonSpawnRequestedSummonBuddyState::Enter() {
        Logging::WriteLine("Entering Summon Spawn Requested State with buddy goods %i", _buddyGoodsId);
        _summonNetworking->SendSummonSpawnRequest(_buddyGoodsId);
    }

    void SummonSpawnRequestedSummonBuddyState::Exit() {
        Logging::WriteLine("Exiting Summon Spawn Requested State");
    }
}
