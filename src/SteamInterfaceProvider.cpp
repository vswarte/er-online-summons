#include "SteamInterfaceProvider.h"

namespace EROnlineSummons {
    void SteamInterfaceProvider::CreateInterfacesWhenReady() {
        // TODO: put a max retry on this
        while (!isReadyForInterfaceCreation()) {
            Sleep(1);
        }

        _networkingMessages = SteamNetworkingMessages();
        _ready = true;
    }

    bool SteamInterfaceProvider::isReadyForInterfaceCreation() {
        return SteamAPI_GetHSteamUser() && SteamAPI_GetHSteamPipe();
    }

    void SteamInterfaceProvider::EnsureReady() {
        if (!_ready) {
            // TODO: implement proper exception type for this
            throw "SteamInterfaceProvider is not _ready";
        }
    }

    ISteamNetworkingMessages *SteamInterfaceProvider::GetNetworkingMessages() {
        EnsureReady();
        return _networkingMessages;
    }
}
