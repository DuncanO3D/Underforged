// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/CraftingMaterial/CraftingMaterial.h"
#include "Features/BladeModel.h"

#include "Features/Interface/CoolableInterface.h"
#include "Features/Interface/HeatableInterface.h"

#include "Ingot.generated.h"



/**
 * 
 */

UCLASS()
class ASYNC_VR_PROJECT_API AIngot : public ACraftingMaterial, public IHeatableInterface, public ICoolableInterface
{
	GENERATED_BODY()
	
public:
	AIngot();

	void SetUp();

	virtual bool IsCool() override;
	virtual bool CanGetPick(USceneComponent * _toAttach) override;

protected:
	virtual void BeginPlay() override;
	virtual void Drop(USceneComponent * _toDetach) override;

	UFUNCTION(BlueprintNativeEvent)
		void OnDropOnAnvil(class AAnvil * _anvil);

public:
	virtual void Tick(float DeltaSeconds) override;

	void SetHotAndTime(bool _IsHot, float _TimeHeat);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool finalState = false;

	void SetTransformationsData(FTransformationsData _data);

	void OnHit(bool _succeeded);

	UFUNCTION(BlueprintImplementableEvent)
		void OnHitFailed();
	UFUNCTION(BlueprintImplementableEvent)
		void OnHitSucceeded();
	UFUNCTION(BlueprintImplementableEvent)
		void OnHitTransformed();
	UFUNCTION(BlueprintImplementableEvent)
		void OnHitFinal();

	UFUNCTION(BlueprintImplementableEvent)
		void OnHitNoModel();

	UFUNCTION(BlueprintImplementableEvent)
		void OnHitWhileCold();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsHot();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetTimeHeat();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHeatMesh(bool _IsHot);

	UFUNCTION(BlueprintNativeEvent)
		void OnFinishHeat();
	UFUNCTION(BlueprintNativeEvent)
		void OnFinishCool();

	virtual void UpdateHeat(float _deltaTime) override;
	virtual void BeginHeat() override;

	virtual void BeginCool() override;
	virtual void UpdateCool(float _deltaTime) override;

private:
	UFUNCTION()
		void DropIfHand();

	void CreateElement();

	void ActualizeHeatMesh();

	UPROPERTY()
		AAnvil * m_snappedAnvil;

	UPROPERTY(VisibleAnywhere)
		FTransformationsData m_transformationsData;

	UPROPERTY(VisibleAnywhere)
		bool m_IsHot = false;
	UPROPERTY(VisibleAnywhere)
		float m_currentHeatTimer = 0;
	UPROPERTY(EditAnywhere)
		float m_CoolDownTime = 3;
	UPROPERTY(EditAnywhere)
		float m_HeatUpTime = 3;

	UPROPERTY(VisibleAnywhere)
		int m_currentStep;
	UPROPERTY(VisibleAnywhere)
		int m_hitCount;

	UPROPERTY(EditAnywhere, meta = (ClampMax = 1.2f, ClampMin = 1.f, DisplayName = "Heat mesh scale", UIMax = 1.2f, UIMin = 1.f, Unit = cm))
		float m_heatMeshScale = 1;

	bool m_HasStart = false;
};
