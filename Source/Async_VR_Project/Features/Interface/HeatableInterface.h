// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HeatableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHeatableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASYNC_VR_PROJECT_API IHeatableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(Category = "HeatReaction")
		virtual void UpdateHeat(float _deltaTime);
	UFUNCTION(Category = "HeatReaction")
		virtual void BeginHeat();
	UFUNCTION(Category = "HeatReaction")
		virtual void EndHeat();
};
