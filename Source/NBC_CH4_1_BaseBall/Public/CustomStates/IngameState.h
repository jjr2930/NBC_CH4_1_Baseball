#pragma once

#include "CoreMinimal.h"
#include "BBGameMode.h"
#include "BBStateBase.h"
#include "IngameState.generated.h"

class NBC_CH4_1_BASEBALL_API FJudgeAnswerResult 
{
public:
	FJudgeAnswerResult();
	bool Is3Strike();
	bool IsZero();
public:
	int32 StrikeCount;
	int32 BallCount;
};

UCLASS()
class NBC_CH4_1_BASEBALL_API UIngameState : public UBBStateBase
{
	GENERATED_BODY()
	
public:
	UIngameState();
	
	virtual void OnEnter() override;
	virtual void OnPlayerMessageCommitted(const FString& InputString, AController* Sender) override;
	
	void ResetGame();
	bool IsEveryPlayerUsedAllGuessCount() const;
	
	ABBPlayerController* GetCurrentTurnPlayer() const;
	void SetCurrentTurnPlayerToNextPlayer();
	void ResetCurrentTurnPlayer();
protected:	
	FJudgeAnswerResult JudgeAnswer(TArray<int32>& PlayerAnswer);

protected:	
	TArray<int32> TempPlayerAnswer;
	TArray<int32> Answer;
	int32 CurrentTurnPlayerIndex;
	
	FString AnswerResoponseFormat;
	FString ChatMessageFormat;
	FString OutMessageFormat;
	FString CorrectMessageFormat;	
};
