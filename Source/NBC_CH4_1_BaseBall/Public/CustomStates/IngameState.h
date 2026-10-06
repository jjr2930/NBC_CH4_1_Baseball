#pragma once

#include "CoreMinimal.h"
#include "BBGameMode.h"
#include "BBStateBase.h"
#include "IngameState.generated.h"

class NBC_CH4_1_BASEBALL_API FJudgeAnswerResult 
{
public:
	FJudgeAnswerResult();
	
public:
	int32 StrikeCount;
	int32 BallCount;
};

UCLASS()
class NBC_CH4_1_BASEBALL_API UIngameState : public UBBStateBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPlayerMessageCommitted(const FString& InputString, AController* Sender) override;
	
	virtual void OnEnter() override;
protected:	
	FJudgeAnswerResult JudgeAnswer(TArray<int32>& PlayerAnswer);
	
protected:
	TArray<int32> TempPlayerAnswer;
	TArray<int32> Answer;
};
