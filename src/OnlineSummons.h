#pragma once

#include <thread>
#include "core/Memory.h"
#include "SummonNetworking.h"
#include "SteamInterfaceProvider.h"
#include "hooks/BuddySummonedHook.h"

namespace EROnlineSummons {
    class OnlineSummons {
    public:
        OnlineSummons(SummonNetworking *summonNetworking);

        void Start();
        void Stop();

    private:
        SummonNetworking *_summonNetworking = nullptr;

        void thread();
        void awaitStop();
        std::thread _mainThread;
        volatile bool _stopRequested = false;
        volatile bool _threadStopped = false;

        void setupHooks();
        EROnlineSummons::BuddySummonedHook *buddySummonedHook = nullptr;
    };
}
