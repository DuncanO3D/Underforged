// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnVR_UI.h"
#include "Components/InputComponent.h"
#include "Character/MotionControllerVR.h"
#include "Components/WidgetComponent.h"


APawnVR_UI::APawnVR_UI() : APawnVR::APawnVR()
{

}

void APawnVR_UI::BeginPlay()
{
	Super::BeginPlay();

	m_MenuWidget->SetVisibility(false);
	LeftController->ShowLineAndHideHands();
	RightController->ShowLineAndHideHands();
}

void APawnVR_UI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APawnVR_UI::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &APawnVR_UI::ClickLeft);
	PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &APawnVR_UI::ReleaseLeft);
	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &APawnVR_UI::ClickRight);
	PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &APawnVR_UI::ReleaseRight);

	PlayerInputComponent->BindAction("ActivateLeft", IE_Pressed, this, &APawnVR_UI::ClickLeft);
	PlayerInputComponent->BindAction("ActivateLeft", IE_Released, this, &APawnVR_UI::ReleaseLeft);
	PlayerInputComponent->BindAction("ActivateRight", IE_Pressed, this, &APawnVR_UI::ClickRight);
	PlayerInputComponent->BindAction("ActivateRight", IE_Released, this, &APawnVR_UI::ReleaseRight);

	PlayerInputComponent->BindAction("TeleportLeft", IE_Pressed, this, &APawnVR_UI::ClickLeft);
	PlayerInputComponent->BindAction("TeleportLeft", IE_Released, this, &APawnVR_UI::ReleaseLeft);
	PlayerInputComponent->BindAction("TeleportRight", IE_Pressed, this, &APawnVR_UI::ClickRight);
	PlayerInputComponent->BindAction("TeleportRight", IE_Released, this, &APawnVR_UI::ReleaseRight);

	// DEBUG

	PlayerInputComponent->BindAction("DebugLClick", IE_Pressed, this, &APawnVR_UI::ClickLeft);
	PlayerInputComponent->BindAction("DebugLClick", IE_Released, this, &APawnVR_UI::ReleaseLeft);
	PlayerInputComponent->BindAction("DebugRClick", IE_Pressed, this, &APawnVR_UI::ClickRight);
	PlayerInputComponent->BindAction("DebugRClick", IE_Released, this, &APawnVR_UI::ReleaseRight);

	PlayerInputComponent->BindAxis("MoveRight", this, &APawnVR_UI::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &APawnVR_UI::MoveForward);

	PlayerInputComponent->BindAxis("LookRight", this, &APawnVR_UI::DebugTurnYaw);
	PlayerInputComponent->BindAxis("LookUp", this, &APawnVR_UI::DebugTurnPitch);
	PlayerInputComponent->BindAxis("Wheel", this, &APawnVR_UI::DebugWheel);

	PlayerInputComponent->BindAction("DebugControlPlayer", IE_Pressed, this, &APawnVR_UI::DebugControlPlayer);
	PlayerInputComponent->BindAction("DebugControlLeft", IE_Pressed, this, &APawnVR_UI::DebugControlLeftHand);
	PlayerInputComponent->BindAction("DebugControlRight", IE_Pressed, this, &APawnVR_UI::DebugControlRightHand);

}

void APawnVR_UI::ClickLeft()
{
	Click(LeftController);
}

void APawnVR_UI::ClickRight()
{
	Click(RightController);
}

void APawnVR_UI::ReleaseLeft()
{
	Release(LeftController);
}

void APawnVR_UI::ReleaseRight()
{
	Release(RightController);
}

void APawnVR_UI::Click(AMotionControllerVR * _MotionController)
{
	_MotionController->ClickOnUI();
}

void APawnVR_UI::Release(AMotionControllerVR * _MotionController)
{
	_MotionController->ReleaseOnUI();
}

