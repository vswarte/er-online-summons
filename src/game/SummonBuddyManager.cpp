#include "SummonBuddyManager.h"

namespace EROnlineSummons {
    void SummonBuddyManager::SpawnSummons(int buddyGoodsId) {
        auto summonBuddyManager = getSummonBuddyManagerBase();
        auto prepareSuccess = prepareSummonSpawn(summonBuddyManager, buddyGoodsId);

        if (prepareSuccess) {
            executeSummonSpawn(summonBuddyManager, buddyGoodsId);
        }
    }

    void SummonBuddyManager::SpawnSummons(int buddyGoodsId, SummonBuddySpawnOrigin *origin) {
        auto summonBuddyManager = getSummonBuddyManagerBase();
        auto prepareSuccess = prepareSummonSpawn(summonBuddyManager, buddyGoodsId);

        placeSummonSpawn(summonBuddyManager, origin);

        if (prepareSuccess) {
            executeSummonSpawn(summonBuddyManager, buddyGoodsId);
        }
    }

    void SummonBuddyManager::QueueRemoveAllSummons() {
        auto summonBuddyManager = getSummonBuddyManagerBase();
        *(char *) (summonBuddyManager + 0x28) = 1;
    }

    SummonBuddySpawnOrigin *SummonBuddyManager::GetSpawnOrigin() {
        auto summonBuddyManager = getSummonBuddyManagerBase();
        return new SummonBuddySpawnOrigin {
            x: *(float *) (summonBuddyManager + 0xa0),
            y: *(float *) (summonBuddyManager + 0xa4),
            z: *(float *) (summonBuddyManager + 0xa8),
            unk0xc: *(float *) (summonBuddyManager + 0xac),
            angle: *(float *) (summonBuddyManager + 0xb0),
        };
    }

    uintptr_t SummonBuddyManager::getSummonBuddyManagerBase() {
        auto worldChrManImp = *(uintptr_t *) (GetBaseAddress() + OFFSET_WORLD_CHR_MANAGER_IMP);
        return *(uintptr_t *) (worldChrManImp + OFFSET_WORLD_CHR_MANAGER_IMP_SUMMON_BUDDY_MANAGER);
    }

    bool SummonBuddyManager::prepareSummonSpawn(uintptr_t summonBuddyManager, int buddyGoodsId) {
        auto tlsIndex = *(int *) (GetBaseAddress() + OFFSET_TLS_INDEX_USE_BUDDY_GOODS);
        auto tlsValue = GetBaseAddress() + OFFSET_TLS_VALUE_USE_BUDDY_GOODS;
        TlsSetValue(tlsIndex, reinterpret_cast<LPVOID>(tlsValue));

        *(int *) (summonBuddyManager + 0xd8) = buddyGoodsId;
        char(*prepareSpawnSummons)(...);
        *(uintptr_t*)&prepareSpawnSummons = GetBaseAddress() + OFFSET_FN_PREPARE_SPAWN_SUMMONS;

        bool(*spawnSummons)(...);
        bool spawnSummonsResult = 0;
        if (prepareSpawnSummons(summonBuddyManager, (buddyGoodsId / 100) * 100) == 0) {
            *(uintptr_t*)&spawnSummons = GetBaseAddress() + OFFSET_FN_SPAWN_SUMMONS_1;
            spawnSummonsResult = spawnSummons(summonBuddyManager);
            Logging::WriteLine("Spawn FN 1");
        } else {
            *(uintptr_t*)&spawnSummons = GetBaseAddress() + OFFSET_FN_SPAWN_SUMMONS_2;
            spawnSummonsResult = spawnSummons(summonBuddyManager, buddyGoodsId);
            Logging::WriteLine("Spawn FN 2");
        }

        return spawnSummonsResult;
    }

    void SummonBuddyManager::executeSummonSpawn(uintptr_t summonBuddyManager, int buddyGoodsId) {
        *(int *) (summonBuddyManager + 0x3c) = *(int *) (summonBuddyManager + 0x38);
        *(int *) (summonBuddyManager + 0xdc) = *(int *) (summonBuddyManager + 0xd8);
        *(int *) (summonBuddyManager + 0x20) = buddyGoodsId;
    }

    // TODO: this is super hacky. I should probably hook the place where it determines the spawn location
    //   and do it there instead of relying on patching the spawn origin. Also these are probably chunk coords which
    //   has its own set of headaches.
    void SummonBuddyManager::placeSummonSpawn(uintptr_t summonBuddyManager, SummonBuddySpawnOrigin *newOrigin) {
        auto currentOrigin = (SummonBuddySpawnOrigin *) (summonBuddyManager + 0xa0);

        currentOrigin->x = newOrigin->x;
        currentOrigin->y = newOrigin->y;
        currentOrigin->z = newOrigin->z;
        currentOrigin->unk0xc = newOrigin->unk0xc;
        currentOrigin->angle = newOrigin->angle;
    }
}