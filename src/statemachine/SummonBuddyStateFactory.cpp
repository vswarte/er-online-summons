#include "statemachine/SummonBuddyStateFactory.h"

namespace EROnlineSummons {
    SummonBuddyStateFactory::SummonBuddyStateFactory(
        SummonBuddyManager *summonBuddyManager,
        SummonNetworking *summonNetworking
    ) {
        _summonBuddyManager = summonBuddyManager;
        _summonNetworking = summonNetworking;
    }

    NoSummonSummonBuddyState *SummonBuddyStateFactory::CreateNoSummonState() {
        return new NoSummonSummonBuddyState();
    }

    SummonSpawnRequestedSummonBuddyState *SummonBuddyStateFactory::CreateSummonSpawnRequestedState(int buddyGoodsId) {
        return new SummonSpawnRequestedSummonBuddyState(_summonNetworking, buddyGoodsId);
    }

    SummonSpawnedSummonBuddyState *SummonBuddyStateFactory::CreateSummonSpawnedState(int buddyGoodsId) {
        return new SummonSpawnedSummonBuddyState(_summonBuddyManager, _summonNetworking, buddyGoodsId);
    }

    SummonSpawnedSummonBuddyState *SummonBuddyStateFactory::CreateSummonSpawnedState(int buddyGoodsId, SummonBuddySpawnOrigin *spawnOrigin) {
        return new SummonSpawnedSummonBuddyState(_summonBuddyManager, _summonNetworking, buddyGoodsId, spawnOrigin);
    }

    SummonDespawnRequestedSummonBuddyState *SummonBuddyStateFactory::CreateSummonDespawnRequestedState() {
        return new SummonDespawnRequestedSummonBuddyState(_summonNetworking);
    }
}
