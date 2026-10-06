#include "BBPlayerController.h"

#include "BBGameMode.h"
#include "GlobalConst.h"
#include "JUtility.h"
#include "BBWidget.h"
#include "Blueprint/UserWidget.h"
#include "BBGameState.h"
#include "BBPlayerState.h"

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
	}
}


void ABBPlayerController::AddPrintChattingMessage(const FString& Message)
{	
	JASSERT(IsValid(IngameWidgetInstance), "IngameWidgetInstance is not valid!");
	
	IngameWidgetInstance->AddChatHistory(Message);
}

void ABBPlayerController::SetAnnounceMessage(const FString& NewAnnounceMessage)
{
	JASSERT( IsValid(IngameWidgetInstance), "IngameWidgetInstance is not valid!");

	IngameWidgetInstance->SetAnnounceText(NewAnnounceMessage);
}


void ABBPlayerController::ServerRpcOnChatCommitted_Implementation(const FString& InputString)
{
	 ABBGameMode* GameMode = GetWorld()->GetAuthGameMode<ABBGameMode>();
	 JASSERT(GameMode != nullptr, "GameMode is not valid!");
	
	 GameMode->OnChatCommitted(InputString, this);
}
