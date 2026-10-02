#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameMode.generated.h"

class UBBWidget;

class NBC_CH4_1_BASEBALL_API FJudgeAnswerResult 
{
public:
	int32 StrikeCount;
	int32 BallCount;
};

UCLASS()
class NBC_CH4_1_BASEBALL_API ABBGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
protected:
	void GenerateRandomNumbers();
	FJudgeAnswerResult JudgeAnswer(TArray<int32>& PlayerAnswer);	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Baseball===|Properties");
	TArray<int32> Answer;
	
};
