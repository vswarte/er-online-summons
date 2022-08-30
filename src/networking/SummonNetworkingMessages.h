#pragma once

namespace EROnlineSummons {
    enum class SummonNetworkMessageType : char {
        SummonSpawned = 0x1,
        SummonRequested = 0x2,
    };

    struct __attribute__ ((__packed__)) SummonMessageHeader {
        SummonNetworkMessageType type;
    };

    struct __attribute__ ((__packed__)) SummonSpawnedMessage {
        SummonMessageHeader header;
        int buddyGoodsId;
    };

    struct __attribute__ ((__packed__)) SummonRequestedMessage {
        SummonMessageHeader header;
        int buddyGoodsId;
    };
}