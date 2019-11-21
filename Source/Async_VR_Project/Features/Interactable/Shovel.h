#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/InteractableActor.h"
#include "Shovel.generated.h"

class UBoxComponent;

UCLASS()
class ASYNC_VR_PROJECT_API AShovel : public AInteractableActor
{
	GENERATED_BODY()

public : 
	AShovel();

protected:

	//virtual bool CanGetPick(USceneComponent * _toAttach) override;
	virtual AInteractableActor * GetPick(USceneComponent * _toAttach) override;

	virtual void SnapTo(USceneComponent * _SceneComponent) override;

	//virtual bool CanDrop(USceneComponent * _toDetach) override;
	//virtual void Drop(USceneComponent * _toDetach) override;

private:

	UPROPERTY(EditAnywhere)
		FRotator m_snapRotation;
};
