#include "BBGameMode.h"

#include "BBPlayerController.h"
#include "BBWidget.h"
#include "GlobalConst.h"
#include "JUtility.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

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
	

	BBPlayerController->SetPlayerName(FString::Printf(TEXT("Player_%d"), PlayerControllerCount));
	
	JServerLog("Player %d has joined the game.", PlayerControllerCount);
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


