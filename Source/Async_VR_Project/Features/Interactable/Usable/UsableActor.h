// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/InteractableActor.h"
#include "UsableActor.generated.h"

/**
 * 
 */
UCLASS()
class ASYNC_VR_PROJECT_API AUsableActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		virtual bool TryBeginUse();

	UFUNCTION(BlueprintCallable)
		virtual bool TryStopUse();

	// False by default
	UFUNCTION(BlueprintCallable)
		bool IsInUse();

protected:
	// True by default
	virtual bool CanBeginUse();
	// Nothing by default
	virtual bool BeginUse();

	// True by default
	virtual bool CanStopUse();
	// Nothing by default
	virtual bool StopUse();

	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginUse();
	UFUNCTION(BlueprintImplementableEvent)
		void OnStopUse();

};
