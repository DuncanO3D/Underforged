// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Features/Recipe.h"
#include "Features/Interface/HeatableInterface.h"

#include "CoreMinimal.h"
#include "Features/Interactable/InteractableActor.h"
#include "FoundryBowl.generated.h"

class UBoxComponent;
class AMold;
class USplineMeshComponent;

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API AFoundryBowl : public AInteractableActor, public IHeatableInterface
{
	GENERATED_BODY()
	
public:
	AFoundryBowl();

	/*** EVENT FUNCTIONS ***/

	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginTimeHeat();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEndTimeHeat();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDeverseLiquid();

	void UpdateHeat(float _DeltaTime) override;

	void CleanFoundryBowl();

	virtual void BeginHeat() override;

	virtual void EndHeat() override;

	UFUNCTION(BlueprintCallable)
		bool TryDeverseTo(AMold * _MoldToDeverse);

	UFUNCTION(BlueprintCallable)
		bool CanDeverseTo(AMold * _MoldToDeverse);

	UFUNCTION(BlueprintCallable)
		void DeverseTo(AMold * _MoldToDeverse);

	void LoadRecipeForReplicate(FRecipe _Recipe);

	UFUNCTION()
		TArray<ACraftingMaterial*> GetCraftingMaterialInFoundryBowl();

protected:
	virtual bool CanGetPick(USceneComponent * _toAttach) override;

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		bool CanBeginTimeHeat();

	UFUNCTION()
		bool CanEndTimeHeat();

	UFUNCTION(BlueprintCallable)
		void BeginTimeHeat();

	UFUNCTION(BlueprintCallable)
		void EndTimeHeat();

	UFUNCTION()
		void DropIfHand();

	

	UFUNCTION()
		bool IsInclined();

	UFUNCTION()
		bool LoadRecipe();

	#pragma region Attributes

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent * m_OverlappBoxCraftedMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USplineMeshComponent * m_SplineMeshLiquid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_InHeating = false;

private:
	UPROPERTY(VisibleAnywhere)
		FRecipe m_ActualRecipe;

	UPROPERTY(VisibleAnywhere)
		float m_ActualHeatingTime = 0;

	UPROPERTY(VisibleAnywhere)
		bool m_HasLiquid = false;

	UPROPERTY(EditAnywhere)
		float m_AngleRequiredForDeverseLiquid = 80;

	int m_TestUpdateHeat = 0;

	int m_TimeInclined = 0;

	#pragma endregion

};
