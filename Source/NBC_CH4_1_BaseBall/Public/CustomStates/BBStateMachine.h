#pragma once

#include "CoreMinimal.h"
#include "StateMachines/StateMachine.h"
#include "BBStateMachine.generated.h"

class ABBGameMode;
class UState;

UCLASS()
class NBC_CH4_1_BASEBALL_API UBBStateMachine : public UStateMachine
{
	GENERATED_BODY()
public:
	static UBBStateMachine* Create(ABBGameMode* InOwnerGameMode);
	
	virtual void BuildStateTransitionMap() override;
	
	bool CheckWaitToIngameTransition(UState* From, UState* To);	
	
	void OnPostLogin(AController* NewPlayer);
};
