#include "networking/SummonNetworking.h"

namespace EROnlineSummons {
    SummonNetworking::SummonNetworking(ISteamNetworkingMessages *steamNetworkingMessages) {
        if (steamNetworkingMessages == nullptr) {
            throw std::invalid_argument("steamNetworkingMessages");
        }

        _steamNetworkingMessages = steamNetworkingMessages;
    }

    void SummonNetworking::SendSummonSpawn(int buddyGoodsId, SummonBuddySpawnOrigin *spawnOrigin) {
        auto message = SummonSpawnMessage();
        message.header.type = SummonNetworkMessageType::SummonSpawn;
        message.buddyGoodsId = buddyGoodsId;
        message.spawnOriginX = spawnOrigin->x;
        message.spawnOriginY = spawnOrigin->y;
        message.spawnOriginZ = spawnOrigin->z;
        message.unk0xc = spawnOrigin->unk0xc;
        message.spawnAngle = spawnOrigin->angle;

        #ifndef NDEBUG
        Logging::WriteLine(
            "Built SummonSpawnMessage (buddyGoodsId: %i, X: %f, Y: %f, Z: %f, Unk0xC: %f, Angle: %f)",
            message.buddyGoodsId,
            message.spawnOriginX,
            message.spawnOriginY,
            message.spawnOriginZ,
            message.unk0xc,
            message.spawnAngle
        );
        #endif

        broadcastBuffer((char *) &message, sizeof(SummonSpawnMessage));
    }

    void SummonNetworking::SendSummonSpawnRequest(int buddyGoodsId) {
        auto message = SummonSpawnRequestMessage();
        message.header.type = SummonNetworkMessageType::SummonSpawnRequest;
        message.buddyGoodsId = buddyGoodsId;

        sendBuffer(SessionManager::GetHostSteamId(), (char *) &message, sizeof(SummonSpawnRequestMessage));
    }

    void SummonNetworking::SendSummonDespawn() {
        auto message = SummonDespawnMessage();
        message.header.type = SummonNetworkMessageType::SummonDespawn;

        #ifndef NDEBUG
        Logging::WriteLine("Sending summon despawn message");
        #endif

        broadcastBuffer((char *) &message, sizeof(SummonDespawnMessage));
    }

    void SummonNetworking::SendSummonDespawnRequest() {
        auto message = SummonDespawnRequestMessage();
        message.header.type = SummonNetworkMessageType::SummonDespawnRequest;

        #ifndef NDEBUG
        Logging::WriteLine("Sending summon despawn request");
        #endif

        sendBuffer(SessionManager::GetHostSteamId(), (char *) &message, sizeof(SummonDespawnRequestMessage));
    }

    // TODO: refactor handling into a reactor pattern
    std::vector<std::vector<char>> SummonNetworking::RetrieveMessages() {
        SteamNetworkingMessage_t *receivedMessages[32];
        auto receivedMessageCount = _steamNetworkingMessages->ReceiveMessagesOnChannel(
            ER_ONLINE_SUMMONS_STEAM_MESSAGE_CHANNEL,
            receivedMessages,
            32
        );

        std::vector<std::vector<char>> result;
        for (auto i = 0; i < receivedMessageCount; i++) {
            #ifndef NDEBUG
            Logging::WriteLine("Received a message of %i bytes", receivedMessages[i]->GetSize());
            #endif

            // TODO: refactor the validation a bit
            auto messageData = receivedMessages[i]->GetData();
            auto header = (SummonMessageHeader *) messageData;
            auto messageLength = getMessageLength(header->type);
            if (messageLength != 0 && messageLength == receivedMessages[i]->GetSize()) {
                std::vector<char> messageBuffer(messageLength);
                memcpy(messageBuffer.data(), messageData, messageLength);
                result.emplace_back(messageBuffer);
            }

            receivedMessages[i]->Release();
        }

        return result;
    }

    int SummonNetworking::getMessageLength(SummonNetworkMessageType type) {
        switch (type) {
            case SummonNetworkMessageType::SummonSpawnRequest:
                return sizeof(SummonSpawnRequestMessage);
            case SummonNetworkMessageType::SummonSpawn:
                return sizeof(SummonSpawnMessage);
            case SummonNetworkMessageType::SummonDespawnRequest:
                return sizeof(SummonDespawnRequestMessage);
            case SummonNetworkMessageType::SummonDespawn:
                return sizeof(SummonDespawnMessage);
        }

        return 0;
    }

    void SummonNetworking::broadcastBuffer(char *payload, int size) {
        for (auto steamId : SessionManager::GetPartyMemberSteamIdsForBroadcast()) {
            sendBuffer(steamId, payload, size);
        }
    }

    // TODO: set the messaging flags to reliable
    void SummonNetworking::sendBuffer(uint64_t steamId, char *payload, int size) {
        #ifndef NDEBUG
        Logging::WriteLine(
            "Attempting to send buffer (type: %i, size: %i) to %llu",
            *(char*) payload,
            size,
            steamId
        );
        #endif

        auto networkIdentity = SteamNetworkingIdentity();
        networkIdentity.SetSteamID64(steamId);

        auto result = _steamNetworkingMessages->SendMessageToUser(
            networkIdentity,
            payload,
            size,
            0,
            ER_ONLINE_SUMMONS_STEAM_MESSAGE_CHANNEL
        );
        #ifndef NDEBUG
        Logging::WriteLine("Done sending buffer to %llu", steamId);
        #endif

        if (result != EResult::k_EResultOK) {
            Logging::WriteLine("Could not send buffer result: %i", result);
        }
    }

    // TODO: validate against players in the steam lobby
    void SummonNetworking::onMessageSessionRequest(SteamNetworkingMessagesSessionRequest_t *messageSessionRequest) {
        _steamNetworkingMessages->AcceptSessionWithUser(messageSessionRequest->m_identityRemote);
    }

    bool SummonNetworking::ShouldNetwork() {
        return SessionManager::IsInSession();
    }

    bool SummonNetworking::HasAuthority() {
        return SessionManager::IsHost();
    }
}
