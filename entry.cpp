#include "entry.h"

BOOL WINAPI DllMain(HINSTANCE baseAddress, DWORD reason, BOOL isStatic) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
        attach();
        break;
        case DLL_PROCESS_DETACH:
            detach();
        break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        default:
        break;
    }
    return true;
}

void attach() {
    EROnlineSummons::Logging::Init();

    if (MH_Initialize() != MH_OK) {
        EROnlineSummons::Logging::WriteLine("MinHook initialization failed");
    }

    // TODO: delete these on cleanup
    auto summonBuddyManager = new EROnlineSummons::SummonBuddyManager();
    auto steamInterfaceProvider = new EROnlineSummons::SteamInterfaceProvider();
    steamInterfaceProvider->CreateInterfacesWhenReady();

    onlineSummons = new EROnlineSummons::OnlineSummons(
        new EROnlineSummons::SummonNetworking(steamInterfaceProvider->GetNetworkingMessages(), summonBuddyManager)
    );
    onlineSummons->Start();
}

void detach() {
    onlineSummons->Stop();
    EROnlineSummons::Logging::WriteLine("Online Summons stopped");

    EROnlineSummons::Logging::WriteLine("Deiniting logging");
    EROnlineSummons::Logging::Deinit();
}