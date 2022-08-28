#include "OnlineSummons.h"

namespace EROnlineSummons {
    OnlineSummons::OnlineSummons(SummonNetworking *summonNetworking) {
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

        buddySummonedHook->Uninstall();
        delete buddySummonedHook;
    }

    void OnlineSummons::thread() {
        while (!_stopRequested) {
            _summonNetworking->ReadSummonEvents();
        }

        _threadStopped = true;
    }

    void OnlineSummons::awaitStop() {
        // TODO: figure out why _mainThread.join() doesn't work here
        while (!_threadStopped) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // TODO: get some AOBs going for these
    void OnlineSummons::setupHooks() {
        auto buddySummonedHookPtr = GetBaseAddress() + OFFSET_FN_USE_BUDDY_GOODS;
        buddySummonedHook = new BuddySummonedHook(buddySummonedHookPtr, _summonNetworking);
        if (!buddySummonedHook->Install()) {
            Logging::WriteLine("Could not attach summon buddy hook at %p", buddySummonedHookPtr);
        }
    }
}