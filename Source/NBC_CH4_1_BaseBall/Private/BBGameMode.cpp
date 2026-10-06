#include "BBGameMode.h"

#include "BBGameState.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "BBWidget.h"
#include "GlobalConst.h"
#include "JUtility.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

FJudgeAnswerResult::FJudgeAnswerResult()
	:StrikeCount(0)
	, BallCount(0)
{
}

void ABBGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateRandomNumbers();
}

void ABBGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(NewPlayer);
	JASSERT(BBPlayerController != nullptr, "NewPlayer is not a BBPlayerController!");
	
	int32 PlayerControllerCount = UGameplayStatics::GetNumPlayerControllers(GetWorld());
	
	ABBPlayerState* BBPlayerState = BBPlayerController->GetPlayerState<ABBPlayerState>();
	JASSERT(BBPlayerState != nullptr, "BBPlayerState is not valid!");
	
	BBPlayerState->SetIngameName(FString::Printf(TEXT("Player_%d"), PlayerControllerCount));
	
	JServerLog("Player %d has joined the game.", PlayerControllerCount);
}

void ABBGameMode::OnChatCommitted(const FString& InputString, AController* PlayerController)
{
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
		ABBPlayerState* PlayerState = PlayerController->GetPlayerState<ABBPlayerState>();
		JASSERT(IsValid(PlayerState), "PlayerState is not valid!");
		
		PlayerState->DecreaseRemainGuessCount();
		
		TempPlayerAnswer.Empty();
		for (TCHAR ch : TrimedInput)
		{
			int32 Digit = ch - '0';
			TempPlayerAnswer.Add(Digit);
		}
		
		FJudgeAnswerResult JudgeResult = JudgeAnswer(TempPlayerAnswer);		
		ABBGameState* CastedGameState = GetGameState<ABBGameState>();
		JASSERT(CastedGameState != nullptr, "GameState is not valid!");
		
		FString JudgeMessage;
		if (JudgeResult.StrikeCount == 0 && JudgeResult.BallCount == 0)
		{
			JServerLog("%s Out", *PlayerState->GetPlayerName());
		}
		else
		{
			JServerLog("[%s]:[%s][%dS %dB]" 
			    , *PlayerState->GetPlayerName()
			    , *TrimedInput
			    , JudgeResult.StrikeCount
			    , JudgeResult.BallCount);
		}
	}
	else
	{
		ABBPlayerState* PlayerState = PlayerController->GetPlayerState<ABBPlayerState>();
		JASSERT(IsValid(PlayerState), "PlayerState is not valid!");
		
		FString ChatMessage = FString::Printf(TEXT("%s: %s"), *PlayerState->GetPlayerName(), *TrimedInput);
		ABBGameState* BBGameState = GetWorld()->GetGameState<ABBGameState>();
		JASSERT(BBGameState != nullptr, "BBGameState is not valid!");
		
		BBGameState->MultiCast_AddChatMessage(ChatMessage);		
	}
}

void ABBGameMode::GenerateRandomNumbers()
{
	Answer.Empty();
	
	for (int i = 0; i<GlobalConst::ANSWER_LENGTH; i++)	
	{
		int32 RandomNumber = FMath::RandRange(0, 9);
		if (Answer.Contains(RandomNumber))
		{
			i--;
			continue;
		}
		else
		{
			Answer.Add(RandomNumber);
		}
	}
	
	FString AnswerString;
	for (int i = 0; i<GlobalConst::ANSWER_LENGTH; i++)
	{
		AnswerString += FString::FromInt(Answer[i]);
	}
	
	JServerLog("Answer is %s", *AnswerString);
}

FJudgeAnswerResult ABBGameMode::JudgeAnswer(TArray<int32>& PlayerAnswer)
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


