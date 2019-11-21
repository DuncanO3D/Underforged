// Fill out your copyright notice in the Description page of Project Settings.

#include "Mold.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Features/Interactable/FoundryBowl.h"
#include "Engine/Engine.h"

#include "Features/Interactable/WeaponElement/WeaponElement.h"
#include "Features/Interactable/CraftingMaterial/Ingot.h"

#include "Character/PlayerStateVR.h"
#include "Character/PlayerControllerVR.h"

AMold::AMold()
{
	m_MeshCubeHeat = CreateDefaultSubobject<UStaticMeshComponent>("MeshCubeHeat");
	m_MeshCubeHeat->SetupAttachment(m_Root);

	m_DeverseLiquidTriggerBox = CreateDefaultSubobject<UBoxComponent>("DeverseLiquidCollider");
	m_DeverseLiquidTriggerBox->SetupAttachment(m_Root);

	m_PointDeverseLiquid = CreateDefaultSubobject<USceneComponent>("PointDeverseLiquid");
	m_PointDeverseLiquid->SetupAttachment(m_Root);
}

bool AMold::IsCool()
{
	return !ContainLiquid();
}

void AMold::Select()
{
	
}

void AMold::BeginPlay()
{
	Super::BeginPlay();

	m_MeshCubeHeat->SetVisibility(false);

	m_DeverseLiquidTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMold::OnBeginOverlap);
	m_DeverseLiquidTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMold::OnEndOverlap);
}

void AMold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ContainSomething())
	{
		for (int i = 0; i < m_ListFoundryBowls.Num(); i++)
		{
			if (m_ListFoundryBowls[i]->CanDeverseTo(this))
			{
				APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
				if (playerStateVR)
				{
					playerStateVR->DeverseFoundryBowlToOnServ(m_ListFoundryBowls[i], this);
					playerStateVR->DeverseFoundryBowlToOnNetMulticast(m_ListFoundryBowls[i], this);
				}
				break;
			}
		}
	}

}

bool AMold::BeginUse()
{
	if (!ContainLiquid())
		Toggle();
	return true;
}

void AMold::BeginCool()
{

}

void AMold::UpdateCool(float _deltaTime)
{

}

void AMold::EndCool()
{

}

void AMold::AddWater()
{
	OnAddWater();
	if (ContainLiquid())
	{
		m_ActualTime += m_ActualRecipe.m_CoolingTime;
		if (IsMoldingFinished())
		{
			if (!GetWorld()->GetAuthGameMode())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "MoldingFinished");
				APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
				if (playerStateVR)
				{
					playerStateVR->SpawnRecipeMoldOnServ(this);
				}
			}
			ActualiseAfterSpawnInteractable();
		}
	}
}

void AMold::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFoundryBowl * foundryBowl = Cast<AFoundryBowl>(OtherActor);
	if (foundryBowl)
	{
		m_ListFoundryBowls.AddUnique(foundryBowl);
	}
}

void AMold::OnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AFoundryBowl * foundryBowl = Cast<AFoundryBowl>(OtherActor);
	if (foundryBowl)
	{
		m_ListFoundryBowls.Remove(foundryBowl);
	}
}

bool AMold::IsOpen()
{
	return m_IsOpen;
}

void AMold::Toggle()
{
	if (IsOpen())
		Close();
	else
		Open();
}

void AMold::Open()
{
	m_IsOpen = true;
	OnOpen();
}

void AMold::Close()
{
	m_IsOpen = false;
	OnClose();
}

bool AMold::ContainSomething()
{
	return ContainLiquid() || ContainObjectSpawned();
}

bool AMold::ContainObjectSpawned()
{
	return m_SpawnedCraftingMaterial != nullptr && m_SpawnedCraftingMaterial->GetAttachParentActor() == this;
}

bool AMold::ContainLiquid()
{
	return m_HasLiquid;
}

