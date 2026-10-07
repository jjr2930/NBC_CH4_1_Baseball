#pragma once

#include "CoreMinimal.h"
#include "StateMachines/State.h"
#include "BBStateBase.generated.h"

class ABBGameMode;

UCLASS()
class NBC_CH4_1_BASEBALL_API UBBStateBase : public UState
{
	GENERATED_BODY()
	
public:
	void SetOwnerGameMode(ABBGameMode* InOwnerGameMode);	
	virtual void OnPostLogin(AController* NewPlayer);
	virtual void OnPlayerMessageCommitted(const FString& InputString, AController* Sender);
	
protected:
	void SendChatMessage(const FString& NewMessage);
	void SendAnnounceMessage(const FString& NewAnnounceMessage);
	
protected:
	UPROPERTY()
	TObjectPtr<ABBGameMode> OwnerGameMode;
};
