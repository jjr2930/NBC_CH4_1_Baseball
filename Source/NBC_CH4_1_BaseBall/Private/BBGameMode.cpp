#include "BBGameMode.h"

#include "BBGameState.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "GlobalConst.h"
#include "JUtility.h"
#include "CustomStates/BBStateBase.h"
#include "CustomStates/BBStateMachine.h"
#include "Kismet/GameplayStatics.h"


ABBGameMode::ABBGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABBGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	StateMachine = UBBStateMachine::Create(this);
	StateMachine->OnEnter();
}

void ABBGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	UBBStateBase* CurrentState = StateMachine->GetCurrentState<UBBStateBase>();
	CurrentState->PostLogin(NewPlayer);
}

void ABBGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	StateMachine->OnTick(DeltaSeconds);
}

void ABBGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{	
	StateMachine->OnFinish();
	
	Super::EndPlay(EndPlayReason);	
}

void ABBGameMode::OnChatCommitted(const FString& InputString, AController* PlayerController)
{
	UBBStateBase* CurrentState = StateMachine->GetCurrentState<UBBStateBase>();
	
	CurrentState->OnPlayerMessageCommitted(InputString, PlayerController);
}

void ABBGameMode::BroadcastAnnounceMessage(const FString& Message)
{
	ABBGameState* BBGameState = GetWorld()->GetGameState<ABBGameState>();
	JASSERT(IsValid(BBGameState), "BBGameState is not valid!");
	
	BBGameState->Multicast_SetAnnounceMessage(Message);
}

void ABBGameMode::BroadcastChatMessage(const FString& Message)
{
	ABBGameState* BBGameState = GetWorld()->GetGameState<ABBGameState>();
	JASSERT(IsValid(BBGameState), "BBGameState is not valid!");
	
	BBGameState->MultiCast_AddChatMessage(Message);
}

void ABBGameMode::AddPlayerController(ABBPlayerController* NewPlayerController)
{
	PlayerControllers.Add(NewPlayerController);
}

int32 ABBGameMode::GetPlayerControllerCount() const
{
	return PlayerControllers.Num();
}
