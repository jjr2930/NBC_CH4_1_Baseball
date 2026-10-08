#pragma once

#include "CoreMinimal.h"
#include "CustomStates/BBStateBase.h"
#include "FinishedState.generated.h"

UCLASS()
class NBC_CH4_1_BASEBALL_API UFinishedState : public UBBStateBase
{
	GENERATED_BODY()
	
public:
	UFinishedState();
	
	virtual void OnEnter() override;
	virtual void OnTick(float DeltaSeconds) override;
	
	void BroadcastReadyForNextGame();
	
	bool IsFinished() const;
protected:
	UPROPERTY()
	float StartTime;
	
	UPROPERTY()
	float StateDuration;
	
	bool bIsFinished;
	FTimerHandle TimerHandle;
};
