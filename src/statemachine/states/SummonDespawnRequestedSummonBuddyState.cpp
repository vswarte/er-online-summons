#include "statemachine/states/SummonDespawnRequestedSummonBuddyState.h"

namespace EROnlineSummons {
    SummonDespawnRequestedSummonBuddyState::SummonDespawnRequestedSummonBuddyState(SummonNetworking *summonNetworking) {
        _summonNetworking = summonNetworking;
    }

    SummonBuddyStateMachine::State SummonDespawnRequestedSummonBuddyState::GetStateEnum() {
        return SummonBuddyStateMachine::SUMMON_DESPAWN_REQUESTED;
    }

    bool SummonDespawnRequestedSummonBuddyState::ValidatePreviousState(SummonBuddyStateMachine::State previousState) {
        return stateEquals(SummonBuddyStateMachine::SUMMON_SPAWNED, previousState);
    }

    void SummonDespawnRequestedSummonBuddyState::Enter() {
        Logging::WriteLine("Entering Summon Despawn Requested State");
        _summonNetworking->SendSummonDespawnRequest();
    }

    void SummonDespawnRequestedSummonBuddyState::Exit() {
        Logging::WriteLine("Exiting Summon Despawn Requested State");
    }
}
