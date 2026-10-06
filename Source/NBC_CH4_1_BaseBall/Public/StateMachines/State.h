#pragma once

#include "CoreMinimal.h"
#include "State.generated.h"

class UStateMachine;

UCLASS()
class NBC_CH4_1_BASEBALL_API UState : public UObject
{
    GENERATED_BODY()

public:    
    virtual void OnEnter();
    virtual void OnTick(float DeltaSeconds);
    virtual void OnExit();
    
    void SetParentMachine(UStateMachine* InParentMachine);
    UStateMachine* GetParentMachine();

    void SetDisplayName(const FString& DisplayName);
    const FString& GetDisplayName() const;
    
protected:
    FString DisplayName;
    UStateMachine* ParentMachine;
};
