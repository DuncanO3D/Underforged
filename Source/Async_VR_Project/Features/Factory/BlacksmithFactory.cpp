// Fill out your copyright notice in the Description page of Project Settings.

#include "BlacksmithFactory.h"

#include "Features/Interactable/CraftingMaterial/CraftingMaterial.h"

#include "Features/Interactable/WeaponElement/Blade.h"
#include "Features/Interactable/WeaponElement/Hilt.h"

UBlacksmithFactory::UBlacksmithFactory()
{
}


FRecipe UBlacksmithFactory::GetCorrespondantRecipe(TArray<ACraftingMaterial*> _MaterialsUsed)
{
	for (int i = 0; i < m_ListRecipe.Num(); i++)
	{
		if (m_ListRecipe[i].IsRecipeCorrespondingTo(_MaterialsUsed))
		{
			return m_ListRecipe[i];
		}
	}

	return m_DefaultRecipe;
}

FOrderElement UBlacksmithFactory::GetRandomOrder()
{
	m_randFinalProduct = FMath::RandRange(0, m_listOrderElement.Num() - 1);
	return m_listOrderElement[m_randFinalProduct];
}


FText UBlacksmithFactory::GetFinalProductText()
{
	FString nbOrder = FString::FromInt(m_randFinalProduct);
	FString OrderHUD = nbOrder + FString(TEXT(""));
	FText OrderText = FText::FromString(OrderHUD);
	return OrderText;
}


FOrderElement UBlacksmithFactory::GetCorrespondandOrder(ABlade * _blade, AHilt * _hilt)
{
	if (_blade == nullptr || _hilt == nullptr)
		return m_DefaultOrder;

	for (int i = 0; i < m_listOrderElement.Num(); i++)
	{
		if (m_listOrderElement[i].m_bladeClass == _blade->GetClass() && m_listOrderElement[i].m_hiltTypeClass == _hilt->GetClass())
			return m_listOrderElement[i];
	}

	return m_DefaultOrder;
}

TSubclassOf<AFinalProduct> UBlacksmithFactory::GetClassFinalProductRef()
{
	return m_ClassFinalProductBP;
}
