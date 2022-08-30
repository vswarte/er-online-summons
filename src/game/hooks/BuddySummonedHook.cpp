#include "game/hooks/BuddySummonedHook.h"

namespace EROnlineSummons {
    BuddySummonedHook* BuddySummonedHook::_instance = nullptr;

    BuddySummonedHook::BuddySummonedHook(uintptr_t target, SummonNetworking *summonNetworking)
        : JumpHook(target, (uintptr_t) onInvoke) {
        _instance = this;
        _summonNetworking = summonNetworking;
    }

    bool BuddySummonedHook::onInvoke(uintptr_t worldChrMan, int buddyGoodsId, bool local) {
        #ifndef NDEBUG
        Logging::WriteLine("Invoked BuddySummonedHook: %i", buddyGoodsId);
        #endif

        uintptr_t (*originalFunction)(...);
        *(uintptr_t*)&originalFunction = _instance->GetOriginal();
        auto result = originalFunction(worldChrMan, buddyGoodsId);

        // TODO: keep track of who initiated summons some other way as this is abusing registers that aren't ours
        if (local) {
            #ifndef NDEBUG
            Logging::WriteLine("Invocation was local");
            #endif
            _instance->_summonNetworking->SendSummonSpawned(buddyGoodsId);
        } else {
            #ifndef NDEBUG
            Logging::WriteLine("Invocation was remote");
            #endif
        }

        return result;
    }
}
