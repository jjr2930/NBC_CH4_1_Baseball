#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameMode.generated.h"

class UBBWidget;
class ABBPlayerController;
class UBBStateMachine;

UCLASS()
class NBC_CH4_1_BASEBALL_API ABBGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABBGameMode();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;	
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	
	void OnChatCommitted(const FString& InputString, AController* PlayerController);
	void BroadcastAnnounceMessage(const FString& Message);
	void BroadcastChatMessage(const FString& Message);
	
	void AddPlayerController(ABBPlayerController* NewPlayerController) ;
	int32 GetPlayerControllerCount() const;
		
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Baseball===|Properties")
	TArray<int32> Answer;
	
	TArray<int32> TempPlayerAnswer;
		
	///////////////////////////////////////
	/// START NO UPROPERTY 
	///////////////////////////////////////
protected:
	UPROPERTY()
	ABBPlayerController* CurrentPlayer;

	UPROPERTY()
	TArray<ABBPlayerController*> PlayerControllers;
	
	UPROPERTY()
	TObjectPtr<UBBStateMachine> StateMachine;
};
