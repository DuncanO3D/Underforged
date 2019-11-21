// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/WeaponElement/WeaponElement.h"

#include "Features/Interface/CoolableInterface.h"
#include "Features/Interface/HeatableInterface.h"

#include "Blade.generated.h"

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API ABlade : public AWeaponElement, public ICoolableInterface, public IHeatableInterface
{
	GENERATED_BODY()
	
public:
	ABlade();


	virtual bool IsCool() override;

protected:
	virtual void BeginPlay() override;
	virtual bool CanGetPick(USceneComponent * _toAttach) override;

public:
	virtual void Tick(float DeltaSeconds) override;

	void SetHot(bool _IsHot);
	void SetTimeHeat(float _TimeHeat);


	virtual void BeginCool() override;
	virtual void UpdateCool(float _deltaTime) override;

	virtual void UpdateHeat(float _deltaTime) override;
	virtual void BeginHeat() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsHot();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetTimeHeat();

	UFUNCTION(BlueprintNativeEvent)
		void OnFinishHeat();
	UFUNCTION(BlueprintNativeEvent)
		void OnFinishCool();

private:
	UFUNCTION()
		void DropIfHand();

	void ActualiseMeshHeat();

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent * m_MeshHeat;

	UPROPERTY(VisibleAnywhere)
		bool m_IsHot = false;
	UPROPERTY(EditAnywhere)
		float m_currentHeatTimer = 0;
	UPROPERTY(EditAnywhere)
		float m_CoolDownTime = 3;
	UPROPERTY(VisibleAnywhere)
		float m_HeatUpTime = 3;
};
