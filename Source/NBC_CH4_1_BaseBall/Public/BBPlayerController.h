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
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void ServerRpcOnChatCommitted(const FString& InputString);
	
	void AddPrintChattingMessage(const FString& Message);
	void SetAnnounceMessage(const FString& NewAnnounceMessage);
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Baseball===|Properties");
	TSubclassOf<UUserWidget> IngameWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UBBWidget> IngameWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Baseball===|Properties");	
	FString PlayerName;
};
