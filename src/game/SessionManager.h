#pragma once

#include <vector>
#include <cstdint>
#include "core/Memory.h"
#include "core/Logging.h"
#include "game/Offsets.h"

enum NetworkState : uint32_t {
    None = 0x0,
    Host = 0x3,
    Client = 0x6,
};

namespace EROnlineSummons {
    class SessionManager {
    public:
        static uint64_t GetHostSteamId();

        static bool IsInSession();
        static bool IsHost();

        // TODO: Apply this kind of filtering outside of the SessionManager
        static std::vector<uint64_t> GetPartyMemberSteamIdsForBroadcast();
    private:
        static uintptr_t getSessionManagerBase();
        static NetworkState getNetworkState();
    };
}