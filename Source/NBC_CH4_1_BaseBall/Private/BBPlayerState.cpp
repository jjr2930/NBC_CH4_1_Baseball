// Fill out your copyright notice in the Description page of Project Settings.


#include "BBPlayerState.h"
#include "GlobalConst.h"
#include "Net/UnrealNetwork.h"

ABBPlayerState::ABBPlayerState()
{
	RemainGuessCount = GlobalConst::TOTAL_ANSWER_COUNT;
	
}

void ABBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, RemainGuessCount);
	DOREPLIFETIME(ThisClass, PlayerName);
}

void ABBPlayerState::DecreaseRemainGuessCount()
{
	RemainGuessCount--;
}

int ABBPlayerState::GetRemainGuessCount()
{
	return RemainGuessCount;
}

FString& ABBPlayerState::GetPlayerName()
{
	return PlayerName;
}
