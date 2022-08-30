#pragma once

#include <windows.h>
#include <exception>
#include "steam/steam_api.h"

namespace EROnlineSummons {
    class SteamInterfaceProvider {
    public:
        void CreateInterfacesWhenReady();
        ISteamNetworkingMessages *GetNetworkingMessages();
        ISteamUser *GetUser();

    private:
        bool _ready = false;
        bool isReadyForInterfaceCreation();
        void EnsureReady();

        ISteamUser *_user = nullptr;
        ISteamNetworkingMessages *_networkingMessages = nullptr;
    };
}
