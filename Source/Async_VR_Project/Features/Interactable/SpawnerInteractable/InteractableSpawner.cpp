// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableSpawner.h"

#include "Features/Interactable/CraftingMaterial/CraftingMaterial.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AInteractableSpawner::AInteractableSpawner()
{

}

AInteractableActor * AInteractableSpawner::GetPick(USceneComponent * _toAttach)
{
	AInteractableActor * interactableActor = GetWorld()->SpawnActor<AInteractableActor>(m_ClassMaterial);

	if (interactableActor)
	{
		interactableActor->SetActorLocationAndRotation(_toAttach->GetComponentLocation(), _toAttach->GetComponentRotation());
		interactableActor->TryGetPick(_toAttach);
	}

	return interactableActor;
}
