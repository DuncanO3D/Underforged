// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/InteractableActor.h"
#include "Hammer.generated.h"

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API AHammer : public AInteractableActor
{
	GENERATED_BODY()

public:

	AHammer();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	//virtual bool CanGetPick(USceneComponent * _toAttach) override;
	virtual AInteractableActor * GetPick(USceneComponent * _toAttach) override;

	virtual void SnapTo(USceneComponent * _SceneComponent) override;

	//virtual bool CanDrop(USceneComponent * _toDetach) override;
	//virtual void Drop(USceneComponent * _toDetach) override;

private:

	void UpdateVelocity(float DeltaTime);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult);

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent * m_HeadCollider;

	FVector m_velocity;
	FVector m_lastPosition;

	FVector m_startPointHeight;

	UPROPERTY(EditAnywhere)
		float m_interpSpeed;
	UPROPERTY(EditAnywhere)
		float m_requiredSpeed;
	UPROPERTY(EditAnywhere)
		float m_requiredDistance;


	UPROPERTY(EditAnywhere)
		FRotator m_snapRotation;

};
