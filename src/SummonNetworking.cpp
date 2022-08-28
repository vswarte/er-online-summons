#include "SummonNetworking.h"

namespace EROnlineSummons {
    SummonNetworking::SummonNetworking(ISteamNetworkingMessages *steamNetworkingMessages, SummonBuddyManager *summonBuddyManager) {
        if (steamNetworkingMessages == nullptr) {
            throw std::invalid_argument("steamNetworkingMessages");
        }

        _steamNetworkingMessages = steamNetworkingMessages;
        _summonBuddyManager = summonBuddyManager;
    }

    void SummonNetworking::SendSummonSpawned(int buddyGoodsId) {
        Logging::WriteLine("Sending SummonSpawned message (size: %i)", sizeof(SummonSpawnedMessage));

        auto summonSpawnedMessage = SummonSpawnedMessage();
        summonSpawnedMessage.header.type = SummonNetworkMessageType::SummonSpawned;
        summonSpawnedMessage.buddyGoodsId = buddyGoodsId;

        // TODO: Probably very cursed and a health hazard, but works for testing I hope
        broadcastBuffer((char *) &summonSpawnedMessage, sizeof(SummonSpawnedMessage));
    }

    void SummonNetworking::ReadSummonEvents() {
        SteamNetworkingMessage_t *receivedMessages[32];
        auto receivedMessageCount = _steamNetworkingMessages->ReceiveMessagesOnChannel(
            ER_ONLINE_SUMMONS_STEAM_MESSAGE_CHANNEL,
            receivedMessages,
            32
        );

        for (auto i = 0; i < receivedMessageCount; i++) {
            Logging::WriteLine("Received a message of %i bytes", receivedMessages[i]->GetSize());

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

    void SummonNetworking::sendBuffer(uint64_t steamId, char *payload, int size) {
        Logging::WriteLine("Attempting to send buffer to %llu", steamId);

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

    void SummonNetworking::onMessageSessionRequest(SteamNetworkingMessagesSessionRequest_t *messageSessionRequest) {
        Logging::WriteLine(
            "Got message session request from %i",
            messageSessionRequest->m_identityRemote.GetSteamID()
        );

        _steamNetworkingMessages->AcceptSessionWithUser(messageSessionRequest->m_identityRemote);
    }
}
