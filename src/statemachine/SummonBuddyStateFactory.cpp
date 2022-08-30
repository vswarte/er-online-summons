#include "SummonBuddyStateFactory.h"

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

    SummonRequestedSummonBuddyState *SummonBuddyStateFactory::CreateSummonRequestedState(int buddyGoodsId) {
        return new SummonRequestedSummonBuddyState(_summonBuddyManager, _summonNetworking, buddyGoodsId);
    }

    NoSummonSummonBuddyState *SummonBuddyStateFactory::CreateNoSummonState() {
        return new NoSummonSummonBuddyState();
    }
}
