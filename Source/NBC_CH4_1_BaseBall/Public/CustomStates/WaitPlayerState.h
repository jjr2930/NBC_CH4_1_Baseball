#pragma once

#include "CoreMinimal.h"
#include "CustomStates/BBStateBase.h"
#include "WaitPlayerState.generated.h"

class ABBPlayerController;

UCLASS()
class NBC_CH4_1_BASEBALL_API UWaitPlayerState : public UBBStateBase
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void OnPlayerMessageCommitted(const FString& InputString, AController* Sender) override;
	
	void SetFinish();
	bool IsFinished() const;
	
	void BroadcastReadyForNextGameIfNeed();
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===WaitPlayerState===|Properties")
	float ReadyWaitingTime = 2.5f;
	
	FTimerHandle TimerHandle;
	bool bIsFinished;
};
