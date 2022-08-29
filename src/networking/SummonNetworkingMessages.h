#pragma once

namespace EROnlineSummons {
    enum class SummonNetworkMessageType : char {
        SummonSpawned = 0x1,
    };

    struct __attribute__ ((__packed__)) SummonNetworkMessageHeader {
        SummonNetworkMessageType type;
    };

    struct __attribute__ ((__packed__)) SummonSpawnedMessage {
        SummonNetworkMessageHeader header;
        int buddyGoodsId;
    };
}