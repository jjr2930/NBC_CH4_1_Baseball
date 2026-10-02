#include "BBGameState.h"
#include "Kismet/GameplayStatics.h"
#include "BBPlayerController.h"
#include "JUtility.h"

void ABBGameState::MultiCast_AddChatMessage_Implementation(const FString& NewMessage)
{
	if (HasAuthority())
	{
		return;
	}
		
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	JASSERT(IsValid(PlayerController), "PlayerController cannot be null");
	
	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(PlayerController);
	JASSERT(IsValid(BBPlayerController), "BBPlayerController cannot be null");

	BBPlayerController->AddPrintChattingMessage(NewMessage);	
}
