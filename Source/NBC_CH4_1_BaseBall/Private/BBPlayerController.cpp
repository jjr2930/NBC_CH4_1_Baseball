#include "BBPlayerController.h"

#include "BBGameMode.h"
#include "GlobalConst.h"
#include "JUtility.h"
#include "BBWidget.h"
#include "Blueprint/UserWidget.h"
#include "BBGameState.h"
#include "BBPlayerState.h"

ABBPlayerController::ABBPlayerController()
	: bIsInit(false)
{
}

void ABBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		FInputModeGameAndUI InputMode;
	
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
	
		IngameWidgetInstance =  Cast<UBBWidget>(CreateWidget(this, IngameWidgetClass));
		JASSERT(IsValid(IngameWidgetInstance), "IngameWidgetInstance is not valid!");
	
		IngameWidgetInstance->AddToViewport(0);
		
		bIsInit = true;
	}
}


void ABBPlayerController::ClientRpcSetAnnounceMessage_Implementation(const FString& NewAnnounceMessage)
{
	if (!IsLocalController())
	{
		return;
	}
	
	//아직 초기화가 안된 경우
	if (!bIsInit)
	{
		return;
	}
	
	JASSERT(IsValid(IngameWidgetInstance), "IngameWidgetInstance is not valid!");
	
	IngameWidgetInstance->SetAnnounceText(NewAnnounceMessage);
}

void ABBPlayerController::AddPrintChattingMessage(const FString& Message)
{	
	if (!IsLocalController())
	{
		return;
	}
	
	if (!bIsInit)
	{
		return;
	}
	
	JASSERT(IsValid(IngameWidgetInstance), "IngameWidgetInstance is not valid!");
	
	IngameWidgetInstance->AddChatHistory(Message);
}

void ABBPlayerController::SetAnnounceMessage(const FString& NewAnnounceMessage)
{
	if (!IsLocalController())
	{
		return;
	}
	
	if (!bIsInit)
	{
		return;
	}
	
	JASSERT( IsValid(IngameWidgetInstance), "IngameWidgetInstance is not valid!");

	IngameWidgetInstance->SetAnnounceText(NewAnnounceMessage);
}

void ABBPlayerController::ResetChatMessage()
{
	if (!bIsInit)
	{
		return;
	}
	
	JASSERT(IsValid(IngameWidgetInstance), "IngameWidgetInstance is not valid!");
	
	IngameWidgetInstance->ResetChatHistory();
}


void ABBPlayerController::ServerRpcOnChatCommitted_Implementation(const FString& InputString)
{
	 ABBGameMode* GameMode = GetWorld()->GetAuthGameMode<ABBGameMode>();
	 JASSERT(GameMode != nullptr, "GameMode is not valid!");
	
	 GameMode->OnChatCommitted(InputString, this);
}
