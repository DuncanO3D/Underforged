// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/InteractableActor.h"
#include "WeaponElement.generated.h"

class ABlade;
class AHilt;

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API AWeaponElement : public AInteractableActor
{
	GENERATED_BODY()

public:
	AWeaponElement();

	UStaticMesh * GetStaticMesh();
	TArray<UMaterialInterface*> GetMaterials();

	FVector GetCenterPosition();

	void ForceDrop();

protected:

	virtual void BeginPlay() override;

	//virtual bool CanGetPick(USceneComponent * _toAttach);
	//virtual AInteractableActor * GetPick(USceneComponent * _toAttach);

	// True by default
	//virtual bool CanDrop(USceneComponent * _toDetach);
	virtual void Drop(USceneComponent * _toDetach);
	
	void OnDropTryAssemble();

	bool TryAssembleToFinalProduct(AWeaponElement * _OtherWeaponElement);
	bool AssembleToFinalProduct(ABlade * _blade, AHilt * _hilt);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult);

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent * m_attachTrigger;

	UPROPERTY()
		FTimerHandle m_TimerOnDropTryAssembleHandle;

	UPROPERTY(EditAnywhere)
		FVector m_center;

};
