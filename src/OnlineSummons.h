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

        // TODO: refactor this to some message handler construction
        void handleMessage(std::vector<char> message);
        void handleSummonSpawnRequestedMessage(SummonSpawnRequestMessage *message);
        void handleSummonSpawnedMessage(SummonSpawnMessage *message);
        void handleSummonDespawnRequestedMessage(SummonDespawnRequestMessage *message);
        void handleSummonDespawnedMessage(SummonDespawnMessage *message);

        void setupHooks();
        BuddySummonedHook *_buddySummonedHook = nullptr;
        BuddyGoodsStateHook *_buddyGoodsStateHook = nullptr;
    };
}
