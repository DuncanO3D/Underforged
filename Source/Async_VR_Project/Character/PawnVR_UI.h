// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PawnVR.h"
#include "PawnVR_UI.generated.h"

/**
 * 
 */

UCLASS()
class ASYNC_VR_PROJECT_API APawnVR_UI : public APawnVR
{
	GENERATED_BODY()

public:
	APawnVR_UI();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void ClickLeft();
	void ClickRight();
	void ReleaseLeft();
	void ReleaseRight();

	void Click(class AMotionControllerVR * _MotionController);
	void Release(AMotionControllerVR * _MotionController);
	
};
