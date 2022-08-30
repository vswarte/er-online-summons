#include "statemachine/SummonBuddyStateMachine.h"

namespace EROnlineSummons {
    SummonBuddyStateMachine::SummonBuddyStateMachine(SummonBuddyState *initialState) {
        _state = initialState;
    }

    void SummonBuddyStateMachine::TransitionTo(SummonBuddyState *newState) {
        auto currentState = _state->GetStateEnum();
        auto validTransition = newState->ValidatePreviousState(currentState);
        if (!validTransition) {
            Logging::WriteLine("Got invalid transition: %i -> %i", currentState, newState->GetStateEnum());
            return;
        }

        _state->Exit();
        _state = newState;
        _state->Enter();
    }

    SummonBuddyStateMachine::State SummonBuddyStateMachine::GetState() {
        return _state->GetStateEnum();
    }

    bool SummonBuddyState::stateEquals(SummonBuddyStateMachine::State expected, SummonBuddyStateMachine::State actual) {
        return expected == actual;
    }
}
