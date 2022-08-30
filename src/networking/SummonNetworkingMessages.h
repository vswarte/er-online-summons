#pragma once

namespace EROnlineSummons {
    enum class SummonNetworkMessageType : char {
        SummonSpawned = 0x1,
        RequestSummonSpawn = 0x2,
    };

    struct __attribute__ ((__packed__)) SummonNetworkMessageHeader {
        SummonNetworkMessageType type;
    };

    struct __attribute__ ((__packed__)) SummonSpawnedMessage {
        SummonNetworkMessageHeader header;
        int buddyGoodsId;
    };

    struct __attribute__ ((__packed__)) RequestSummonSpawnMessage {
        SummonNetworkMessageHeader header;
        int buddyGoodsId;
    };
}