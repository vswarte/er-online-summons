#include "game/hooks/BuddyGoodsStateHook.h"

namespace EROnlineSummons {
    BuddyGoodsStateHook* BuddyGoodsStateHook::_instance = nullptr;

    BuddyGoodsStateHook::BuddyGoodsStateHook(uintptr_t target, SummonBuddyStateMachine *stateMachine)
        : JumpHook(target, (uintptr_t) onInvoke) {
        _instance = this;
        _stateMachine = stateMachine;
    }

    BuddyGoodsStateHook::BuddyGoodsStates BuddyGoodsStateHook::onInvoke(uintptr_t summonBuddyManager, int buddyGoodsId) {
        return _instance->_stateMachine->GetState() == SummonBuddyStateMachine::NO_SUMMONS ? Usable : AlreadySpawned;
    }
}
