// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBGameState.generated.h"

/**
 * 
 */
UCLASS()
class NBC_CH4_1_BASEBALL_API ABBGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_AddChatMessage(const FString& NewMessage);
	
protected:
	FString LastMessageReceived;
};
