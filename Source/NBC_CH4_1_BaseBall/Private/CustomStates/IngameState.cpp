// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStates/IngameState.h"

#include "BBGameMode.h"
#include "BBGameState.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "GlobalConst.h"
#include "JUtility.h"

FJudgeAnswerResult::FJudgeAnswerResult()
	: StrikeCount(0)
	, BallCount(0)
{
}

bool FJudgeAnswerResult::Is3Strike()
{
	return StrikeCount == GlobalConst::ANSWER_LENGTH;
}

bool FJudgeAnswerResult::IsZero()
{
	return StrikeCount == 0 && BallCount == 0;
}

void UIngameState::OnEnter()
{
	Super::OnEnter();

	JServerLog("UIngameState::OnEnter()");
	
	Answer.Empty();
	for (int32 i = 0; i < GlobalConst::ANSWER_LENGTH; i++)
	{
		int32 RnadomNumber = FMath::RandRange(0, 9);
		while (Answer.Contains(RnadomNumber))
		{
			RnadomNumber = FMath::RandRange(0, 9);
		}
		Answer.Add(RnadomNumber);
	}
	
	OwnerGameMode->ResetCurrentTurnPlayer();
}

void UIngameState::OnPlayerMessageCommitted(const FString& InputString, AController* Sender)
{
	Super::OnPlayerMessageCommitted(InputString, Sender);
	
	ABBPlayerController* CastedSender = Cast<ABBPlayerController>(Sender);
	ABBPlayerController* CurrentTurnPlayer = GetCurrentTurnPlayer();
	
	//it must be run at release build so not use JASSERT;
	if (InputString.IsEmpty())
	{
		JServerLog("InputString is empty.");
		return;
	}
	
	FString TrimedInput = InputString.TrimStartAndEnd();
	bool bIsAnswerInput = TrimedInput.IsNumeric() && TrimedInput.Len() == GlobalConst::ANSWER_LENGTH;
	if (bIsAnswerInput)
	{
		if (CastedSender != CurrentTurnPlayer)
		{
			JServerLog("[%s] is not current turn player. Current turn player is [%s]. Sending message to [%s] to wait for their turn."
				, *CastedSender->GetPlayerState<ABBPlayerState>()->GetPlayerName()
				, *CurrentTurnPlayer->GetPlayerState<ABBPlayerState>()->GetPlayerName()
				, *CastedSender->GetPlayerState<ABBPlayerState>()->GetPlayerName());
		
			FString Message = FString::Printf(TEXT("System: not your turn. Wait your turn."));
			CastedSender->ClientRpcSetAnnounceMessage(Message);
			return;
		}
		
		ABBPlayerState* PlayerState = Sender->GetPlayerState<ABBPlayerState>();
		JASSERT(IsValid(PlayerState), "PlayerState is not valid!");
		
		TempPlayerAnswer.Empty();
		for (TCHAR ch : TrimedInput)
		{
			int32 Digit = ch - '0';
			if (TempPlayerAnswer.Contains(Digit))
			{
				FString Message = FString::Printf(TEXT("%s:[%s] Invalid input! Duplicate digit found.")
					, *PlayerState->GetPlayerName()
					, *TrimedInput);
				
				JServerLog("%s", *Message);
				CastedSender->ClientRpcSetAnnounceMessage(Message);
				return;
			}
			
			TempPlayerAnswer.Add(Digit);
		}
		
		PlayerState->DecreaseRemainGuessCount();
		
		FJudgeAnswerResult JudgeResult = JudgeAnswer(TempPlayerAnswer);		

		FString JudgeMessage;
		if (JudgeResult.IsZero())
		{
			FString Message = FString::Printf(TEXT("[%s]:[%s] OUT!")
				, *PlayerState->GetPlayerName()
				, *TrimedInput);
			
			JServerLog("%s", *Message);
			OwnerGameMode->BroadcastChatMessage(Message);
		}
		else if (JudgeResult.Is3Strike())
		{
			FString Message = FString::Printf(TEXT("%s:[%s] Correct! You win!")
				, *PlayerState->GetPlayerName()
				, *TrimedInput);
			
			JServerLog("%s", *Message);
			OwnerGameMode->BroadcastChatMessage(Message);
			
			FString AnnounceMessage = FString::Printf(TEXT("System: [%s] has won the game!"), *PlayerState->GetPlayerName());
			OwnerGameMode->BroadcastAnnounceMessage(AnnounceMessage);
		}
		else
		{
			FString Message = FString::Printf(TEXT("%s:[%s][%dS %dB]")
				, *PlayerState->GetPlayerName()
				, *TrimedInput
				, JudgeResult.StrikeCount
				, JudgeResult.BallCount);
			
			JServerLog("%s", *Message);	
			OwnerGameMode->BroadcastChatMessage(Message);
		}
		
		CurrentTurnPlayerIndex = (CurrentTurnPlayerIndex + 1) % OwnerGameMode->GetPlayerControllerCount();
		OwnerGameMode->SetCurrentTurnPlayer(CurrentTurnPlayerIndex);
	}
	else
	{
		ABBPlayerState* PlayerState = Sender->GetPlayerState<ABBPlayerState>();
		JASSERT(IsValid(PlayerState), "PlayerState is not valid!");
		
		FString ChatMessage = FString::Printf(TEXT("%s: %s"), *PlayerState->GetPlayerName(), *TrimedInput);
		ABBGameState* BBGameState = GetWorld()->GetGameState<ABBGameState>();
		JASSERT(BBGameState != nullptr, "BBGameState is not valid!");
		
		BBGameState->MultiCast_AddChatMessage(ChatMessage);		
	}
}

FJudgeAnswerResult UIngameState::JudgeAnswer(TArray<int32>& PlayerAnswer)
{
	FJudgeAnswerResult Result;

	for (int32 i = 0; i<GlobalConst::ANSWER_LENGTH; i++)
	{
		if (PlayerAnswer[i] == Answer[i])
		{
			Result.StrikeCount++;
		}
		else if (Answer.Contains(PlayerAnswer[i]))
		{
			Result.BallCount++;
		}
	}	

	return Result;
}

ABBPlayerController* UIngameState::GetCurrentTurnPlayer() const
{
	return OwnerGameMode->GetPlayerControllerByIndex(CurrentTurnPlayerIndex);
}
