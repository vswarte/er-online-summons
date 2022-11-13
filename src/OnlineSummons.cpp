#include "OnlineSummons.h"

namespace EROnlineSummons {
    OnlineSummons::OnlineSummons(
        SummonBuddyStateMachine *stateMachine,
        SummonBuddyStateFactory *stateFactory,
        SummonNetworking *summonNetworking
    ) {
        _stateMachine = stateMachine;
        _stateFactory = stateFactory;
        _summonNetworking = summonNetworking;
    }

    void OnlineSummons::Start() {
        setupHooks();
        _mainThread = std::thread{&OnlineSummons::thread, this};
    }

    void OnlineSummons::Stop() {
        // TODO: implement some error about stopping the mod twice (probably detach while shutdown is already happening)
        _stopRequested = true;

        awaitStop();

        _summonCleanupConditionCodePatch->Uninstall();
        _buddySummonedHook->Uninstall();
        _buddyGoodsStateHook->Uninstall();
        delete _summonCleanupConditionCodePatch;
        delete _buddySummonedHook;
        delete _buddyGoodsStateHook;
    }

    void OnlineSummons::thread() {
        while (!_stopRequested) {
            auto messages = _summonNetworking->RetrieveMessages();

            for (const auto& message : messages) {
                handleMessage(message);
            }

            // TODO: free up message memory
        }

        _threadStopped = true;
    }

    // TODO: move this garbage into its own unit
    void OnlineSummons::handleMessage(std::vector<char> message) {
        auto data = message.data();
        auto header = (SummonMessageHeader *) data;

        #ifndef NDEBUG
        Logging::WriteLine("Handling message (type: %i, size: %i)", header->type, message.size());
        #endif

        switch (header->type) {
            case SummonNetworkMessageType::SummonSpawnRequest:
                return handleSummonSpawnRequestedMessage((SummonSpawnRequestMessage *) data);
            case SummonNetworkMessageType::SummonSpawn:
                return handleSummonSpawnedMessage((SummonSpawnMessage *) data);
            case SummonNetworkMessageType::SummonDespawnRequest:
                return handleSummonDespawnRequestedMessage((SummonDespawnRequestMessage *) data);
            case SummonNetworkMessageType::SummonDespawn:
                return handleSummonDespawnedMessage((SummonDespawnMessage *) data);
        }
    }

    void OnlineSummons::awaitStop() {
        // TODO: figure out why _mainThread.join() doesn't work here
        while (!_threadStopped) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void OnlineSummons::handleSummonSpawnRequestedMessage(SummonSpawnRequestMessage *message) {
        if (!_summonNetworking->HasAuthority()) {
            Logging::WriteLine("Got summon spawn request while not having authority");
            return;
        }
        _stateMachine->TransitionTo(_stateFactory->CreateSummonSpawnedState(message->buddyGoodsId));
    }

    void OnlineSummons::handleSummonSpawnedMessage(SummonSpawnMessage *message) {
        // TODO: pass through steam ID for message to validate against host
        _stateMachine->TransitionTo(
            _stateFactory->CreateSummonSpawnedState(message->buddyGoodsId, new SummonBuddySpawnOrigin {
                x: message->spawnOriginX,
                y: message->spawnOriginY,
                z: message->spawnOriginZ,
                unk0xc: message->unk0xc,
                angle: message->spawnAngle,
            })
        );
    }

    void OnlineSummons::handleSummonDespawnRequestedMessage(SummonDespawnRequestMessage *message) {
        if (!_summonNetworking->HasAuthority()) {
            Logging::WriteLine("Got summon request while not having authority");
            return;
        }
        _stateMachine->TransitionTo(_stateFactory->CreateNoSummonState());
    }

    void OnlineSummons::handleSummonDespawnedMessage(SummonDespawnMessage *message) {
        // TODO: pass through steam ID for message to validate against host
        _stateMachine->TransitionTo(_stateFactory->CreateNoSummonState());
    }

    // TODO: get some AOBs going for these
    void OnlineSummons::setupHooks() {
        _buddySummonedHook = new BuddySummonedHook(
             GetBaseAddress() + OFFSET_FN_USE_BUDDY_GOODS,
            _stateMachine,
            _stateFactory,
            _summonNetworking
        );
        if (!_buddySummonedHook->Install()) {
            Logging::WriteLine("Could not attach summon buddy hook");
        }

        _buddyGoodsStateHook = new BuddyGoodsStateHook(GetBaseAddress() + OFFSET_FN_BUDDY_GOODS_STATE, _stateMachine);
        if (!_buddyGoodsStateHook->Install()) {
            Logging::WriteLine("Could not attach buddy goods state hook");
        }

        _summonCleanupConditionCodePatch = new BuddyCleanupConditionCodePatch(GetBaseAddress() + OFFSET_FN_SUMMON_CLEANUP_CONDITION);
        _summonCleanupConditionCodePatch->Install();
        /**
        _buddyCleanupHook = new BuddyCleanupHook(GetBaseAddress() + OFFSET_FN_SUMMON_CLEANUP);
        if (!_buddyCleanupHook->Install()) {
            Logging::WriteLine("Could not attach buddy cleanup hook");
        }
        */
    }
}