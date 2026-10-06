// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStates/WaitPlayerState.h"

#include "BBGameMode.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "JUtility.h"
#include "Kismet/GameplayStatics.h"

void UWaitPlayerState::OnEnter()
{
	Super::OnEnter();
	
	JServerLog("UWaitPlayerState::OnEnter");
}

void UWaitPlayerState::PostLogin(AController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(NewPlayer);
	JASSERT(BBPlayerController != nullptr, "NewPlayer is not a BBPlayerController!");
	
	int32 PlayerControllerCount = UGameplayStatics::GetNumPlayerControllers(GetWorld());
	
	ABBPlayerState* BBPlayerState = BBPlayerController->GetPlayerState<ABBPlayerState>();
	JASSERT(BBPlayerState != nullptr, "BBPlayerState is not valid!");
	
	BBPlayerState->SetIngameName(FString::Printf(TEXT("Player_%d"), PlayerControllerCount));
	OwnerGameMode->AddPlayerController(BBPlayerController);
	
	OwnerGameMode->BroadcastAnnounceMessage(FString::Printf(TEXT("Player %s has joined the game!"), *BBPlayerState->GetPlayerName()));
}

void UWaitPlayerState::OnPlayerMessageCommitted(const FString& InputString, AController* Sender)
{
	Super::OnPlayerMessageCommitted(InputString, Sender);
}
