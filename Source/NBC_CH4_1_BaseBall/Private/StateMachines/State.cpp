#include "StateMachines/State.h"
#include "JUtility.h"


void UState::OnEnter()
{
    JLog("%s Entered", *DisplayName);
}

void UState::OnTick(float DeltaSeconds)
{
}

void UState::OnExit()
{
    JLog("%s Exit", *DisplayName);
}

void UState::SetParentMachine(UStateMachine* InParentMachine)
{
    ParentMachine = InParentMachine;
}

UStateMachine* UState::GetParentMachine()
{
    return ParentMachine;
}

const FString& UState::GetDisplayName() const
{
    return DisplayName;
}

void UState::SetDisplayName(const FString& NewDisplayName)
{
    DisplayName = NewDisplayName;
}