bool AMold::TryAddRecipe(FRecipe _Recipe)
{
	if (CanAddRecipe(_Recipe))
	{
		AddRecipe(_Recipe);

		return true;
	}
	return false;
}

AInteractableActor * AMold::GetPick(USceneComponent * _toAttach, bool _snapToTarget, bool _disablePhysicsQuery)
{
	if (ContainObjectSpawned())
	{
		AInteractableActor * interactableActor = m_SpawnedCraftingMaterial->TryGetPick(_toAttach);
		if (interactableActor != nullptr)
		{
			m_SpawnedCraftingMaterial->PhysicalCollision();
			m_SpawnedCraftingMaterial = nullptr;
			interactableActor->SetActorLocation(_toAttach->GetComponentLocation());
		}

		return interactableActor;
	}
	else
	{
		return nullptr;
	}
}

void AMold::Drop(USceneComponent * _toDetach)
{
	
}

bool AMold::HasGoodInclinaison()
{
	return true;
}

bool AMold::CanAddRecipe(FRecipe _Recipe)
{

	if (ContainSomething())
		return false;

	for (int i = 0; i < m_AllowedIngots.Num(); i++)
	{
		if (_Recipe.m_IngotWhoCanBeCrafted == m_AllowedIngots[i])
			return true;
	}

	for (int i = 0; i < m_AllowedWeaponElements.Num(); i++)
	{
		for (int j = 0; j < _Recipe.m_WeaponsElementWhoCanBeCrafted.Num(); j++)
		{
			if (m_AllowedWeaponElements[i] == _Recipe.m_WeaponsElementWhoCanBeCrafted[j])
				return true;
		}
	}

	return false;
}

void AMold::AddRecipe(FRecipe _Recipe)
{
	m_ActualRecipe = _Recipe;
	bool found = false;
	for (int i = 0; i < m_AllowedIngots.Num(); i++)
	{
		if (_Recipe.m_IngotWhoCanBeCrafted == m_AllowedIngots[i])
		{
			m_ClassMaterialToSpawn = _Recipe.m_IngotWhoCanBeCrafted;
			found = true;
			break;
		}
	}

	if (found == false)
	{
		for (int i = 0; i < m_AllowedWeaponElements.Num(); i++)
		{
			for (int j = 0; j < _Recipe.m_WeaponsElementWhoCanBeCrafted.Num(); j++)
			{
				if (m_AllowedWeaponElements[i] == _Recipe.m_WeaponsElementWhoCanBeCrafted[j])
				{
					m_ClassMaterialToSpawn = _Recipe.m_WeaponsElementWhoCanBeCrafted[j];
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
	}

	m_HasLiquid = true;
	m_ActualTime = 0;

	m_MeshCubeHeat->SetVisibility(true);

	OnAddRecipe();
}

void AMold::SpawnInteractableFromRecipe(AInteractableActor * spawnedMaterial)
{
	if (spawnedMaterial)
	{
		m_SpawnedCraftingMaterial = spawnedMaterial;
		spawnedMaterial->TryGetPick(m_Root);
		spawnedMaterial->SetActorLocationAndRotation(m_MeshCubeHeat->GetComponentLocation(), m_MeshCubeHeat->GetComponentRotation());
		m_MeshCubeHeat->SetVisibility(false);
		m_HasLiquid = false;
		spawnedMaterial->NoCollision();
	}
}

void AMold::ActualiseAfterSpawnInteractable()
{
	m_HasLiquid = false;
	m_MeshCubeHeat->SetVisibility(false);
	if (m_SpawnedCraftingMaterial)
	{
		m_SpawnedCraftingMaterial->TryGetPick(m_Root);
		m_SpawnedCraftingMaterial->SetActorLocationAndRotation(m_MeshCubeHeat->GetComponentLocation(), m_MeshCubeHeat->GetComponentRotation());
		m_SpawnedCraftingMaterial->NoCollision();
	}
}

bool AMold::IsMoldingFinished()
{
	return m_ActualTime >= m_ActualRecipe.m_CoolingTime;
}
