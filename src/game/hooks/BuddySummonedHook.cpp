#include "game/hooks/BuddySummonedHook.h"

namespace EROnlineSummons {
    BuddySummonedHook* BuddySummonedHook::_instance = nullptr;

    BuddySummonedHook::BuddySummonedHook(
        uintptr_t target,
        SummonBuddyStateMachine *stateMachine,
        SummonBuddyStateFactory *stateFactory,
        SummonNetworking *summonNetworking
    ) : JumpHook(target, (uintptr_t) onInvoke) {
        _instance = this;
        _stateMachine = stateMachine;
        _stateFactory = stateFactory;
        _summonNetworking = summonNetworking;
    }

    // TODO: keep track of who initiated summons some other way as this is abusing registers that aren't ours (isLocalInvocation)
    bool BuddySummonedHook::onInvoke(uintptr_t summonBuddyManager, int buddyGoodsId) {
        #ifndef NDEBUG
        Logging::WriteLine("Invoked BuddySummonedHook: %i", buddyGoodsId);
        #endif

        switch (_instance->_stateMachine->GetState()) {
            case (int) SummonBuddyStateMachine::State::SUMMON_SPAWNED:
                _instance->_stateMachine->TransitionTo(_instance->_stateFactory->CreateNoSummonState());
                return false;
            case (int) SummonBuddyStateMachine::State::NO_SUMMONS:
                if (_instance->_summonNetworking->ShouldNetwork() && !_instance->_summonNetworking->HasAuthority()) {
                    _instance->_stateMachine->TransitionTo(_instance->_stateFactory->CreateSummonRequestedState(buddyGoodsId));
                } else {
                    _instance->_stateMachine->TransitionTo(_instance->_stateFactory->CreateSummonSpawnedState(buddyGoodsId));
                }
                return true;
        }

        #ifndef NDEBUG
        Logging::WriteLine("Did not handle transition from state");
        #endif
        throw "Did not handle transition from state";
    }
}
