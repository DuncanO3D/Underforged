// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/InteractableActor.h"
#include "BookBladeModel.generated.h"

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API ABookBladeModel : public AInteractableActor
{
	GENERATED_BODY()

public:

	ABookBladeModel();

protected:

	//virtual bool CanGetPick(USceneComponent * _toAttach) override;
	virtual AInteractableActor * GetPick(USceneComponent * _toAttach) override;

	//virtual bool CanDrop(USceneComponent * _toDetach) override;
	virtual void Drop(USceneComponent * _toDetach) override;

public:

	UFUNCTION(BlueprintNativeEvent)
		void OnDropOnAnvil(class AAnvil * _anvilOverlapped);
	UFUNCTION(BlueprintImplementableEvent)
		void OnDropOnVoid();

	TSubclassOf<class ABladeModel> GetModel();

private:

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABladeModel> m_bladeModel;
	
};
