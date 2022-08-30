#include "SummonBuddyManager.h"

namespace EROnlineSummons {
    // TODO: proper type defs might help a ton cleaning this up
    void SummonBuddyManager::SpawnSummons(int buddyGoodsId) {
        auto tlsIndex = *(int *) (GetBaseAddress() + OFFSET_TLS_INDEX_USE_BUDDY_GOODS);
        auto tlsValue = GetBaseAddress() + OFFSET_TLS_VALUE_USE_BUDDY_GOODS;
        TlsSetValue(tlsIndex, reinterpret_cast<LPVOID>(tlsValue));

        auto summonBuddyManager = getSummonBuddyManagerBase();

        *(int *) (summonBuddyManager + 0xd8) = buddyGoodsId;
        char(*prepareSpawnSummons)(...);
        *(uintptr_t*)&prepareSpawnSummons = GetBaseAddress() + OFFSET_FN_PREPARE_SPAWN_SUMMONS;

        bool(*spawnSummons)(...);
        bool spawnSummonsResult = 0;
        if (prepareSpawnSummons(summonBuddyManager, (buddyGoodsId / 100) * 100) == 0) {
            *(uintptr_t*)&spawnSummons = GetBaseAddress() + OFFSET_FN_SPAWN_SUMMONS_1;
            spawnSummonsResult = spawnSummons(summonBuddyManager);
        } else {
            *(uintptr_t*)&spawnSummons = GetBaseAddress() + OFFSET_FN_SPAWN_SUMMONS_2;
            spawnSummonsResult = spawnSummons(summonBuddyManager, buddyGoodsId);
        }

        #ifndef NDEBUG
        Logging::WriteLine("spawnSummons result: %i", spawnSummonsResult);
        #endif
        if (spawnSummonsResult) {
            *(int *) (summonBuddyManager + 0x3c) = *(int *) (summonBuddyManager + 0x38);
            *(int *) (summonBuddyManager + 0xdc) = *(int *) (summonBuddyManager + 0xd8);
            *(int *) (summonBuddyManager + 0x20) = buddyGoodsId;
        }
    }

    void SummonBuddyManager::QueueRemoveAllSummons() {
        auto summonBuddyManager = getSummonBuddyManagerBase();
        *(char *) (summonBuddyManager + 0x28) = 1;
    }

    uintptr_t SummonBuddyManager::getSummonBuddyManagerBase() {
        auto worldChrManImp = *(uintptr_t *) (GetBaseAddress() + OFFSET_WORLD_CHR_MANAGER_IMP);
        return *(uintptr_t *) (worldChrManImp + OFFSET_WORLD_CHR_MANAGER_IMP_SUMMON_BUDDY_MANAGER);
    }
}