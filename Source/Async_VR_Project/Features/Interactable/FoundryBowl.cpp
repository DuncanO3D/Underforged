// Fill out your copyright notice in the Description page of Project Settings.

#include "FoundryBowl.h"
#include "Components/StaticMeshComponent.h"
#include "Character/MotionControllerVR.h"
#include "Features/Factory/BlacksmithFactory.h"
#include "GameCore/OnlineGameInstance.h"
#include "Features/Interactable/Usable/Mold.h"
#include "Components/SplineMeshComponent.h"

#include "Engine/Engine.h"

#include "Components/BoxComponent.h"

#include "Features/Interactable/WeaponElement/WeaponElement.h"
#include "Features/Interactable/CraftingMaterial/Ingot.h"

#include "Character/PlayerStateVR.h"
#include "Character/PlayerControllerVR.h"

AFoundryBowl::AFoundryBowl() : AInteractableActor()
{
	m_OverlappBoxCraftedMaterial = CreateDefaultSubobject<UBoxComponent>("OverlappBox For CraftedMaterials");
	m_OverlappBoxCraftedMaterial->SetupAttachment(m_Root);

	m_SplineMeshLiquid = CreateDefaultSubobject<USplineMeshComponent>("Spline Mesh liquid");
	m_SplineMeshLiquid->SetupAttachment(m_Root);
}

bool AFoundryBowl::CanGetPick(USceneComponent * _toAttach)
{
	if (m_HasLiquid)
	{
		AMotionControllerVR * motionControllerVR = Cast<AMotionControllerVR>(_toAttach->GetOwner());
		if (motionControllerVR)
		{
			motionControllerVR->RumbleController(0.3f);
			return false;
		}
	}
	return true;
}

void AFoundryBowl::BeginPlay()
{
	Super::BeginPlay();

}

void AFoundryBowl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInclined() && m_HasLiquid)
	{
		m_TimeInclined++;
		if (m_TimeInclined >= 5)
		{
			CleanFoundryBowl();
			m_TimeInclined = 0;
		}
	}
}

void AFoundryBowl::UpdateHeat(float _DeltaTime)
{
	if (m_InHeating)
	{
		DropIfHand();
		m_ActualHeatingTime += _DeltaTime;
		if (CanEndTimeHeat())
			EndTimeHeat();
	}
}

void AFoundryBowl::CleanFoundryBowl()
{
	OnDeverseLiquid();
	m_HasLiquid = false;
}

void AFoundryBowl::BeginHeat()
{
	if (CanBeginTimeHeat())
	{
		BeginTimeHeat();
	}
}

void AFoundryBowl::EndHeat()
{
	// Nothing
}

bool AFoundryBowl::CanBeginTimeHeat()
{
	TArray<ACraftingMaterial*> craftingMaterials = GetCraftingMaterialInFoundryBowl();
	return craftingMaterials.Num() > 0 && m_HasLiquid == false;
}

bool AFoundryBowl::CanEndTimeHeat()
{
	return m_ActualHeatingTime >= m_ActualRecipe.m_HeatingTime;
}

void AFoundryBowl::BeginTimeHeat()
{
	if (LoadRecipe())
	{
		// TEST WITH PLAYER CONTROLLER VR
		//APlayerControllerVR * playerController = APlayerControllerVR::GetPlayerController(GetWorld());
		//if (playerController)
		//{
		//	playerController->LoadRecipeFoundryBowlOnNetMulticast(this, m_ActualRecipe);
		//	playerController->LoadRecipeFoundryBowlOnServ(this, m_ActualRecipe);
		//}

		
		APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
		if (playerStateVR)
		{
			playerStateVR->LoadRecipeFoundryBowlOnServ(this, m_ActualRecipe);
			playerStateVR->LoadRecipeFoundryBowlOnNetMulticast(this, m_ActualRecipe);
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 50, FColor::Red, "Error accessing onlineGameInstance for recipe");
	}	
}

void AFoundryBowl::EndTimeHeat()
{
	m_InHeating = false;

	APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());

	TArray<ACraftingMaterial*> _Materials = GetCraftingMaterialInFoundryBowl();
	for (int i = 0; i < _Materials.Num(); i++)
	{
		if (playerStateVR)
		{
			playerStateVR->DestroyActorOnServ(_Materials[i]);
			playerStateVR->DestroyActorNetMulticast(_Materials[i]);
		}
	}

	OnEndTimeHeat();
}

