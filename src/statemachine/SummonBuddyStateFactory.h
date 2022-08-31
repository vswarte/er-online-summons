#pragma once

#include "networking/SummonNetworking.h"
#include "statemachine/states/NoSummonSummonBuddyState.h"
#include "statemachine/states/SummonSpawnedSummonBuddyState.h"
#include "statemachine/states/SummonRequestedSummonBuddyState.h"

namespace EROnlineSummons {
    class SummonBuddyStateFactory {
    public:
        SummonBuddyStateFactory(
            SummonBuddyManager *summonBuddyManager,
            SummonNetworking *summonNetworking
        );

        SummonSpawnedSummonBuddyState *CreateSummonSpawnedState(int buddyGoodsId);
        SummonSpawnedSummonBuddyState *CreateSummonSpawnedState(int buddyGoodsId, SummonBuddySpawnOrigin *spawnOrigin);
        SummonRequestedSummonBuddyState *CreateSummonRequestedState(int buddyGoodsId);
        NoSummonSummonBuddyState *CreateNoSummonState();

    private:
        SummonBuddyManager *_summonBuddyManager = nullptr;
        SummonNetworking *_summonNetworking = nullptr;
    };
}