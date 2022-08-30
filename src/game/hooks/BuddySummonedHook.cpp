#include "game/hooks/BuddySummonedHook.h"

namespace EROnlineSummons {
    BuddySummonedHook* BuddySummonedHook::_instance = nullptr;

    BuddySummonedHook::BuddySummonedHook(uintptr_t target, SummonNetworking *summonNetworking)
        : JumpHook(target, (uintptr_t) onInvoke) {
        _instance = this;
        _summonNetworking = summonNetworking;
    }

    // TODO: keep track of who initiated summons some other way as this is abusing registers that aren't ours (isLocalInvocation)
    bool BuddySummonedHook::onInvoke(uintptr_t worldChrMan, int buddyGoodsId, bool isLocalInvocation) {
        #ifndef NDEBUG
        Logging::WriteLine("Invoked BuddySummonedHook: %i", buddyGoodsId);
        #endif

        // TODO: gotta find some way to bring the handling for this and that on the message reading together
        auto hasSummoned = invokeOriginal(worldChrMan, buddyGoodsId);
        if (SessionManager::IsInSession()) {
            auto isHost = SessionManager::IsHost();
            #ifndef NDEBUG
            Logging::WriteLine(
                "Invocation Local: %i, Is Host: %i, Has Summoned: %i",
                isLocalInvocation,
                isHost,
                hasSummoned
            );
            #endif

            if (hasSummoned) {
                handleSummonSpawn(buddyGoodsId, isHost, isLocalInvocation);
            } else {
                handleSummonDespawn(buddyGoodsId, isHost, isLocalInvocation);
            }
        }

        return hasSummoned;
    }

    bool BuddySummonedHook::invokeOriginal(uintptr_t worldChrMan, int buddyGoodsId) {
        uintptr_t (*originalFunction)(...);
        *(uintptr_t*)&originalFunction = _instance->GetOriginal();
        return originalFunction(worldChrMan, buddyGoodsId);
    }

    void BuddySummonedHook::handleSummonSpawn(int buddyGoodsId, bool isHost, bool isLocalInvocation) {
        if (isHost) {
            _instance->_summonNetworking->SendSummonSpawned(buddyGoodsId);
        } else if (isLocalInvocation && !isHost) {
            _instance->_summonNetworking->SendRequestSummonSpawn(buddyGoodsId);
        }
    }

    void BuddySummonedHook::handleSummonDespawn(int buddyGoodsId, bool isHost, bool isLocalInvocation) {
        #ifndef NDEBUG
        Logging::WriteLine("Should despawn summons");
        #endif
    }
}
