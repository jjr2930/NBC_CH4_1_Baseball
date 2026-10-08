#include "CustomStates/BBStateMachine.h"
#include "CustomStates/WaitPlayerState.h"
#include "CustomStates/IngameState.h"
#include "StateMachines/Transition.h"
#include "BBGameMode.h"
#include "GlobalConst.h"
#include "JUtility.h"
#include "CustomStates/FinishedState.h"


UBBStateMachine::UBBStateMachine()
{
	UWaitPlayerState* WaitPlayerState = CreateDefaultSubobject<UWaitPlayerState>(TEXT("WaitPlayerState"));
	WaitPlayerState->SetParentMachine(this);	
	
	UIngameState* IngameState = CreateDefaultSubobject<UIngameState>( TEXT("IngameState"));
	IngameState->SetParentMachine(this);
	
	UFinishedState* FinishedState = CreateDefaultSubobject<UFinishedState>( TEXT("FinishedState"));
	FinishedState->SetParentMachine(this);
	
	AddState(WaitPlayerState);
	AddState(IngameState);
	AddState(FinishedState);
	
	AddTransition(WaitPlayerState
		, FTransitionCheckingDelegate::CreateUObject(this, &UBBStateMachine::CheckWaitToIngameTransition)
		, IngameState);
	
	AddTransition(IngameState
		, FTransitionCheckingDelegate::CreateUObject(this, &UBBStateMachine::CheckIngameToFinishedTransition)
		, FinishedState);
	
	AddTransition(FinishedState,
		FTransitionCheckingDelegate::CreateUObject(this, &UBBStateMachine::CheckFinishedToIngameTransition),
		IngameState);
	
	TransitionMap.Empty();
	for (UTransition* Transition : Transitions)
	{
		TransitionMap.Add(Transition->GetFromState(), Transition);
	}
	
	this->StartState = WaitPlayerState;
}

bool UBBStateMachine::CheckWaitToIngameTransition(UState* From, UState* To)
{
	ABBGameMode* GameMode = GetCastedOuter<ABBGameMode>();
	UWaitPlayerState* WaitPlayerState = Cast<UWaitPlayerState>(From);
	JASSERT_BOOL(IsValid(GameMode), "GameMode is not valid!");
	
	bool bIsAllPlayerJoined = GameMode->GetPlayerControllerCount() == GlobalConst::MAX_PLAYER_COUNT;
	bool bIsStateFinished = WaitPlayerState->IsFinished();
	if (  bIsAllPlayerJoined && bIsStateFinished)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UBBStateMachine::CheckIngameToFinishedTransition(UState* From, UState* To)
{
	ABBGameMode* GameMode = GetCastedOuter<ABBGameMode>();
	JASSERT_BOOL(IsValid(GameMode), "GameMode is not valid!");
	
	if (GameMode->GetRunningState() != ABBGameMode::ERunningState::Playing)
	{
		JServerLog("GameMode's running state is not Playing, transition to FinishedState.");
		return true;
	}
	else
	{
		return false;
	}
}

bool UBBStateMachine::CheckFinishedToIngameTransition(UState* From, UState* To)
{
	UFinishedState* FinishedState = Cast<UFinishedState>(From);
	JASSERT_BOOL(IsValid(FinishedState), "FinishedState is not valid!");
	
	return FinishedState->IsFinished();
}

void UBBStateMachine::OnPostLogin(AController* NewPlayer)
{
	JASSERT(IsValid(CurrentState), "NewPlayer is not valid!");
	
	UBBStateBase* CastedState = GetCurrentState<UBBStateBase>();
	CastedState->OnPostLogin(NewPlayer);
}