void AFoundryBowl::DropIfHand()
{
	if (m_AttachedComponent)
	{
		AMotionControllerVR * motionController = Cast<AMotionControllerVR>(m_AttachedComponent->GetOwner());
		
		if (motionController)
		{
			motionController->RumbleController(0.3f);
			motionController->ReleaseActor();
		}
	}
}

TArray<ACraftingMaterial*> AFoundryBowl::GetCraftingMaterialInFoundryBowl()
{
	TArray<AActor*> m_ListActor;
	TArray<ACraftingMaterial*> m_ListCraftingMaterials;
	m_OverlappBoxCraftedMaterial->GetOverlappingActors(m_ListActor);

	ACraftingMaterial * craftingMaterial;
	for (int i = 0; i < m_ListActor.Num(); i++)
	{
		craftingMaterial = Cast<ACraftingMaterial>(m_ListActor[i]);
		if (craftingMaterial)
			m_ListCraftingMaterials.Add(craftingMaterial);
	}

	return m_ListCraftingMaterials;
}

bool AFoundryBowl::TryDeverseTo(AMold * _MoldToDeverse)
{
	if (!IsInclined())
		return false;

	if (m_HasLiquid == false || m_InHeating == true)
		return false;

	if (_MoldToDeverse->TryAddRecipe(m_ActualRecipe))
	{
		m_HasLiquid = false;
		OnDeverseLiquid();
		// Activation du fait de verser le liquide
		m_TimeInclined = 0;

		return true;
	}

	return false;
}

bool AFoundryBowl::CanDeverseTo(AMold * _MoldToDeverse)
{
	if (!IsInclined())
		return false;

	if (m_HasLiquid == false || m_InHeating == true)
		return false;

	return _MoldToDeverse->CanAddRecipe(m_ActualRecipe);
}

void AFoundryBowl::DeverseTo(AMold * _MoldToDeverse)
{
	if (_MoldToDeverse)
	{
		_MoldToDeverse->AddRecipe(m_ActualRecipe);
		m_HasLiquid = false;
		OnDeverseLiquid();
	}
}

void AFoundryBowl::LoadRecipeForReplicate(FRecipe _Recipe)
{
	m_ActualRecipe = _Recipe;

	m_InHeating = true;
	m_HasLiquid = true;
	m_ActualHeatingTime = 0;

	//APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
	//
	//TArray<ACraftingMaterial*> _Materials = GetCraftingMaterialInFoundryBowl();
	//for (int i = 0; i < _Materials.Num(); i++)
	//{
	//	if (playerStateVR)
	//	{
	//		playerStateVR->DestroyActorOnServ(_Materials[i]);
	//		playerStateVR->DestroyActorNetMulticast(_Materials[i]);
	//	}
	//}

	OnBeginTimeHeat();
}

bool AFoundryBowl::IsInclined()
{
	FVector upWorld = FVector(0, 0, 1);
	FVector upFoundry = GetActorUpVector();

	float angle = FQuat::FindBetweenVectors(upWorld, upFoundry).GetAngle();
	angle = FMath::RadiansToDegrees(angle);

	return angle >= m_AngleRequiredForDeverseLiquid;
}

bool AFoundryBowl::LoadRecipe()
{
	UOnlineGameInstance * gameInstance = UOnlineGameInstance::GetInstance(GetWorld());
	if (gameInstance)
	{
		m_ActualRecipe = gameInstance->GetBlacksmithFactory()->GetCorrespondantRecipe(GetCraftingMaterialInFoundryBowl());
		FString string = "Recipe for : " + m_ActualRecipe.m_IngotWhoCanBeCrafted->GetName();
		for (int i = 0; i < m_ActualRecipe.m_WeaponsElementWhoCanBeCrafted.Num(); i++)
		{
			string += " | " + m_ActualRecipe.m_WeaponsElementWhoCanBeCrafted[i]->GetName();
		}
		//GEngine->AddOnScreenDebugMessage(-1, 50, FColor::Yellow, string);
		return true;
	}
	else
		return false;
}

