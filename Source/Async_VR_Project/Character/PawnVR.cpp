#include "PawnVR.h"
#include "Components/InputComponent.h"

#include "HeadMountedDisplayFunctionLibrary.h"

#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayTypes.h"

#include "Character/MotionControllerVR.h"

#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/Engine.h"

#include "UnrealNetwork.h"

#include "Character/PlayerStateVR.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"



APawnVR::APawnVR()
{
	PrimaryActorTick.bCanEverTick = true;

	VROrigin = CreateDefaultSubobject<USceneComponent>("VROrigin");
	VROrigin->SetupAttachment(RootComponent);

	m_CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	m_CameraComponent->SetupAttachment(VROrigin);

	m_MenuSupport = CreateDefaultSubobject<USceneComponent>("MenuOrigin");
	m_MenuSupport->SetupAttachment(VROrigin);

	m_MenuWidget = CreateDefaultSubobject<UWidgetComponent>("WorldMenu");
	m_MenuWidget->SetupAttachment(m_MenuSupport);

	bReplicates = true;

	//debug
	m_possesed = Possessed::Player;
}

void APawnVR::BeginPlay()
{
	Super::BeginPlay();
	
	SetupPlayerHeight();
	SpawnAndAttachMotionControllers();

	int32 playerIndex = GetWorld()->GetFirstPlayerController() == Cast<APlayerController>(GetController())? 0:1;
	LeftController->SetInputOwner(playerIndex);
	RightController->SetInputOwner(playerIndex);
	LeftController->InitPawnOwner(this);
	RightController->InitPawnOwner(this);

}

void APawnVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateVRMenu(DeltaTime);

	if (m_HasRegisterPlayerStateVR == false)
	{
		m_HasRegisterPlayerStateVR = true;
		APlayerStateVR::RegisterPlayerStateVR(Cast<APlayerStateVR>(GetPlayerState()));
		
	}
}

void APawnVR::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &APawnVR::GrabLeft);
	PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &APawnVR::ReleaseLeft);
	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &APawnVR::GrabRight);
	PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &APawnVR::ReleaseRight);

	PlayerInputComponent->BindAction("ActivateLeft", IE_Pressed, this, &APawnVR::ActivateLeftObject);
	PlayerInputComponent->BindAction("ActivateLeft", IE_Released, this, &APawnVR::DeactivateLeftObject);
	PlayerInputComponent->BindAction("ActivateRight", IE_Pressed, this, &APawnVR::ActivateRightObject);
	PlayerInputComponent->BindAction("ActivateRight", IE_Released, this, &APawnVR::DeactivateRightObject);


	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &APawnVR::ToogleVRMenuVisibility);

	// DEBUG

	PlayerInputComponent->BindAction("DebugLClick", IE_Pressed, this, &APawnVR::DebugGrab);
	PlayerInputComponent->BindAction("DebugLClick", IE_Released, this, &APawnVR::DebugRelease);
	PlayerInputComponent->BindAction("DebugRClick", IE_Pressed, this, &APawnVR::DebugActivate);
	PlayerInputComponent->BindAction("DebugRClick", IE_Released, this, &APawnVR::DebugDeactivate);

	PlayerInputComponent->BindAxis("MoveRight", this, &APawnVR::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &APawnVR::MoveForward);
	
	PlayerInputComponent->BindAxis("LookRight", this, &APawnVR::DebugTurnYaw);
	PlayerInputComponent->BindAxis("LookUp", this, &APawnVR::DebugTurnPitch);
	PlayerInputComponent->BindAxis("Wheel", this, &APawnVR::DebugWheel);

	PlayerInputComponent->BindAction("DebugControlPlayer", IE_Pressed, this, &APawnVR::DebugControlPlayer);
	PlayerInputComponent->BindAction("DebugControlLeft", IE_Pressed, this, &APawnVR::DebugControlLeftHand);
	PlayerInputComponent->BindAction("DebugControlRight", IE_Pressed, this, &APawnVR::DebugControlRightHand);
}

FVector APawnVR::GetCameraLocation()
{
	return m_CameraComponent->GetComponentLocation();
}

void APawnVR::SetupPlayerHeight()
{
	OnSetUpPlayerHeightBP();
	//UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

}

void APawnVR::SpawnAndAttachMotionControllers()
{
	RightController = GetWorld()->SpawnActor<AMotionControllerVR>(PrefabRightController, GetActorTransform());
	LeftController = GetWorld()->SpawnActor<AMotionControllerVR>(PrefabLeftController, GetActorTransform());

	FAttachmentTransformRules rules = FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	RightController->AttachToComponent(VROrigin, rules);
	RightController->SetHand(EHand::Right);
	LeftController->AttachToComponent(VROrigin, rules);
	LeftController->SetHand(EHand::Left);

	APlayerStateVR * playerStateVR = Cast<APlayerStateVR>(GetPlayerState());
	RightController->RegisterPlayerStateVR(playerStateVR);
	LeftController->RegisterPlayerStateVR(playerStateVR);

	if (m_interactionEnabledOnBeginPlay)
		EnableMotionInteractions();
	else
		DisableMotionInteractions();
}

