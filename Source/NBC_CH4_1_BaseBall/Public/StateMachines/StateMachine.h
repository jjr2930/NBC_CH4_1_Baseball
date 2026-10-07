#pragma once

#include "CoreMinimal.h"
#include "DefineStateMachineDelegates.h"
#include "StateMachine.generated.h"


class UState;
class UTransition;

template<typename T>
concept CState = std::derived_from<T, UState>;

template<typename T>
concept CActor = std::derived_from<T, AActor>;

UCLASS()
class NBC_CH4_1_BASEBALL_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
    virtual void BuildStateTransitionMap() PURE_VIRTUAL(UStateMachine::BuildStateTransitionMap, return; );
    
    void OnEnter();
    void OnTick(float DeltaTime);
    void OnFinish();

    void AddState(TObjectPtr<UState> InNewState);
    void AddTransition(TObjectPtr<UState> InFromState, FTransitionCheckingDelegate InDelegate, TObjectPtr<UState> InToState);
    
    void SetOwner(AActor* InOwner);
    AActor* GetOwner();
    template<CActor T>
    T* GetOwner();
    
    UState* GetCurrentState();
    
    template<CState T>
    T* GetCurrentState();
    
protected:
    void ChangeState(TObjectPtr<UState> InNextState);

protected:
    UPROPERTY()
    TArray<TObjectPtr<UState>> States;
    
    UPROPERTY()
    TArray<TObjectPtr<UTransition>> Transitions;
    
    TMultiMap<TObjectPtr<UState>, TObjectPtr<UTransition>> TransitionMap;
    
    UPROPERTY()
    TObjectPtr<UState> StartState;
    
    UPROPERTY()
    TObjectPtr<UState> CurrentState;
    
    UPROPERTY()
    TObjectPtr<AActor> Owner;
};

template <CActor T>
T* UStateMachine::GetOwner()
{
    return Cast<T>(Owner);
}

template <CState T>
T* UStateMachine::GetCurrentState()
{
    return Cast<T>(CurrentState);
}
