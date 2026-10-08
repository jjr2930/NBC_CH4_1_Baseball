// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NBC_CH4_1_BASEBALL_API ABBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABBPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void DecreaseRemainGuessCount();
	int GetRemainGuessCount();
	void ResetRemainGuessCount();
	
	void SetIngameName(const FString& NewName);
	FString& GetPlayerName();
protected:
	UPROPERTY(Replicated);
	int RemainGuessCount;
	
	UPROPERTY(Replicated)
	FString IngameName;
};
