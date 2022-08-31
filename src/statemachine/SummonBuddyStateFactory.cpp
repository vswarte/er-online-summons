#include "statemachine/SummonBuddyStateFactory.h"

namespace EROnlineSummons {
    SummonBuddyStateFactory::SummonBuddyStateFactory(
        SummonBuddyManager *summonBuddyManager,
        SummonNetworking *summonNetworking
    ) {
        _summonBuddyManager = summonBuddyManager;
        _summonNetworking = summonNetworking;
    }

    SummonSpawnedSummonBuddyState *SummonBuddyStateFactory::CreateSummonSpawnedState(int buddyGoodsId) {
        return new SummonSpawnedSummonBuddyState(_summonBuddyManager, _summonNetworking, buddyGoodsId);
    }

    SummonSpawnedSummonBuddyState *SummonBuddyStateFactory::CreateSummonSpawnedState(int buddyGoodsId, SummonBuddySpawnOrigin *spawnOrigin) {
        return new SummonSpawnedSummonBuddyState(_summonBuddyManager, _summonNetworking, buddyGoodsId, spawnOrigin);
    }

    SummonRequestedSummonBuddyState *SummonBuddyStateFactory::CreateSummonRequestedState(int buddyGoodsId) {
        return new SummonRequestedSummonBuddyState(_summonNetworking, buddyGoodsId);
    }

    NoSummonSummonBuddyState *SummonBuddyStateFactory::CreateNoSummonState() {
        return new NoSummonSummonBuddyState();
    }
}
