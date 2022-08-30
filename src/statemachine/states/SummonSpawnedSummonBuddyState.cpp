#include "statemachine/states/SummonSpawnedSummonBuddyState.h"

namespace EROnlineSummons {
    SummonSpawnedSummonBuddyState::SummonSpawnedSummonBuddyState(
        SummonBuddyManager *summonBuddyManager,
        SummonNetworking *summonNetworking,
        int buddyGoodsId
    ) {
        _summonBuddyManager = summonBuddyManager;
        _summonNetworking = summonNetworking;
        _buddyGoodsId = buddyGoodsId;
    }

    SummonBuddyStateMachine::State SummonSpawnedSummonBuddyState::GetStateEnum() {
        return SummonBuddyStateMachine::SUMMON_SPAWNED;
    }

    bool SummonSpawnedSummonBuddyState::ValidatePreviousState(SummonBuddyStateMachine::State previousState) {
        return stateEquals(SummonBuddyStateMachine::NO_SUMMONS, previousState) ||
            stateEquals(SummonBuddyStateMachine::SUMMON_REQUESTED, previousState);
    }

    void SummonSpawnedSummonBuddyState::Enter() {
        Logging::WriteLine("Entering Summon Spawned State with buddy goods %i", _buddyGoodsId);

        _summonBuddyManager->SpawnSummons(_buddyGoodsId);
        if (_summonNetworking->HasAuthority()) {
            _summonNetworking->SendSummonSpawned(_buddyGoodsId);
        }
    }

    void SummonSpawnedSummonBuddyState::Exit() {
        Logging::WriteLine("Exiting Summon Spawned State");
        _summonBuddyManager->QueueRemoveAllSummons();
    }
}
