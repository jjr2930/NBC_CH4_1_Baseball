#include "BBWidget.h"

#include "BBPlayerController.h"
#include "JUtility.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void UBBWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	JASSERT(IsValid(TextInput), "TextInput is not valid!");
	
	TextInput->OnTextCommitted.AddDynamic(this, &UBBWidget::OnTextCommitted);
}

void UBBWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	float Now = GetWorld()->GetTimeSeconds();
	
	if (Now - LastAnnounceTime > AnnounceDisplayDuration)
	{
		SetAnnounceText(TEXT(""));
	}
}

void UBBWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::OnEnter:
			{
				JASSERT( GetOwningPlayer()->IsLocalController(), "this widget must be owned by a local player controller!" );			
				
				ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(GetOwningPlayer());
				JASSERT(IsValid(BBPlayerController), "Owning player controller is not a BBPlayerController!");
				
				BBPlayerController->ServerRpcOnChatCommitted(Text.ToString());				
				break;
			}
		default:
			// other case not supported yet
			break;
	}
}

void UBBWidget::AddChatHistory(const FString& NewMessage)
{	
	JASSERT(IsValid(ChatHistory), "ChatHistory is not valid!");
	
	FString NextText = ChatHistory->GetText().ToString() + LINE_TERMINATOR + NewMessage;
	ChatHistory->SetText(FText::FromString(NextText));
}

void UBBWidget::SetAnnounceText(const FString& NewAnnounceText)
{
	JASSERT(IsValid(AnnounceText), "AnnounceText is not valid!");
	AnnounceText->SetText(FText::FromString(NewAnnounceText));
	
	LastAnnounceTime = GetWorld()->GetTimeSeconds();
}
