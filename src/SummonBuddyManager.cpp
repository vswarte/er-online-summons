#include "SummonBuddyManager.h"

namespace EROnlineSummons {
    // TODO: proper type defs might help a ton cleaning this up
    void SummonBuddyManager::SpawnSummons(int buddyGoodsId) {
        auto tlsIndex = *(int *) (GetBaseAddress() + OFFSET_TLS_INDEX_USE_BUDDY_GOODS);
        auto tlsValue = GetBaseAddress() + OFFSET_TLS_VALUE_USE_BUDDY_GOODS;
        TlsSetValue(tlsIndex, reinterpret_cast<LPVOID>(tlsValue));

        auto worldChrManImp = *(uintptr_t *) (GetBaseAddress() + OFFSET_WORLD_CHR_MANAGER_IMP);
        auto summonBuddyManager = *(uintptr_t *) (worldChrManImp + OFFSET_WORLD_CHR_MANAGER_IMP_SUMMON_BUDDY_MANAGER);

        *(int *) (summonBuddyManager + 0xd8) = buddyGoodsId;
        void(*useBuddyGoods)(...);
        *(uintptr_t*)&useBuddyGoods = GetBaseAddress() + OFFSET_FN_USE_BUDDY_GOODS;
        useBuddyGoods(summonBuddyManager, buddyGoodsId, false);
    }
}