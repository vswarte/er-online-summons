#pragma once

#include <vector>
#include <cstdint>
#include "core/Memory.h"
#include "core/Logging.h"

namespace EROnlineSummons {
    class SessionManager {
    public:
        static uint64_t GetHostSteamId();

        // TODO: Apply this kind of filtering outside of the SessionManager
        static std::vector<uint64_t> GetPartyMemberSteamIdsForBroadcast();
    };
}