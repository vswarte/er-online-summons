#pragma once

#include "core/Memory.h"
#include "core/Logging.h"
#include "game/SessionManager.h"
#include "game/SummonBuddyManager.h"
#include "core/SteamInterfaceProvider.h"
#include "networking/SummonNetworkingMessages.h"

#define ER_ONLINE_SUMMONS_STEAM_MESSAGE_CHANNEL 6386536 // = 61 73 68 = ash

namespace EROnlineSummons {
    class SummonNetworking {
    public:
        SummonNetworking(ISteamNetworkingMessages *steamNetworkingMessages, SummonBuddyManager *summonBuddyManager);

        void SendSummonSpawned(int buddyGoodsId);
        void SendRequestSummonSpawn(int buddyGoodsId);

        void ReadSummonEvents();

    private:
        STEAM_CALLBACK(SummonNetworking, onMessageSessionRequest, SteamNetworkingMessagesSessionRequest_t);

        ISteamNetworkingMessages *_steamNetworkingMessages = nullptr;
        void broadcastBuffer(char *payload, int size);
        void sendBuffer(uint64_t steamId, char *payload, int size);

        SummonBuddyManager *_summonBuddyManager = nullptr;
    };
}