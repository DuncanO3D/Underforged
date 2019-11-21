// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/InteractableActor.h"
#include "InteractableSpawner.generated.h"

class ACraftingMaterial;

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API AInteractableSpawner : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	AInteractableSpawner();

	virtual AInteractableActor * GetPick(USceneComponent * _toAttach) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACraftingMaterial> m_ClassMaterial;

};
