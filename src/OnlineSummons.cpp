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

        _buddySummonedHook->Uninstall();
        _buddyGoodsStateHook->Uninstall();
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
            case SummonNetworkMessageType::SummonRequested:
                return handleSummonRequestedMessage((SummonRequestedMessage *) data);
            case SummonNetworkMessageType::SummonSpawned:
                return handleSummonSpawnedMessage((SummonSpawnedMessage *) data);
        }
    }

    void OnlineSummons::awaitStop() {
        // TODO: figure out why _mainThread.join() doesn't work here
        while (!_threadStopped) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void OnlineSummons::handleSummonRequestedMessage(SummonRequestedMessage *message) {
        _stateMachine->TransitionTo(_stateFactory->CreateSummonSpawnedState(message->buddyGoodsId));
    }

    void OnlineSummons::handleSummonSpawnedMessage(SummonSpawnedMessage *message) {
        _stateMachine->TransitionTo(_stateFactory->CreateSummonSpawnedState(message->buddyGoodsId));
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
    }

}