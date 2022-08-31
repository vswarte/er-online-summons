#include "statemachine/states/SummonSpawnedSummonBuddyState.h"

namespace EROnlineSummons {
    SummonSpawnedSummonBuddyState::SummonSpawnedSummonBuddyState(
        SummonBuddyManager *summonBuddyManager,
        SummonNetworking *summonNetworking,
        int buddyGoodsId
    ) {
        SummonSpawnedSummonBuddyState(summonBuddyManager, summonNetworking, buddyGoodsId, nullptr);
    }

    SummonSpawnedSummonBuddyState::SummonSpawnedSummonBuddyState(
        SummonBuddyManager *summonBuddyManager,
        SummonNetworking *summonNetworking,
        int buddyGoodsId,
        SummonBuddySpawnOrigin *spawnOrigin
    ) {
        _summonBuddyManager = summonBuddyManager;
        _summonNetworking = summonNetworking;
        _buddyGoodsId = buddyGoodsId;
        _spawnOrigin = spawnOrigin;
    }

    SummonBuddyStateMachine::State SummonSpawnedSummonBuddyState::GetStateEnum() {
        return SummonBuddyStateMachine::SUMMON_SPAWNED;
    }

    bool SummonSpawnedSummonBuddyState::ValidatePreviousState(SummonBuddyStateMachine::State previousState) {
        return stateEquals(SummonBuddyStateMachine::NO_SUMMONS, previousState) ||
            stateEquals(SummonBuddyStateMachine::SUMMON_REQUESTED, previousState);
    }

    void SummonSpawnedSummonBuddyState::Enter() {
        Logging::WriteLine("Entering Summon Spawned state with buddy goods %i", _buddyGoodsId);

        if(!_summonNetworking->ShouldNetwork()) {
            _summonBuddyManager->SpawnSummons(_buddyGoodsId);
            return;
        }

        if (_summonNetworking->HasAuthority()) {
            _summonBuddyManager->SpawnSummons(_buddyGoodsId);
            _summonNetworking->SendSummonSpawned(_buddyGoodsId, _summonBuddyManager->GetSpawnOrigin());
        } else {
            if (_spawnOrigin == nullptr) {
                Logging::WriteLine("Entering Summon Spawned state, but didn't get spawnOrigin");
            }
            _summonBuddyManager->SpawnSummons(_buddyGoodsId, _spawnOrigin);
        }
    }

    void SummonSpawnedSummonBuddyState::Exit() {
        Logging::WriteLine("Exiting Summon Spawned State");
        _summonBuddyManager->QueueRemoveAllSummons();
    }
}
