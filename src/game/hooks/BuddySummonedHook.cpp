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

    bool BuddySummonedHook::onInvoke(uintptr_t summonBuddyManager, int buddyGoodsId) {
        #ifndef NDEBUG
        Logging::WriteLine("Invoked BuddySummonedHook: %i", buddyGoodsId);
        #endif

        switch (_instance->_stateMachine->GetState()) {
            case SummonBuddyStateMachine::State::SUMMON_SPAWNED:
                if (_instance->_summonNetworking->ShouldNetwork() && !_instance->_summonNetworking->HasAuthority()) {
                    _instance->_stateMachine->TransitionTo(
                        _instance->_stateFactory->CreateSummonDespawnRequestedState()
                    );
                } else {
                    _instance->_stateMachine->TransitionTo(
                        _instance->_stateFactory->CreateNoSummonState()
                    );
                }
                return false;

            case SummonBuddyStateMachine::State::NO_SUMMONS:
                if (_instance->_summonNetworking->ShouldNetwork() && !_instance->_summonNetworking->HasAuthority()) {
                    _instance->_stateMachine->TransitionTo(
                        _instance->_stateFactory->CreateSummonSpawnRequestedState(buddyGoodsId)
                    );
                } else {
                    _instance->_stateMachine->TransitionTo(
                        _instance->_stateFactory->CreateSummonSpawnedState(buddyGoodsId)
                    );
                }
                return true;

            case SummonBuddyStateMachine::State::SUMMON_SPAWN_REQUESTED:
            case SummonBuddyStateMachine::State::SUMMON_DESPAWN_REQUESTED:
                return false;
        }

        #ifndef NDEBUG
        Logging::WriteLine("Did not handle transition from state");
        #endif
        throw "Did not handle transition from state";
    }
}
