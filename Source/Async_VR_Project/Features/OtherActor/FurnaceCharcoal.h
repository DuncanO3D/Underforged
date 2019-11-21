// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/Interface/HeatableInterface.h"
#include "Features/Interface/CoolableInterface.h"
#include "FurnaceCharcoal.generated.h"

class USphereComponent;

UCLASS()
class ASYNC_VR_PROJECT_API AFurnaceCharcoal : public AActor, public IHeatableInterface, public ICoolableInterface
{
	GENERATED_BODY()

private : 
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		USceneComponent * m_scene = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent * m_staticMeshComponent = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		USphereComponent * m_sphereComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
		FVector m_respawnPosition;

public : 
	//feedBack
	UPROPERTY(EditAnywhere, Category = "Material")
		class UMaterialInterface * m_coolMaterial;
	UPROPERTY(EditAnywhere, Category = "Material")
		class UMaterialInterface * m_heatMaterial;

	UPROPERTY(EditAnywhere, Category = "Variables")
		float m_timeToBeHeat = 5.0;
	UPROPERTY(EditAnywhere, Category = "Variables")
		float m_timeToBeCool = 15.0;
	UPROPERTY(EditAnywhere, Category = "Variables")
		float m_timeToDestroy = 30.0;

	UPROPERTY(VisibleAnywhere)
		bool m_isHeating = false;
	UPROPERTY(VisibleAnywhere)
		bool m_isCooling = true;

	//==destroy
	UPROPERTY(VisibleAnywhere, Category = "Variable")
		bool m_isConsume = false;

	UFUNCTION()
		bool Heating();
	UFUNCTION()
		bool Cooling();

	UFUNCTION()
		bool Consuming();

	//Heatable Interface
	virtual void BeginHeat() override;
	virtual void UpdateHeat(float _deltaTime) override;
	virtual void EndHeat() override;

	//Coolable Interface
	virtual void BeginCool() override;
	virtual void UpdateCool(float _deltaTime) override;
	virtual void EndCool() override;

	void HeatingCharcoal();

	void CoolingCharcoal();

	void DecreaseHeatingCharcoal(float _value);

	UFUNCTION()
		void Respawn();

	// Sets default values for this actor's properties
	AFurnaceCharcoal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};