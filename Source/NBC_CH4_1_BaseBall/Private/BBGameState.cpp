#include "BBGameState.h"
#include "Kismet/GameplayStatics.h"
#include "BBPlayerController.h"
#include "JUtility.h"

ABBGameState::ABBGameState()
{
	bReplicates = true;
}

void ABBGameState::Multicast_ResetChatMessage_Implementation()
{
	APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	JASSERT(IsValid(LocalPlayerController), "PlayerController cannot be null");
	
	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(LocalPlayerController);
	JASSERT(IsValid(BBPlayerController), "BBPlayerController cannot be null");

	BBPlayerController->ResetChatMessage();
}

void ABBGameState::Multicast_SetAnnounceMessage_Implementation(const FString& NewAnnounceMessage)
{
	APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	JASSERT(IsValid(LocalPlayerController), "PlayerController cannot be null");
	
	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(LocalPlayerController);
	JASSERT(IsValid(BBPlayerController), "BBPlayerController cannot be null");

	BBPlayerController->SetAnnounceMessage(NewAnnounceMessage);
}

void ABBGameState::MultiCast_AddChatMessage_Implementation(const FString& NewMessage)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	JASSERT(IsValid(PlayerController), "PlayerController cannot be null");
		
	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(PlayerController);
	JASSERT(IsValid(BBPlayerController), "BBPlayerController cannot be null");

	BBPlayerController->AddPrintChattingMessage(NewMessage);	
}
