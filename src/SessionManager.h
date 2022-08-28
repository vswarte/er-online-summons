#pragma once

#include <vector>
#include <cstdint>
#include "core/Memory.h"
#include "core/Logging.h"

namespace EROnlineSummons {
    class SessionManager {
    public:
        static uint64_t GetHostSteamId();
        static std::vector<uint64_t> GetPartyMemberSteamIdsForBroadcast();
    };
}