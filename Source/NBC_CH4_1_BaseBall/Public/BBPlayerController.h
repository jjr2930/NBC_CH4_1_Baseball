// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"

class UUserWidget;
class UBBWidget;

UCLASS()
class NBC_CH4_1_BASEBALL_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABBPlayerController();
	
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void ServerRpcOnChatCommitted(const FString& InputString);
	
	UFUNCTION(Client, Reliable)
	void ClientRpcSetAnnounceMessage(const FString& NewAnnounceMessage);
	
	void AddPrintChattingMessage(const FString& Message);
	void SetAnnounceMessage(const FString& NewAnnounceMessage);
	void ResetChatMessage();
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Baseball===|Properties");
	TSubclassOf<UUserWidget> IngameWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UBBWidget> IngameWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Baseball===|Properties");	
	FString PlayerName;
	
	bool bIsInit;
};
