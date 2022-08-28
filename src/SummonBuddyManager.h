#pragma once

#include "core/Memory.h"
#include "core/Logging.h"
#include  <Processthreadsapi.h>

namespace EROnlineSummons {
    class SummonBuddyManager {
    public:
        void SpawnSummons(int buddyGoodsId);
    };
}
