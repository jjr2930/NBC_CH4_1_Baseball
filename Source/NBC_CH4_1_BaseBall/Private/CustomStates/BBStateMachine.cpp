#include "CustomStates/BBStateMachine.h"
#include "CustomStates/WaitPlayerState.h"
#include "CustomStates/IngameState.h"
#include "StateMachines/Transition.h"
#include "BBGameMode.h"
#include "GlobalConst.h"

void UBBStateMachine::BuildStateTransitionMap()
{	
	UWaitPlayerState* WaitPlayerState = NewObject<UWaitPlayerState>(this);
	WaitPlayerState->SetParentMachine(this);	
	WaitPlayerState->SetOwnerGameMode(GetOwner<ABBGameMode>());
	WaitPlayerState->SetDisplayName(TEXT("WaitPlayerState"));
	
	UIngameState* IngameState = NewObject<UIngameState>(this);
	IngameState->SetParentMachine(this);
	IngameState->SetOwnerGameMode(GetOwner<ABBGameMode>());
	IngameState->SetDisplayName(TEXT("IngameState"));
	
	UTransition* WaitToIngameTransition = UTransition::Create(this
		, WaitPlayerState
		, IngameState
		, FTransitionCheckingDelegate::CreateUObject(this, &UBBStateMachine::CheckWaitToIngameTransition));
		
	AddState(WaitPlayerState);
	AddState(IngameState);
	AddTransition(WaitPlayerState
		, FTransitionCheckingDelegate::CreateUObject(this, &UBBStateMachine::CheckWaitToIngameTransition)
		, IngameState);
	
	this->StartState = WaitPlayerState;
}

bool UBBStateMachine::CheckWaitToIngameTransition(UState* From, UState* To)
{
	return GetOwner<ABBGameMode>()->GetPlayerControllerCount() == GlobalConst::MAX_PLAYER_COUNT;
}

UBBStateMachine* UBBStateMachine::Create(ABBGameMode* InOwnerGameMode)
{
	UBBStateMachine* NewStateMachine = NewObject<UBBStateMachine>(InOwnerGameMode, TEXT("BBStateMachine"));
	NewStateMachine->SetOwner(InOwnerGameMode);
	NewStateMachine->BuildStateTransitionMap();
	
	return NewStateMachine;
}
