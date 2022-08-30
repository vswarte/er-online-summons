#include "SessionManager.h"

namespace EROnlineSummons {
    uint64_t SessionManager::GetHostSteamId() {
        // TODO: refactor duplicate code
        auto sessionManagerImp = *(uintptr_t *) (GetBaseAddress() + OFFSET_SESSION_MANAGER_IMP);
        if (sessionManagerImp == NULL) {
            Logging::WriteLine("Could not obtain CSSessionManagerImp");
            return 0;
        }

        return *(uint64_t *) (sessionManagerImp + 0xA0);
    }

    // TODO: this code assumes one player and is in dire need of a refactor
    std::vector<uint64_t> SessionManager::GetPartyMemberSteamIdsForBroadcast() {
        auto result = std::vector<uint64_t>();

        // TODO: refactor duplicate code
        auto sessionManagerImp = *(uintptr_t *) (GetBaseAddress() + OFFSET_SESSION_MANAGER_IMP);
        if (sessionManagerImp == NULL) {
            Logging::WriteLine("Could not obtain CSSessionManagerImp");
            return result;
        }

        auto playerConnection = *(uintptr_t *) (sessionManagerImp + OFFSET_SESSION_MANAGER_IMP_PLAYER_CONNECTION);
        if (playerConnection == NULL) {
            Logging::WriteLine("Could not obtain CSSessionManagerImp");
            return result;
        }

        // TODO: assumes 4 player
        for (auto i = 0; i < 4; i++) {
            // TODO: for the local player this will be NULL but might want to use something less naive?
            auto steamConnection = *(uintptr_t *) (playerConnection + (i * 0x100));
            if (steamConnection == NULL) {
                #ifndef NDEBUG
                Logging::WriteLine("Got player connection without steam connection!");
                #endif
                continue;
            }

            auto playerSteamId = *(int64_t *) (playerConnection + (i * 0x100) + OFFSET_PLAYER_CONNECTION_STEAM_ID);
            if (playerSteamId == NULL) {
                Logging::WriteLine("Steam ID of player is NULL");
                continue;
            }

            Logging::WriteLine("Retrieved steam ID of %llu", playerSteamId);
            result.emplace_back(playerSteamId);
        }

        return result;
    }
}
