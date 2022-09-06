#pragma once

#include "networking/SummonNetworking.h"
#include "statemachine/states/NoSummonSummonBuddyState.h"
#include "statemachine/states/SummonSpawnedSummonBuddyState.h"
#include "statemachine/states/SummonSpawnRequestedSummonBuddyState.h"
#include "statemachine/states/SummonDespawnRequestedSummonBuddyState.h"

namespace EROnlineSummons {
    class SummonBuddyStateFactory {
    public:
        SummonBuddyStateFactory(
            SummonBuddyManager *summonBuddyManager,
            SummonNetworking *summonNetworking
        );

        NoSummonSummonBuddyState *CreateNoSummonState();
        SummonSpawnRequestedSummonBuddyState *CreateSummonSpawnRequestedState(int buddyGoodsId);
        SummonSpawnedSummonBuddyState *CreateSummonSpawnedState(int buddyGoodsId);
        SummonSpawnedSummonBuddyState *CreateSummonSpawnedState(int buddyGoodsId, SummonBuddySpawnOrigin *spawnOrigin);
        SummonDespawnRequestedSummonBuddyState *CreateSummonDespawnRequestedState();

    private:
        SummonBuddyManager *_summonBuddyManager = nullptr;
        SummonNetworking *_summonNetworking = nullptr;
    };
}