#pragma once

#include <windows.h>
#include <exception>
#include "steam/steam_api.h"

namespace EROnlineSummons {
    class SteamInterfaceProvider {
    public:
        void CreateInterfacesWhenReady();
        ISteamNetworkingMessages *GetNetworkingMessages();

    private:
        bool _ready = false;
        bool isReadyForInterfaceCreation();
        void EnsureReady();

        ISteamNetworkingMessages *_networkingMessages = nullptr;
    };
}
