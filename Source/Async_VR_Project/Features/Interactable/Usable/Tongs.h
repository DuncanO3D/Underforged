// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/Usable/UsableActor.h"
#include "Tongs.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class ASYNC_VR_PROJECT_API ATongs : public AUsableActor
{
	GENERATED_BODY()

public:

	ATongs();

	bool TryBeginUse() override;
	bool TryBeginUse(AInteractableActor * _interactableActor);

	bool TryStopUse() override;

	bool TransferGrabbedActorTo(class AMotionControllerVR * _hand);
	bool TransferGrabbedActorTo(ATongs * _tong);
	bool TryGetTransferedActor(AInteractableActor * _transferedActor);

	bool HasPickedActor();

	AMotionControllerVR * GetHand();

	virtual bool BeginUse() override;

protected:

	virtual void Tick(float DeltaTime) override;

	//virtual bool CanGetPick(USceneComponent * _toAttach) override;
	virtual AInteractableActor * GetPick(USceneComponent * _toAttach) override;

	//virtual bool CanDrop(USceneComponent * _toDetach) override;
	virtual void Drop(USceneComponent * _toDetach) override;

	virtual void SnapTo(USceneComponent * _SceneComponent) override;

	virtual bool CanBeginUse() override;
	
	virtual bool BeginUse(AInteractableActor * _pickedActor);

	virtual bool CanStopUse() override;
	virtual bool StopUse() override;

	AInteractableActor * GetNearestInteractableActor();
	
private:

	UPROPERTY()
		AInteractableActor * m_pickedActor;

	UPROPERTY()
		AInteractableActor * m_ActorWhoCanBeSelected = nullptr;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent * m_grabCollider;

	UPROPERTY(EditAnywhere)
		FRotator m_snapRotation;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AInteractableActor>> m_incompatibleObjects;

};
