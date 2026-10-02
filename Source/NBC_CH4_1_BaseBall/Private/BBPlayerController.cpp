#include "BBPlayerController.h"

#include "GlobalConst.h"
#include "JUtility.h"
#include "BBWidget.h"
#include "Blueprint/UserWidget.h"
#include "BBGameState.h"


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

void ABBPlayerController::SetPlayerName(const FString& NewPlayerName)
{
	this->PlayerName = NewPlayerName;
}

FString ABBPlayerController::GetPlayerName()
{
	return this->PlayerName;
}

void ABBPlayerController::ServerRPC_OnTextCommitted_Implementation(const FString& InputString)
{
	if (!HasAuthority())
	{
		return;
	}
	
	
	//it must be run at release build so not use JASSERT;
	if (InputString.IsEmpty())
	{
		JServerLog("InputString is empty.");
		return;
	}
	
	FString TrimedInput = InputString.TrimStartAndEnd();
	bool bIsAnswerInput = TrimedInput.IsNumeric() && TrimedInput.Len() == GlobalConst::ANSWER_LENGTH;
	if (bIsAnswerInput)
	{
	}
	else
	{
		FString ChatMessage = FString::Printf(TEXT("%s: %s"), *PlayerName, *TrimedInput);
		GetWorld()->GetGameState<ABBGameState>()->MultiCast_AddChatMessage(ChatMessage);
	}
}

void ABBPlayerController::AddPrintChattingMessage(const FString& Message)
{			
	if (IsValid(IngameWidgetInstance))
	{
		IngameWidgetInstance->AddChatHistory(Message);
	}
}