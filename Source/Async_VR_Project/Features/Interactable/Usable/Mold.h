// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/Usable/UsableActor.h"
#include "Features/Interface/CoolableInterface.h"
#include "Features/Recipe.h"
#include "Mold.generated.h"

class UBoxComponent;
class AFoundryBowl;
class USceneComponent;
class AWeaponElement;
class AInteractableActor;

class UMaterialInterface;

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API AMold : public AUsableActor, public ICoolableInterface
{
	GENERATED_BODY()
	
public:
	AMold();


	virtual bool IsCool() override;


	virtual void Select() override;

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	virtual bool BeginUse() override;

	// CoolableInterface
	virtual void BeginCool() override;
	virtual void UpdateCool(float _deltaTime) override;
	virtual void EndCool() override;

	UFUNCTION(BlueprintCallable)
		void AddWater();

	UFUNCTION(BlueprintImplementableEvent)
		void OnAddWater();

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
		bool IsOpen();

	UFUNCTION(BlueprintCallable)
		void Toggle();

	UFUNCTION(BlueprintCallable)
		void Open();

	UFUNCTION(BlueprintCallable)
		void Close();

	UFUNCTION(BlueprintImplementableEvent)
		void OnOpen();

	UFUNCTION(BlueprintImplementableEvent)
		void OnClose();

	UFUNCTION(BlueprintCallable)
		bool ContainSomething();

	UFUNCTION(BlueprintCallable)
		bool ContainObjectSpawned();

	UFUNCTION(BlueprintCallable)
		bool ContainLiquid();

	UFUNCTION(BlueprintCallable)
		bool TryAddRecipe(FRecipe _Recipe);

	UFUNCTION()
		void SpawnInteractableFromRecipe(AInteractableActor * spawnedMaterial);

	UFUNCTION()
		void ActualiseAfterSpawnInteractable();

	UFUNCTION()
		bool CanAddRecipe(FRecipe _Recipe);

	UFUNCTION()
		void AddRecipe(FRecipe _Recipe);

	UFUNCTION(BlueprintImplementableEvent)
		void OnAddRecipe();

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<AInteractableActor> m_ClassMaterialToSpawn;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent * m_MeshCubeHeat;

	UPROPERTY(VisibleAnywhere)
		AInteractableActor * m_SpawnedCraftingMaterial;
	
	UFUNCTION(BlueprintCallable)
		bool IsMoldingFinished();

protected:
	virtual AInteractableActor * GetPick(USceneComponent * _toAttach, bool _snapToTarget, bool _disablePhysicsQuery) override;


	virtual void Drop(USceneComponent * _toDetach) override;

private:
	UFUNCTION()
		bool HasGoodInclinaison();

	UPROPERTY(VisibleAnywhere)
		UBoxComponent * m_DeverseLiquidTriggerBox;

	UPROPERTY(VisibleAnywhere)
		USceneComponent * m_PointDeverseLiquid;

	UPROPERTY(VisibleAnywhere)
		TArray<AFoundryBowl*> m_ListFoundryBowls;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AIngot>> m_AllowedIngots;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AWeaponElement>> m_AllowedWeaponElements;

	UPROPERTY(EditAnywhere)
		UMaterialInterface * materialCubeNormal;

	UPROPERTY(EditAnywhere)
		UMaterialInterface * materialCubeHeat;

	UPROPERTY(VisibleAnywhere)
		FRecipe m_ActualRecipe;

	UPROPERTY(VisibleAnywhere)
		bool m_IsOpen = false;

	UPROPERTY(VisibleAnywhere)
		float m_ActualTime = 0;

	UPROPERTY(VisibleAnywhere)
		bool m_HasLiquid = false;

	
};
