// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/InteractableActor.h"
#include "Book.generated.h"

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API ABook : public AInteractableActor
{
	GENERATED_BODY()

public:

	ABook();

protected:

	virtual void BeginPlay() override;

	//virtual bool CanGetPick(USceneComponent * _toAttach) override;
	virtual AInteractableActor * GetPick(USceneComponent * _toAttach, bool _snapToTarget, bool _disablePhysicsQuery) override;

	//virtual bool CanDrop(USceneComponent * _toDetach) override;
	//virtual void Drop(USceneComponent * _toDetach) override;

private:

	AInteractableActor * SpawnModel(FVector _position);
	FVector ToLocalPosition(FVector _worldPosition);

	void FlipLeftPage();
	void FlipRightPage();

	int GetMaxPagesCount();

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AInteractableActor>> m_models;
	UPROPERTY(EditAnywhere)
		float m_middleMargin;
	
	UPROPERTY(VisibleAnywhere)
		int m_currentPage;


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex);

	UPROPERTY(EditAnywhere, meta = (ClampMax = 50, ClampMin = 0, DisplayName = "Flip distance", ToolTip = "Minimum distance to travel required to flip a page", UIMax = 50, UIMin = 0, Unit = cm))
		float m_flipDistance;

	FVector m_handOverlapStartPoint;

};
