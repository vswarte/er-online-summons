#pragma once

#include "core/Memory.h"
#include "core/Logging.h"
#include "game/Offsets.h"
#include  <Processthreadsapi.h>

namespace EROnlineSummons {
    class SummonBuddyManager {
    public:
        void SpawnSummons(int buddyGoodsId);
        void QueueRemoveAllSummons();

    private:
        uintptr_t getSummonBuddyManagerBase();
    };
}
