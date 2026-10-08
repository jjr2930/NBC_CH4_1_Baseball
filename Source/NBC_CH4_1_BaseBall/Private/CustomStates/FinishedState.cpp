#include "CustomStates/FinishedState.h"
#include "BBGameMode.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "JUtility.h"

UFinishedState::UFinishedState()
{
	StateDuration = 5.0f;
	bIsFinished = false;
}

void UFinishedState::OnEnter()
{
	Super::OnEnter();
	
	StartTime = OwnerGameMode->GetWorld()->TimeSeconds;
	
	switch (OwnerGameMode->GetRunningState())
	{
		case ABBGameMode::ERunningState::SomeoneWin:
		{
			ABBPlayerController* Winner = OwnerGameMode->GetWinner();
			ABBPlayerState* WinnerPlayerState = Winner->GetPlayerState<ABBPlayerState>();
			FString Message = FString::Printf(TEXT("System: The winner is [%s]")
				, *WinnerPlayerState->GetPlayerName());
			
			OwnerGameMode->BroadcastAnnounceMessage(Message);
			break;
		}
		
		case ABBGameMode::ERunningState::Draw:
		{
			FString Message = FString::Printf(TEXT("System: The game is a draw!"));
			
			OwnerGameMode->BroadcastAnnounceMessage(Message);
			break;
		}
		case ABBGameMode::ERunningState::Playing:
			JError("DO NOT ENTER THIS CASE!");
			break;
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		 TimerHandle
		,this,
		&UFinishedState::BroadcastReadyForNextGame
		,StateDuration/2.f
	);
	
	bIsFinished = false;
}

void UFinishedState::OnTick(float DeltaSeconds)
{
	Super::OnTick(DeltaSeconds);
	
	float Now = OwnerGameMode->GetWorld()->TimeSeconds;
	if (Now - StartTime >= StateDuration)
	{
		bIsFinished = true;
	}
}

void UFinishedState::BroadcastReadyForNextGame()
{
	OwnerGameMode->BroadcastAnnounceMessage(TEXT("System: Ready for the next game!"));
}

bool UFinishedState::IsFinished() const
{
	return bIsFinished;
}
