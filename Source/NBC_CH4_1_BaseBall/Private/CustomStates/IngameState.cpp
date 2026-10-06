// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStates/IngameState.h"

#include "BBGameMode.h"
#include "BBGameState.h"
#include "BBPlayerState.h"
#include "GlobalConst.h"
#include "JUtility.h"

FJudgeAnswerResult::FJudgeAnswerResult()
	: StrikeCount(0)
	, BallCount(0)
{
}

void UIngameState::OnPlayerMessageCommitted(const FString& InputString, AController* Sender)
{
	Super::OnPlayerMessageCommitted(InputString, Sender);
	
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
		ABBPlayerState* PlayerState = Sender->GetPlayerState<ABBPlayerState>();
		JASSERT(IsValid(PlayerState), "PlayerState is not valid!");
		
		PlayerState->DecreaseRemainGuessCount();
		
		TempPlayerAnswer.Empty();
		for (TCHAR ch : TrimedInput)
		{
			int32 Digit = ch - '0';
			TempPlayerAnswer.Add(Digit);
		}
		
		FJudgeAnswerResult JudgeResult = JudgeAnswer(TempPlayerAnswer);		

		FString JudgeMessage;
		if (JudgeResult.StrikeCount == 0 && JudgeResult.BallCount == 0)
		{
			
			FString Message = FString::Printf(TEXT("[%s]:[%s] OUT!")
				, *PlayerState->GetPlayerName()
				, *TrimedInput);
			
			JServerLog("%s", *Message);
			OwnerGameMode->BroadcastChatMessage(Message);
		}
		else
		{
			FString Message = FString::Printf(TEXT("[%s]:[%s][%dS %dB]")
				, *PlayerState->GetPlayerName()
				, *TrimedInput
				, JudgeResult.StrikeCount
				, JudgeResult.BallCount);
			
			JServerLog("%s", *Message);	
			OwnerGameMode->BroadcastChatMessage(Message);
		}
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
