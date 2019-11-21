// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Features/Recipe.h"

#include "BlacksmithFactory.generated.h"

class ACraftingMaterial;
class AFinalProduct;
class ABlade;
class AHilt;

USTRUCT(BlueprintType)
struct FOrderElement
{
	GENERATED_BODY()

	//Constructor
	FOrderElement()
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponElement")
		TSubclassOf<ABlade> m_bladeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponElement")
		TSubclassOf<AHilt> m_hiltTypeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponElement")
		TSubclassOf<AFinalProduct> m_finalProductClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponElement")
		FString m_bladeName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponElement")
		FString m_hiltName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponElement")
		FString m_swordName;
};
/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class ASYNC_VR_PROJECT_API UBlacksmithFactory : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		int32 m_randFinalProduct = 0;
	UBlacksmithFactory();

	FRecipe GetCorrespondantRecipe(TArray<ACraftingMaterial*> _MaterialsUsed);

	FOrderElement GetRandomOrder();
	FText GetFinalProductText();
	FOrderElement GetCorrespondandOrder(ABlade * _blade, AHilt * _hilt);

	TSubclassOf<AFinalProduct> GetClassFinalProductRef();

private:
	UPROPERTY(EditAnywhere, Category = "Recipe")
		TArray<FRecipe> m_ListRecipe;

	UPROPERTY(EditAnywhere, Category = "Recipe")
		FRecipe m_DefaultRecipe;

	UPROPERTY(EditAnywhere, Category = "Other")
		TSubclassOf<AFinalProduct> m_ClassFinalProductBP;

	UPROPERTY(EditAnywhere, Category = "Order")
		TArray<FOrderElement> m_listOrderElement;

	UPROPERTY(EditAnywhere, Category = "Order")
		FOrderElement m_DefaultOrder;
};
