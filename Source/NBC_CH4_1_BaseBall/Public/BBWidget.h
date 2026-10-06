#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BBWidget.generated.h"

class UTextBlock;
class UEditableText;
UCLASS()
class NBC_CH4_1_BASEBALL_API UBBWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	void AddChatHistory(const FString& NewMessage);
	void SetAnnounceText(const FString& NewAnnounceText);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> TextInput;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChatHistory;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AnnounceText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Widget===|Properties");
	float AnnounceDisplayDuration = 3.0f;
	/////////////////////////////////////////////
	/// START NO UPROPERTY
	/////////////////////////////////////////////
protected:
	float LastAnnounceTime;
};
