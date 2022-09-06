#pragma once

#include "core/logging.h"

namespace EROnlineSummons {
    class SummonBuddyState;

    class SummonBuddyStateMachine {
    public:
        enum State {
            NO_SUMMONS,
            SUMMON_SPAWN_REQUESTED,
            SUMMON_SPAWNED,
            SUMMON_DESPAWN_REQUESTED,
        };

        SummonBuddyStateMachine(SummonBuddyState *initialState);

        State GetState();
        void TransitionTo(SummonBuddyState *newState);

    private:
        SummonBuddyState *_state = nullptr;
    };

    class SummonBuddyState {
    public:
        virtual SummonBuddyStateMachine::State GetStateEnum();

        virtual bool ValidatePreviousState(SummonBuddyStateMachine::State previousState);
        virtual void Enter();
        virtual void Exit();

    protected:
        bool stateEquals(SummonBuddyStateMachine::State expected, SummonBuddyStateMachine::State actual);
    };
}
