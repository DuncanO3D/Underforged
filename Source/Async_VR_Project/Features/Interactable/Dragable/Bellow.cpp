// Fill out your copyright notice in the Description page of Project Settings.

#include "Bellow.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"

#include "Features/Furnace.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Character/PlayerStateVR.h"

#include "GameCore/OnlineGameInstance.h"

#include "Engine/Engine.h"
#include "Engine/World.h"

ABellow::ABellow() : ADragable()
{
	m_bone = CreateDefaultSubobject<USkeletalMeshComponent>("bones");
	m_bone->SetupAttachment(m_Root);

	bReplicates = true;
	bReplicateMovement = true;
}

void ABellow::Drag_Implementations(FVector dragVelocity)
{
	if (dragVelocity != FVector::ZeroVector)
	{
		FVector ZdragVelocity = dragVelocity;
		ZdragVelocity.X = 0;
		ZdragVelocity.Y = 0;

		FVector newLocation = GetActorLocation() + ZdragVelocity;

		if (newLocation.Z >= m_minHeight && newLocation.Z <= m_maxHeight)
		{
			/*SetActorLocation(GetActorLocation() + ZdragVelocity);*/
			m_bone->BoneSpaceTransforms[1].SetTranslation(newLocation);
			for (int i = 0; i < m_furnaces.Num(); i++)
			{
				m_furnaces[i]->IncreaseFlame(ZdragVelocity.Z);
			}
			OnDrag(ZdragVelocity);
		}
	}
}

void ABellow::Select()
{
	Super::Select();
	bool outline = m_NumberOfSelection > 0;
	m_bone->SetRenderCustomDepth(outline);
}

void ABellow::Deselect()
{
	Super::Deselect();
	bool outline = m_NumberOfSelection > 0;
	m_bone->SetRenderCustomDepth(outline);
}

void ABellow::Drag(FVector dragVelocity)
{
	auto gameState = GetWorld()->GetGameState();
	if (gameState)
	{
		for (size_t i = 0; i < gameState->PlayerArray.Num(); i++)
		{
			APlayerStateVR * playerState = Cast<APlayerStateVR>(gameState->PlayerArray[i]);
			if (playerState)
			{
				if (!GetWorld()->GetAuthGameMode())
				{
					playerState->BelowDragOnServ(this, dragVelocity);
					
				}
				if (i == 0)
				{
					playerState->BelowDragOnNetMulticast(this, dragVelocity);
				}
			}
		}
	}
}
 
