// Fill out your copyright notice in the Description page of Project Settings.

#include "Recipe.h"

#include "Features/Interactable/WeaponElement/WeaponElement.h"
#include "Features/Interactable/CraftingMaterial/Ingot.h"

bool FRecipe::IsRecipeCorrespondingTo(TArray<ACraftingMaterial*> _Materials)
{
	if (_Materials.Num() != m_MaterialForRecipe.Num())
		return false;

	TArray<TSubclassOf<ACraftingMaterial>> _CopieMaterialRequired = m_MaterialForRecipe;

	for (int i = 0; i < _Materials.Num(); i++)
	{
		for (int j = 0; j < _CopieMaterialRequired.Num(); j++)
		{
			if (_Materials[i]->GetClass() == _CopieMaterialRequired[j])
			{
				_CopieMaterialRequired.RemoveAt(j);
				break;
			}
		}
	}

	return _CopieMaterialRequired.Num() == 0;
}
