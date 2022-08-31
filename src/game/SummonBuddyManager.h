#pragma once

#include "core/Memory.h"
#include "core/Logging.h"
#include "game/Offsets.h"
#include  <Processthreadsapi.h>

namespace EROnlineSummons {
    struct SummonBuddySpawnOrigin {
        float x;
        float y;
        float z;
        float unk0xc;
        float angle;
    };

    class SummonBuddyManager {
    public:
        void SpawnSummons(int buddyGoodsId);
        void SpawnSummons(int buddyGoodsId, SummonBuddySpawnOrigin *origin);
        void QueueRemoveAllSummons();
        SummonBuddySpawnOrigin *GetSpawnOrigin();

    private:
        bool prepareSummonSpawn(uintptr_t summonBuddyManager, int buddyGoodsId);
        void executeSummonSpawn(uintptr_t summonBuddyManager, int buddyGoodsId);
        void placeSummonSpawn(uintptr_t summonBuddyManager, SummonBuddySpawnOrigin *origin);

        uintptr_t getSummonBuddyManagerBase();
    };
}
