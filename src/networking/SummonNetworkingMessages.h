#pragma once

namespace EROnlineSummons {
    enum class SummonNetworkMessageType : char {
        SummonSpawn = 0x1,
        SummonSpawnRequest = 0x2,
        SummonDespawn = 0x3,
        SummonDespawnRequest = 0x4,
    };

    struct __attribute__ ((__packed__)) SummonMessageHeader {
        SummonNetworkMessageType type;
    };

    struct __attribute__ ((__packed__)) SummonSpawnMessage {
        SummonMessageHeader header;
        int buddyGoodsId;
        float spawnOriginX;
        float spawnOriginY;
        float spawnOriginZ;
        float unk0xc;
        float spawnAngle;
    };

    struct __attribute__ ((__packed__)) SummonSpawnRequestMessage {
        SummonMessageHeader header;
        int buddyGoodsId;
    };

    struct __attribute__ ((__packed__)) SummonDespawnMessage {
        SummonMessageHeader header;
    };

    struct __attribute__ ((__packed__)) SummonDespawnRequestMessage {
        SummonMessageHeader header;
    };
}