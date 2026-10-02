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
	
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	void AddChatHistory(const FString& NewMessage);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> TextInput;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChatHistory;
};
