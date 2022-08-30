#pragma once

#include <thread>
#include "core/Memory.h"
#include "game/Offsets.h"
#include "networking/SummonNetworking.h"
#include "core/SteamInterfaceProvider.h"
#include "game/hooks/BuddySummonedHook.h"
#include "game/hooks/BuddyGoodsStateHook.h"
#include "statemachine/SummonBuddyStateMachine.h"
#include "statemachine/SummonBuddyStateFactory.h"

namespace EROnlineSummons {
    class OnlineSummons {
    public:
        OnlineSummons(
            SummonBuddyStateMachine *stateMachine,
            SummonBuddyStateFactory *stateFactory,
            SummonNetworking *summonNetworking
        );

        void Start();
        void Stop();

    private:
        SummonBuddyStateMachine *_stateMachine = nullptr;
        SummonBuddyStateFactory *_stateFactory = nullptr;
        SummonNetworking *_summonNetworking = nullptr;

        void thread();
        void awaitStop();
        std::thread _mainThread;
        volatile bool _stopRequested = false;
        volatile bool _threadStopped = false;

        void handleMessage(std::vector<char> message);
        void handleSummonRequestedMessage(SummonRequestedMessage *message);
        void handleSummonSpawnedMessage(SummonSpawnedMessage *message);

        void setupHooks();
        BuddySummonedHook *_buddySummonedHook = nullptr;
        BuddyGoodsStateHook *_buddyGoodsStateHook = nullptr;
    };
}
