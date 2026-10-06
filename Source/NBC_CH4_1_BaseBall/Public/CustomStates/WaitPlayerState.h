#pragma once

#include "CoreMinimal.h"
#include "CustomStates/BBStateBase.h"
#include "WaitPlayerState.generated.h"

UCLASS()
class NBC_CH4_1_BASEBALL_API UWaitPlayerState : public UBBStateBase
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter() override;
	
	virtual void PostLogin(AController* NewPlayer) override;
	virtual void OnPlayerMessageCommitted(const FString& InputString, AController* Sender) override;
	
};
