// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStates/WaitPlayerState.h"

#include "BBGameMode.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "GlobalConst.h"
#include "JUtility.h"
#include "Kismet/GameplayStatics.h"

void UWaitPlayerState::OnEnter()
{
	Super::OnEnter();
	
	JServerLog("UWaitPlayerState::OnEnter");
}

void UWaitPlayerState::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(NewPlayer);
	JASSERT(BBPlayerController != nullptr, "NewPlayer is not a BBPlayerController!");
	
	int32 PlayerControllerCount = UGameplayStatics::GetNumPlayerControllers(GetWorld());
	
	ABBPlayerState* BBPlayerState = BBPlayerController->GetPlayerState<ABBPlayerState>();
	JASSERT(BBPlayerState != nullptr, "BBPlayerState is not valid!");
	
	BBPlayerState->SetIngameName(FString::Printf(TEXT("Player_%d"), PlayerControllerCount));
	OwnerGameMode->AddPlayerController(BBPlayerController);
	
	FString Message = FString::Printf(TEXT("Player %s has joined the game!")
		, *BBPlayerController->GetPlayerState<ABBPlayerState>()->GetPlayerName());
		
	OwnerGameMode->BroadcastAnnounceMessage(Message);
}

void UWaitPlayerState::OnPlayerMessageCommitted(const FString& InputString, AController* Sender)
{
	Super::OnPlayerMessageCommitted(InputString, Sender);
}

bool UWaitPlayerState::IsPlayerQueueEmpty() const
{
	return NewPlayerQueue.IsEmpty();
}
