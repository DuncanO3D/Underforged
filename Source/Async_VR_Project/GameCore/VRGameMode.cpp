#include "VRGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Actor.h"
#include "Character/PlayerStateVR.h"
#include "GameFramework/GameStateBase.h"
#include "Character/PawnVR.h"
#include "GameFramework/PlayerState.h"

AVRGameMode::AVRGameMode() : AGameMode()
{
	MaxPlayers = 2;
	MapName = "MainMap";
	bUseSeamlessTravel = true;
}

void AVRGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (PlayerControllerList.Contains(NewPlayer) == false)
	{
		PlayerControllerList.Add(NewPlayer);
		
		AActor * playerStart = FindRightPlayerStart(NewPlayer);
		
		if (playerStart != nullptr)
		{
			NewPlayer->StartSpot = playerStart;
			RestartPlayer(NewPlayer);
		
			playerStart->Destroy();
		}
	}
}

void AVRGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	APlayerController * playerController = Cast<APlayerController>(Exiting);
	if (playerController != nullptr)
	{
		if (PlayerControllerList.Contains(playerController) == true)
		{
			PlayerControllerList.Remove(playerController);
		}
	}
}

bool AVRGameMode::AllPlayersAreReady()
{
	auto gameState = GetWorld()->GetGameState();
	if (gameState && gameState->PlayerArray.Num() > 1) {
		for (size_t i = 0; i < gameState->PlayerArray.Num(); i++) {

			APlayerStateVR * playerState = Cast<APlayerStateVR>(gameState->PlayerArray[i]);
			if (!playerState)
				return false;
			if (!playerState->IsReady())
				return false;
		}
		return true;
	}
	return false;
}

bool AVRGameMode::StartGame()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Starting game");
	auto gameState = GetWorld()->GetGameState();
	if (gameState && gameState->PlayerArray.Num() > 1) {
		for (size_t i = 0; i < gameState->PlayerArray.Num(); i++) {
			APlayerStateVR * playerState = Cast<APlayerStateVR>(gameState->PlayerArray[i]);
			if (playerState) {
				playerState->StartGame();
				APawnVR * pawn = Cast<APawnVR>(playerState->GetPawn());
				if (pawn)
					pawn->EnableMotionInteractions();
			}
		}
		return true;
	}
	return false;
}

bool AVRGameMode::EndGame()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "Ending game");
	auto gameState = GetWorld()->GetGameState();
	if (gameState && gameState->PlayerArray.Num() > 1) {
		for (size_t i = 0; i < gameState->PlayerArray.Num(); i++) {
			APlayerStateVR * playerState = Cast<APlayerStateVR>(gameState->PlayerArray[i]);
			if (playerState) {
				playerState->EndGame();
				APawnVR * pawn = Cast<APawnVR>(playerState->GetPawn());
				if (pawn) {
					pawn->DisableMotionInteractions();
				}
			}
		}
		return true;
	}
	return false;
}

AActor * AVRGameMode::FindRightPlayerStart(APlayerController * Player)
{
	if(m_PlayerStartList.Num() == 0)
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), m_PlayerStartList);

	int index = GetHigherPlayerStartIndex(m_PlayerStartList);
	
	AActor * playerStart = m_PlayerStartList[index];

	m_PlayerStartList.RemoveAt(index);

	return playerStart;
}

int AVRGameMode::GetHigherPlayerStartIndex(TArray<AActor*> PlayerStartList)
{
	int HigherIndex = 0;
	int MaxHeight = -10000;

	for (int i = 0; i < PlayerStartList.Num(); i++)
	{
		if (PlayerStartList[i]->GetActorLocation().Z > MaxHeight)
		{
			MaxHeight = PlayerStartList[i]->GetActorLocation().Z;
			HigherIndex = i;
		}
	}

	return HigherIndex;
}