#pragma region Enable/Disable motion interactions

void APawnVR::EnableMotionInteractions()
{
	OnlineEnableInteractions_S();
	OnlineEnableInteractions_M();
}

void APawnVR::DisableMotionInteractions()
{
	OnlineDisableInteractions_S();
	OnlineDisableInteractions_M();
}

void APawnVR::OnlineEnableInteractions_S_Implementation()
{
	RightController->EnableInteractions();
	LeftController->EnableInteractions();
}

bool APawnVR::OnlineEnableInteractions_S_Validate()
{
	return true;
}

void APawnVR::OnlineEnableInteractions_M_Implementation()
{
	RightController->EnableInteractions();
	LeftController->EnableInteractions();
}

void APawnVR::OnlineDisableInteractions_S_Implementation()
{
	RightController->DisableInteractions();
	LeftController->DisableInteractions();
}

bool APawnVR::OnlineDisableInteractions_S_Validate()
{
	return true;
}

void APawnVR::OnlineDisableInteractions_M_Implementation()
{
	RightController->DisableInteractions();
	LeftController->DisableInteractions();
}

bool APawnVR::ToogleReadyState()
{
	APlayerStateVR * playerState = Cast<APlayerStateVR>(GetPlayerState());
	if (playerState == nullptr)
		return false;

	return playerState->ToogleReadyState();
}

void APawnVR::SetReadyState()
{
	APlayerStateVR * playerState = Cast<APlayerStateVR>(GetPlayerState());
	if (playerState == nullptr)
		return;

	playerState->SetReadyState();
}

void APawnVR::SetNotReadyState()
{
	APlayerStateVR * playerState = Cast<APlayerStateVR>(GetPlayerState());
	if (playerState == nullptr)
		return;

	playerState->SetNotReadyState();
}

bool APawnVR::IsReady()
{
	APlayerStateVR * playerState = Cast<APlayerStateVR>(GetPlayerState());
	if (playerState == nullptr)
		return false;

	return playerState->IsReady();
}

void APawnVR::GrabRight()
{
	Grab(RightController);
}

void APawnVR::GrabLeft()
{
	Grab(LeftController);
}

void APawnVR::ReleaseRight()
{
	Release(RightController);
}

void APawnVR::ReleaseLeft()
{
	Release(LeftController);
}

void APawnVR::ActivateRightObject()
{
	ActivateObject(RightController);
}

void APawnVR::ActivateLeftObject()
{
	ActivateObject(LeftController);
}

void APawnVR::DeactivateRightObject()
{
	DeactivateObject(RightController);
}

void APawnVR::DeactivateLeftObject()
{
	DeactivateObject(LeftController);
}

void APawnVR::MoveRight(float _Value)
{
	FVector v = m_CameraComponent->GetRightVector();
	v.Z = 0;
	v.Normalize();
	SetActorLocation(GetActorLocation() + v * m_Speed * _Value);
}

void APawnVR::MoveForward(float _Value)
{
	FVector v = m_CameraComponent->GetForwardVector();
	v.Z = 0;
	v.Normalize();
	SetActorLocation(GetActorLocation() + v * m_Speed * _Value);
}

void APawnVR::Grab(AMotionControllerVR * _MotionController)
{
	if (_MotionController == nullptr)
		return;

	if (MenuOpened())
		_MotionController->ClickOnUI();
	else
		_MotionController->GrabActor();
}

void APawnVR::Release(AMotionControllerVR * _MotionController)
{
	if (_MotionController == nullptr)
		return;

	if (MenuOpened())
		_MotionController->ReleaseOnUI();
	else
		_MotionController->ReleaseActor();
}

void APawnVR::ActivateObject(AMotionControllerVR * _MotionController)
{
	if (_MotionController == nullptr)
		return;

	if (MenuOpened())
		_MotionController->ClickOnUI();
	else
		_MotionController->ActivateGrabbedActor();
}

void APawnVR::DeactivateObject(AMotionControllerVR * _MotionController)
{
	if (_MotionController == nullptr)
		return;

	if (MenuOpened())
		_MotionController->ReleaseOnUI();
	else
		_MotionController->DeactivateGrabbedActor();
}

void APawnVR::DebugGrab()
{
	switch (m_possesed)
	{
	case Possessed::Player:
	case Possessed::LeftHand:
		GrabLeft();
		break;
	case Possessed::RightHand:
		GrabRight();
		break;
	}
}

void APawnVR::DebugRelease()
{
	switch (m_possesed)
	{
	case Possessed::Player:
	case Possessed::LeftHand:
		ReleaseLeft();
		break;
	case Possessed::RightHand:
		ReleaseRight();
		break;
	}
}

void APawnVR::DebugActivate()
{
	switch (m_possesed)
	{
	case Possessed::Player:
		GrabRight();
		break;
	case Possessed::LeftHand:
		ActivateLeftObject();
		break;
	case Possessed::RightHand:
		ActivateRightObject();
		break;
	}
}

