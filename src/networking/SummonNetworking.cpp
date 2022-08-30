#include "networking/SummonNetworking.h"

namespace EROnlineSummons {
    SummonNetworking::SummonNetworking(ISteamNetworkingMessages *steamNetworkingMessages, SummonBuddyManager *summonBuddyManager) {
        if (steamNetworkingMessages == nullptr) {
            throw std::invalid_argument("steamNetworkingMessages");
        }

        if (summonBuddyManager == nullptr) {
            throw std::invalid_argument("summonBuddyManager");
        }

        _steamNetworkingMessages = steamNetworkingMessages;
        _summonBuddyManager = summonBuddyManager;
    }

    void SummonNetworking::SendSummonSpawned(int buddyGoodsId) {
        #ifndef NDEBUG
        Logging::WriteLine("Sending SummonSpawned message (size: %i)", sizeof(SummonSpawnedMessage));
        #endif

        auto message = SummonSpawnedMessage();
        message.header.type = SummonNetworkMessageType::SummonSpawned;
        message.buddyGoodsId = buddyGoodsId;

        broadcastBuffer((char *) &message, sizeof(SummonSpawnedMessage));
    }

    void SummonNetworking::SendRequestSummonSpawn(int buddyGoodsId) {
        #ifndef NDEBUG
        Logging::WriteLine("Sending RequestSummonSpawn message (size: %i)", sizeof(SummonSpawnedMessage));
        #endif

        auto message = RequestSummonSpawnMessage();
        message.header.type = SummonNetworkMessageType::RequestSummonSpawn;
        message.buddyGoodsId = buddyGoodsId;

        broadcastBuffer((char *) &message, sizeof(RequestSummonSpawnMessage));
    }

    // TODO: refactor handling into a reactor pattern
    void SummonNetworking::ReadSummonEvents() {
        SteamNetworkingMessage_t *receivedMessages[32];
        auto receivedMessageCount = _steamNetworkingMessages->ReceiveMessagesOnChannel(
            ER_ONLINE_SUMMONS_STEAM_MESSAGE_CHANNEL,
            receivedMessages,
            32
        );

        for (auto i = 0; i < receivedMessageCount; i++) {
            #ifndef NDEBUG
            Logging::WriteLine("Received a message of %i bytes", receivedMessages[i]->GetSize());
            #endif

            auto summonMessage = (SummonSpawnedMessage *) receivedMessages[i]->GetData();
            _summonBuddyManager->SpawnSummons(summonMessage->buddyGoodsId);

            // TODO: implement some garbage to demux whatever is going on and handle the payloads
            receivedMessages[i]->Release();
        }
    }

    void SummonNetworking::broadcastBuffer(char *payload, int size) {
        for (auto steamId : SessionManager::GetPartyMemberSteamIdsForBroadcast()) {
            sendBuffer(steamId, payload, size);
        }
    }

    // TODO: set the messaging flags to reliable
    void SummonNetworking::sendBuffer(uint64_t steamId, char *payload, int size) {
        #ifndef NDEBUG
        Logging::WriteLine("Attempting to send buffer to %llu", steamId);
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

        if (result != EResult::k_EResultOK) {
            Logging::WriteLine("Could not send buffer result: %i", result);
        }
    }

    // TODO: validate against players in the steam lobby
    void SummonNetworking::onMessageSessionRequest(SteamNetworkingMessagesSessionRequest_t *messageSessionRequest) {
        _steamNetworkingMessages->AcceptSessionWithUser(messageSessionRequest->m_identityRemote);
    }
}
