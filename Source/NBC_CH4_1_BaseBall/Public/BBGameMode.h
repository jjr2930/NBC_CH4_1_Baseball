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
	ABBPlayerController* GetPlayerControllerByIndex(int32 index) const;
		
	void ResetCurrentTurnPlayer();
	void SetCurrentTurnPlayer(int32 index);
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Baseball===|Properties")
	TArray<int32> Answer;
	
	TArray<int32> TempPlayerAnswer;
		
	///////////////////////////////////////
	/// START NO UPROPERTY 
	///////////////////////////////////////
protected:
	UPROPERTY()
	ABBPlayerController* CurrentTurnPlayer;

	UPROPERTY()
	TArray<ABBPlayerController*> PlayerControllers;
	
	UPROPERTY()
	TObjectPtr<UBBStateMachine> StateMachine;
};
