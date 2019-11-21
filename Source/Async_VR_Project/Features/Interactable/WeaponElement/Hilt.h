// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/WeaponElement/WeaponElement.h"
#include "Hilt.generated.h"

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API AHilt : public AWeaponElement
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

};
