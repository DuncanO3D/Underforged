#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "VRGameMode.generated.h"

UCLASS()
class ASYNC_VR_PROJECT_API AVRGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AVRGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRule")
		int MaxPlayers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRule")
		FName MapName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Players")
		TSet<class APlayerController*> PlayerControllerList;

	virtual void PostLogin(APlayerController * NewPlayer) override;
	virtual void Logout(AController * Exiting) override;

	UFUNCTION(BlueprintCallable)
		bool AllPlayersAreReady();
	UFUNCTION(BlueprintCallable)
		bool StartGame();
	UFUNCTION(BlueprintCallable)
		bool EndGame();

private:
	UFUNCTION()
		AActor * FindRightPlayerStart(class APlayerController* Player);
	UFUNCTION()
		int GetHigherPlayerStartIndex(TArray<AActor*> PlayerStartList);

	UPROPERTY()
		TArray<AActor*> m_PlayerStartList;
};
