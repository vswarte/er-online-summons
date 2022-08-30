#include "SessionManager.h"

namespace EROnlineSummons {
    uint64_t SessionManager::GetHostSteamId() {
        return *(uint64_t *) (getSessionManagerBase() + 0xA0);
    }

    bool SessionManager::IsInSession() {
        return getNetworkState() != NetworkState::None;
    }

    bool SessionManager::IsHost() {
        return getNetworkState() == NetworkState::Host;
    }

    // TODO: this code is in dire need of a refactor
    std::vector<uint64_t> SessionManager::GetPartyMemberSteamIdsForBroadcast() {
        auto result = std::vector<uint64_t>();

        auto sessionManagerImp = getSessionManagerBase();

        auto playerConnection = *(uintptr_t *) (sessionManagerImp + OFFSET_SESSION_MANAGER_IMP_PLAYER_CONNECTION);
        if (playerConnection == NULL) {
            Logging::WriteLine("Could not obtain player connection list");
            return result;
        }

        // TODO: assumes 4 player
        for (auto i = 0; i < 4; i++) {
            // TODO: for the local player this will be NULL but might want to use something less naive?
            auto steamConnection = *(uintptr_t *) (playerConnection + (i * 0x100));
            if (steamConnection == NULL) {
                Logging::WriteLine("Got player connection without steam connection!");
                continue;
            }

            auto playerSteamId = *(int64_t *) (playerConnection + (i * 0x100) + OFFSET_PLAYER_CONNECTION_STEAM_ID);
            if (playerSteamId == NULL) {
                Logging::WriteLine("Steam ID of player is NULL");
                continue;
            }

            #ifndef NDEBUG
            Logging::WriteLine("Retrieved steam ID of %llu", playerSteamId);
            #endif
            result.emplace_back(playerSteamId);
        }

        return result;
    }

    uintptr_t SessionManager::getSessionManagerBase() {
        auto sessionManagerImp = *(uintptr_t *) (GetBaseAddress() + OFFSET_SESSION_MANAGER_IMP);
        if (sessionManagerImp == NULL) {
            Logging::WriteLine("Could not obtain CSSessionManagerImp");
            return 0;
        }

        return sessionManagerImp;
    }

    NetworkState SessionManager::getNetworkState() {
        return *(NetworkState *) (getSessionManagerBase() + 0xC);
    }
}
