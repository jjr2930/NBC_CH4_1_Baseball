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

UIngameState::UIngameState()
{
	//PlayerName, InputNumber, StrikeCount, BallCount, RemainGuessCount, TotalGuessCount
	AnswerResoponseFormat = TEXT("{0}:{1} [{2}S {3}B] [{4}/{5}]");
	//PlayerName, Message
	ChatMessageFormat = TEXT("{0}: {1}");
	//PlayerName, Input Number, RemainGuessCount, TotalGuessCount
	OutMessageFormat = TEXT("{0}: {1} OUT! [{2}/{3}]");
	//PlayerName, Input Number
	CorrectMessageFormat = TEXT("{0}: {1} Correct!");
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
	
	//log answer
	FString AnswerString;
	for (int32 Num : Answer)
	{
		AnswerString += FString::FromInt(Num);
	}
	
	JServerLog("UIngameState::OnEnter() Answer: %s", *AnswerString);
	
	OwnerGameMode->BroadcastAnnounceMessage(TEXT(""));
	OwnerGameMode->BroadCastResetChatMessage();
	
	OwnerGameMode->ResetCurrentTurnPlayer();
	OwnerGameMode->SetRunningState(ABBGameMode::ERunningState::Playing);
}

void UIngameState::OnPlayerMessageCommitted(const FString& InputString, AController* Sender)
{
	Super::OnPlayerMessageCommitted(InputString, Sender);
	
	ABBPlayerController* CastedSender = Cast<ABBPlayerController>(Sender);
	ABBPlayerController* CurrentTurnPlayer = GetCurrentTurnPlayer();
	ABBGameState* BBGameState = GetWorld()->GetGameState<ABBGameState>();
	JASSERT(BBGameState != nullptr, "BBGameState is not valid!");
	
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
			FString Message = FString::Format(*OutMessageFormat, 
		{
				PlayerState->GetPlayerName()
				,TrimedInput
				,PlayerState->GetRemainGuessCount()
				,GlobalConst::TOTAL_GUESS_COUNT
			});
			
			JServerLog("%s", *Message);
			OwnerGameMode->BroadcastChatMessage(Message);
		}
		else if (JudgeResult.Is3Strike())
		{
			FString Message = FString::Format(*CorrectMessageFormat, 
		{
				PlayerState->GetPlayerName()
				, TrimedInput
			});
			
			JServerLog("%s", *Message);
			OwnerGameMode->BroadcastChatMessage(Message);
			
			OwnerGameMode->SetRunningState(ABBGameMode::ERunningState::SomeoneWin);
			OwnerGameMode->SetWinner(CastedSender);
		}
		else
		{
			FString Message = FString::Format(*AnswerResoponseFormat, 
		{
				PlayerState->GetPlayerName()
				,TrimedInput
				,JudgeResult.StrikeCount
				,JudgeResult.BallCount
				,PlayerState->GetRemainGuessCount()
				,GlobalConst::TOTAL_GUESS_COUNT
			});
			
			JServerLog("%s", *Message);	
			OwnerGameMode->BroadcastChatMessage(Message);
		}
		
		if (IsEveryPlayerUsedAllGuessCount())
		{
			OwnerGameMode->SetRunningState(ABBGameMode::ERunningState::Draw);
			
			FString DrawMessage = FString::Printf(TEXT("System: The game is a draw!"));
		
			BBGameState->Multicast_SetAnnounceMessage(DrawMessage);
		}
		else
		{
			CurrentTurnPlayerIndex = (CurrentTurnPlayerIndex + 1) % OwnerGameMode->GetPlayerControllerCount();
			OwnerGameMode->SetCurrentTurnPlayer(CurrentTurnPlayerIndex);
		}		
	}
	else
	{
		ABBPlayerState* PlayerState = Sender->GetPlayerState<ABBPlayerState>();
		JASSERT(IsValid(PlayerState), "PlayerState is not valid!");
		
		FString ChatMessage = FString::Format(*ChatMessageFormat, 
		{
			PlayerState->GetPlayerName()
			,TrimedInput
		});
		
		BBGameState->MultiCast_AddChatMessage(ChatMessage);		
	}	
}

bool UIngameState::IsEveryPlayerUsedAllGuessCount() const
{
	int32 ControllerCounut = OwnerGameMode->GetPlayerControllerCount();
	for (int32 i = 0; i < ControllerCounut; i++)
	{
		ABBPlayerController* PlayerController = OwnerGameMode->GetPlayerControllerByIndex(i);
		if (PlayerController->GetPlayerState<ABBPlayerState>()->GetRemainGuessCount() > 0)
		{
			return false;
		}
	}
	return true;
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