void APawnVR::DebugDeactivate()
{
	switch (m_possesed)
	{
	case Possessed::Player:
		ReleaseRight();
		break;
	case Possessed::LeftHand:
		DeactivateLeftObject();
		break;
	case Possessed::RightHand:
		DeactivateRightObject();
		break;
	}
}

void APawnVR::DebugTurnYaw(float _axis)
{
	switch (m_possesed)
	{
	case Possessed::Player:
		m_CameraComponent->SetWorldRotation(m_CameraComponent->GetComponentRotation() + FRotator(0, _axis, 0));
		break;
	case Possessed::LeftHand:
		LeftController->Move(m_CameraComponent->GetRightVector() * _axis);
		break;
	case Possessed::RightHand:
		RightController->Move(m_CameraComponent->GetRightVector() * _axis);
		break;
	}
}

void APawnVR::DebugWheel(float _axis)
{
	_axis *= 8;
	switch (m_possesed)
	{
	case Possessed::Player:
		SetActorLocation(GetActorLocation() + GetActorUpVector() * _axis);
		break;
	case Possessed::LeftHand:
		LeftController->Move(m_CameraComponent->GetForwardVector() * _axis);
		break;
	case Possessed::RightHand:
		RightController->Move(m_CameraComponent->GetForwardVector() * _axis);
		break;
	}
}

void APawnVR::DebugTurnPitch(float _axis)
{
	FRotator rotation = m_CameraComponent->GetComponentRotation();
	_axis *= -1;
	switch (m_possesed)
	{
	case Possessed::Player:
		if(_axis > 0 &&  rotation.Pitch < 75 || _axis < 0 && rotation.Pitch > -75)
			m_CameraComponent->SetWorldRotation(rotation + FRotator(_axis, 0, 0));
		break;
	case Possessed::LeftHand:
		LeftController->MoveUp(_axis);
		break;
	case Possessed::RightHand:
		RightController->MoveUp(_axis);
		break;
	}
}

void APawnVR::DebugControlPlayer()
{
	m_possesed = Possessed::Player;
}

void APawnVR::DebugControlLeftHand()
{
	m_possesed = Possessed::LeftHand;
}

void APawnVR::DebugControlRightHand()
{
	m_possesed = Possessed::RightHand;
}

void APawnVR::UpdateVRMenu(float _deltaTime)
{
	if (!m_MenuWidget->IsVisible())
		return;
	KeepMenuSupportYawOrientationInView(m_YawMax, _deltaTime);
	KeepMenuSupportPitchOrientationInView(m_PitchMax, _deltaTime);
	SetMenuSupportAtCameraHeight();
}

void APawnVR::KeepMenuSupportPitchOrientationInView(float _maxPitch, float _deltaTime)
{
	float camPitch = m_CameraComponent->GetComponentRotation().Pitch;
	FRotator supportRotation = m_MenuSupport->GetComponentRotation();
	if (FMath::Abs(camPitch - supportRotation.Pitch) < _maxPitch)
		return;
	FRotator targetRotation = supportRotation;
	targetRotation.Pitch = camPitch;
	FQuat newRotation = FQuat::FastLerp(supportRotation.Quaternion(), targetRotation.Quaternion(), _deltaTime);
	m_MenuSupport->SetWorldRotation(newRotation);
}

void APawnVR::KeepMenuSupportYawOrientationInView(float _maxYaw, float _deltaTime)
{
	float camYaw = m_CameraComponent->GetComponentRotation().Yaw;
	FRotator supportRotation = m_MenuSupport->GetComponentRotation();
	if (FMath::Abs(camYaw - supportRotation.Yaw) < _maxYaw)
		return;
	FRotator targetRotation = supportRotation;
	targetRotation.Yaw = camYaw;
	FQuat newRotation = FQuat::FastLerp(supportRotation.Quaternion(), targetRotation.Quaternion(), _deltaTime);
	m_MenuSupport->SetWorldRotation(newRotation);
}

void APawnVR::SetMenuSupportAtCameraHeight()
{
	FVector location = m_MenuSupport->GetComponentLocation();
	location.Z = GetCameraLocation().Z;
	m_MenuSupport->SetWorldLocation(location);
}

void APawnVR::ToogleVRMenuVisibility()
{
	if (!m_MenuWidget)
		return;

	ReleaseLeft();
	ReleaseRight();

	if (m_MenuWidget->IsVisible())
		HideVRMenu();
	else
		ShowVRMenu();
}

void APawnVR::HideVRMenu()
{
	m_MenuWidget->SetVisibility(false);
	LeftController->HideLineAndShowHands();
	RightController->HideLineAndShowHands();
}

void APawnVR::ShowVRMenu()
{
	ResetVRMenuOrientation();
	m_MenuWidget->SetVisibility(true);
	LeftController->ShowLineAndHideHands();
	RightController->ShowLineAndHideHands();
}

void APawnVR::ResetVRMenuOrientation()
{
	FRotator newRot = m_CameraComponent->GetComponentRotation();
	newRot.Roll = 0;
	m_MenuSupport->SetWorldRotation(newRot);
}

bool APawnVR::MenuOpened()
{
	return m_MenuWidget->IsVisible();
}
