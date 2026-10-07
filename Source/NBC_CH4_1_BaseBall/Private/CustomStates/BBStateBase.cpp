#include "CustomStates/BBStateBase.h"
#include "JUtility.h"
#include "BBGameMode.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "Kismet/GameplayStatics.h"

void UBBStateBase::SetOwnerGameMode(ABBGameMode* InOwnerGameMode)
{
	OwnerGameMode = InOwnerGameMode;
}

void UBBStateBase::OnPostLogin(AController* NewPlayer)
{
}

void UBBStateBase::SendChatMessage(const FString& NewMessage)
{
	OwnerGameMode->BroadcastChatMessage(NewMessage);
}

void UBBStateBase::SendAnnounceMessage(const FString& NewAnnounceMessage)
{
	OwnerGameMode->BroadcastAnnounceMessage(NewAnnounceMessage);
}

void UBBStateBase::OnPlayerMessageCommitted(const FString& InputString, AController* Sender)
{
}
