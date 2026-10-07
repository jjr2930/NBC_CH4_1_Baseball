#include "CustomStates/BBStateMachine.h"
#include "CustomStates/WaitPlayerState.h"
#include "CustomStates/IngameState.h"
#include "StateMachines/Transition.h"
#include "BBGameMode.h"
#include "GlobalConst.h"
#include "JUtility.h"

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
	
	AddState(WaitPlayerState);
	AddState(IngameState);
	AddTransition(WaitPlayerState
		, FTransitionCheckingDelegate::CreateUObject(this, &UBBStateMachine::CheckWaitToIngameTransition)
		, IngameState);
	
	TransitionMap.Empty();
	for (UTransition* Transition : Transitions)
	{
		TransitionMap.Add(Transition->GetFromState(), Transition);
	}
	
	this->StartState = WaitPlayerState;
}

bool UBBStateMachine::CheckWaitToIngameTransition(UState* From, UState* To)
{
	ABBGameMode* GameMode = GetOwner<ABBGameMode>();
	if (GameMode->GetPlayerControllerCount() == GlobalConst::MAX_PLAYER_COUNT)
	{
		JServerLog("Entered player count == MAX_PLAYER_COUNT(%d), transition to IngameState."
			, GlobalConst::MAX_PLAYER_COUNT);
		
		GameMode->BroadcastAnnounceMessage(TEXT("All players have joined. The game is starting!"));
		return true;
	}
	else
	{
		return false;
	}
}

void UBBStateMachine::OnPostLogin(AController* NewPlayer)
{
	JASSERT(IsValid(CurrentState), "NewPlayer is not valid!");
	
	UBBStateBase* CastedState = GetCurrentState<UBBStateBase>();
	CastedState->OnPostLogin(NewPlayer);
}

UBBStateMachine* UBBStateMachine::Create(ABBGameMode* InOwnerGameMode)
{
	UBBStateMachine* NewStateMachine = NewObject<UBBStateMachine>(InOwnerGameMode, TEXT("BBStateMachine"));
	NewStateMachine->SetOwner(InOwnerGameMode);
	NewStateMachine->BuildStateTransitionMap();
	
	return NewStateMachine;
}
