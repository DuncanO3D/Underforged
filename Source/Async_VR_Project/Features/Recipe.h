// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Features/Interactable/CraftingMaterial/CraftingMaterial.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Recipe.generated.h"

class AInteractableActor;
class AIngot;
class AWeaponElement;

USTRUCT(BlueprintType)
struct FRecipe
{
	GENERATED_BODY()

	//Constructor
	FRecipe()
	{
		
	}

	bool IsRecipeCorrespondingTo(TArray<ACraftingMaterial*> _Materials);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<ACraftingMaterial>> m_MaterialForRecipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_HeatingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_CoolingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AIngot> m_IngotWhoCanBeCrafted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AWeaponElement>> m_WeaponsElementWhoCanBeCrafted;

};