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
	
	void OnPostLogin(AController* NewPlayer);
	
	bool CheckWaitToIngameTransition(UState* From, UState* To);	
	bool CheckIngameToFinishedTransition(UState* From, UState* To);
	bool CheckFinishedToIngameTransition(UState* From, UState* To);
};