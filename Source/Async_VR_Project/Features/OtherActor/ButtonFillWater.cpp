// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonFillWater.h"

#include "Components/StaticMeshComponent.h"
#include "Features/Interactable/Usable/Mold.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Character/PlayerStateVR.h"

// Sets default values
AButtonFillWater::AButtonFillWater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//m_MeshRoot = m_MeshRoot->CreateDefaultSubobject<UStaticMeshComponent>("Root");
	//m_MeshRoot->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AButtonFillWater::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AButtonFillWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtonFillWater::PushButton()
{
	auto gameState = GetWorld()->GetGameState();
	if (gameState)
	{
		for (size_t i = 0; i < gameState->PlayerArray.Num(); i++)
		{
			APlayerStateVR * playerState = Cast<APlayerStateVR>(gameState->PlayerArray[i]);
			if (playerState)
			{
				for (int j = 0; j < m_Mold.Num(); j++)
				{
					playerState->FillWaterMoldOnServer(m_Mold[j]);
					playerState->FillWaterMoldOnMulticast(m_Mold[j]);
				}
				
			}
		}
	}

	
}

