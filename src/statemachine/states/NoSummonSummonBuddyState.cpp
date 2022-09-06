#include "statemachine/states/NoSummonSummonBuddyState.h"

namespace EROnlineSummons {
    SummonBuddyStateMachine::State NoSummonSummonBuddyState::GetStateEnum() {
        return SummonBuddyStateMachine::NO_SUMMONS;
    }

    bool NoSummonSummonBuddyState::ValidatePreviousState(SummonBuddyStateMachine::State previousState) {
        return stateEquals(SummonBuddyStateMachine::SUMMON_SPAWNED, previousState)
            || stateEquals(SummonBuddyStateMachine::SUMMON_DESPAWN_REQUESTED, previousState);
    }

    void NoSummonSummonBuddyState::Enter() {
        Logging::WriteLine("Entering No Summons State");
    }

    void NoSummonSummonBuddyState::Exit() {
        Logging::WriteLine("Exiting No Summons State");
    }
}
