#include "hooks/BuddySummonedHook.h"

namespace EROnlineSummons {
    BuddySummonedHook* BuddySummonedHook::_instance = nullptr;

    BuddySummonedHook::BuddySummonedHook(uintptr_t target, SummonNetworking *summonNetworking)
        : JumpHook(target, (uintptr_t) onInvoke) {
        _instance = this;
        _summonNetworking = summonNetworking;
    }

    bool BuddySummonedHook::onInvoke(uintptr_t worldChrMan, int buddyGoodsId, bool local) {
        uintptr_t (*originalFunction)(...);
        *(uintptr_t*)&originalFunction = _instance->GetOriginal();
        auto result = originalFunction(worldChrMan, buddyGoodsId);

        // TODO: keep track of who initiated summons some other way as this is abusing registers that aren't ours
        if (local) {
            Logging::WriteLine("Invocation was local");
            _instance->_summonNetworking->SendSummonSpawned(buddyGoodsId);
        } else {
            Logging::WriteLine("Invocation was remote");
        }

        return result;
    }
}
