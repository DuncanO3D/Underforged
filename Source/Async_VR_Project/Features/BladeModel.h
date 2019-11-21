// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BladeModel.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTransformationStep
{
	GENERATED_BODY()


	FTransformationStep()
	{
		mesh = nullptr;
		hitCountRequired = 1;
	}

	UPROPERTY(EditAnywhere)
		UStaticMesh * mesh;

	UPROPERTY(EditAnywhere)
		int hitCountRequired;

};

USTRUCT(BlueprintType)
struct FTransformationsData
{
	GENERATED_BODY()


	FTransformationsData()
	{
		finalForm = nullptr;
		hitCountFinalRequired = 1;
		m_compatibleIngot = nullptr;
	}

	UPROPERTY(EditAnywhere)
		TArray<FTransformationStep> steps;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeaponElement> finalForm;

	UPROPERTY(EditAnywhere)
		int hitCountFinalRequired;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AIngot> m_compatibleIngot;

};


UCLASS()
class ASYNC_VR_PROJECT_API ABladeModel : public AActor
{
	GENERATED_BODY()

public:

	ABladeModel();

	FVector GetCenterPosition();

protected:

	virtual void BeginPlay() override;

private:

	void SetupModel(class AIngot * _material);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult);

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent * m_root;

	UPROPERTY(EditAnywhere)
		TArray<FTransformationsData> m_ArrayTransformationsData;

	UPROPERTY(EditAnywhere)
		FVector m_center;

};
