#include "MotionControllerVR.h"

#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Features/Interactable/InteractableActor.h"
#include "SteamVRChaperoneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "MotionControllerComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Features/Interactable/Usable/Tongs.h"
#include "Features/Interactable/Usable/UsableActor.h"
#include "Components/LineBatchComponent.h"
#include "Engine/Engine.h"

#include "Character/PlayerStateVR.h"
#include "Character/PawnVR.h"

#include "Kismet/GameplayStatics.h"

AMotionControllerVR::AMotionControllerVR()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	m_Scene->SetupAttachment(RootComponent);
	m_MotionController = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");
	m_MotionController->SetupAttachment(m_Scene);
	//m_HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandMesh");
	//m_HandMesh->SetupAttachment(m_MotionController);
	m_ControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>("ControllerMesh");
	m_ControllerMesh->SetupAttachment(m_MotionController);
	m_GrabSphere = CreateDefaultSubobject<USphereComponent>("GrabSphere");
	m_GrabSphere->SetupAttachment(m_MotionController);

	m_SteamVRChaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>("SteamVRChaperone");

	m_WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>("Pointer");
	m_WidgetInteraction->SetupAttachment(m_GrabSphere);
}

void AMotionControllerVR::BeginPlay()
{
	Super::BeginPlay();
}

void AMotionControllerVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_CursorActive)
		DrawInteractionLine();

	if (m_AttachedActor == nullptr)
	{
		AInteractableActor * nearestActor = GetNearestActor();
		if (nearestActor != m_ActorWhoCanBeSelected)
		{
			if (m_ActorWhoCanBeSelected)
				m_ActorWhoCanBeSelected->Deselect();
			if (nearestActor)
				nearestActor->Select();

			m_ActorWhoCanBeSelected = nearestActor;
		}
	}
	else
	{
		if (m_ActorWhoCanBeSelected)
			m_ActorWhoCanBeSelected->Deselect();
		m_ActorWhoCanBeSelected = nullptr;
	}

}

void AMotionControllerVR::RumbleController(float _Intensity)
{
	if(m_rumbleEffect)
		GetWorld()->GetFirstPlayerController()->PlayHapticEffect(m_rumbleEffect, m_ControllerHand, FMath::Clamp<float>(_Intensity, 0, 1));
}

void AMotionControllerVR::GrabActor()
{
	ATongs * tong = Cast<ATongs>(m_AttachedActor);
	if (tong)
	{
		if (!tong->HasPickedActor())
		{
			tong->TryBeginUse();
		}
	}
	else
	{
		AInteractableActor * interactableActor = GetNearestActor();
		GrabActor(interactableActor);
	}
}

void AMotionControllerVR::GrabActor(AInteractableActor * interactableActorToGrab)
{
	if (!m_interactionEnabled || m_AttachedActor)
		return;

	if (interactableActorToGrab)
	{
		AInteractableActor * grabbedActor = interactableActorToGrab->TryGetPick(m_GrabSphere);
		m_HandOpen = false;
		if (grabbedActor)
		{
			m_AttachedActor = grabbedActor;
				
			RumbleController(.7f);
		}
	}
	else
		m_HandOpen = false;
}

void AMotionControllerVR::ReleaseActor()
{
	ATongs * tong = Cast<ATongs>(m_AttachedActor);
	if (tong)
	{
		tong->TryStopUse();
		//if (tong->HasPickedActor())
		//{
		//	
		//}
	}
	else
	{
		if (m_AttachedActor != nullptr)
		{
			if (m_AttachedActor->TryDrop(m_GrabSphere))
			{
				m_HandOpen = true;
				m_AttachedActor = nullptr;
			}
		}
		else
			m_HandOpen = true;
	}
}

bool AMotionControllerVR::TransferGrabbedActorTo(AMotionControllerVR * _hand)
{
	if(!_hand->TryGetTransferedActor(m_AttachedActor))
		return false;
	m_AttachedActor = nullptr;
	return true;
}

bool AMotionControllerVR::TransferGrabbedActorTo(ATongs * _tong)
{
	if (!_tong->TryGetTransferedActor(m_AttachedActor))
		return false;
	m_AttachedActor = nullptr;
	return true;
}

bool AMotionControllerVR::TryGetTransferedActor(AInteractableActor * _transferedActor)
{
	if (m_AttachedActor)
		return false;
	m_AttachedActor = _transferedActor;
	return true;
}

void AMotionControllerVR::ActivateGrabbedActor()
{
	if (m_AttachedActor != nullptr)
	{
		ATongs * tongs = Cast<ATongs>(m_AttachedActor);
		if (tongs)
		{
			tongs->TryStopUse();
			if (tongs->TryDrop(m_GrabSphere))
			{
				m_HandOpen = true;
				m_AttachedActor = nullptr;
			}
		}
	}
}

void AMotionControllerVR::DeactivateGrabbedActor()
{
}

bool AMotionControllerVR::HasSomethingInHand()
{
	return m_AttachedActor != nullptr;
}

bool AMotionControllerVR::IsHandOpen()
{
	return m_HandOpen;
}

bool AMotionControllerVR::CanTakeObject()
{
	return m_ActorWhoCanBeSelected != nullptr;
}

void AMotionControllerVR::SetHand(EHand _Hand)
{
	if (_Hand == EHand::Left)
	{
		m_ControllerHand = EControllerHand::Left;
		m_MotionController->SetTrackingMotionSource(FName("Left"));
		//m_HandMesh->SetWorldScale3D(FVector(1, 1, -1));
		m_WidgetInteraction->PointerIndex = 1;
	}
	else if (_Hand == EHand::Right)
	{
		m_ControllerHand = EControllerHand::Right;
		m_MotionController->SetTrackingMotionSource(FName("Right"));
		m_WidgetInteraction->PointerIndex = 2;
	}

}

UMotionControllerComponent * AMotionControllerVR::GetMotionController()
{
	return m_MotionController;
}

FRotator AMotionControllerVR::GetInitialControllerRotation()
{
	return m_InitialControllerRotation;
}

void AMotionControllerVR::SetInitialControllerRotation(FRotator _Rotation)
{
	m_InitialControllerRotation = _Rotation;
}

void AMotionControllerVR::SetInputOwner(int32 _playerIndex)
{
	m_MotionController->PlayerIndex = _playerIndex;
}

EControllerHand AMotionControllerVR::GetControllerHand()
{
	return m_ControllerHand;
}

void AMotionControllerVR::RegisterPlayerStateVR(APlayerStateVR * playerStateVR)
{
	m_PlayerStateVR = playerStateVR;
}

void AMotionControllerVR::ClickOnUI()
{
	m_WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
	m_clickActive = true;
}

void AMotionControllerVR::ReleaseOnUI()
{
	m_WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
	m_clickActive = false;
}

void AMotionControllerVR::ShowLineAndHideHands()
{
	m_CursorActive = true;
	//m_HandMesh->SetVisibility(false);
}

void AMotionControllerVR::HideLineAndShowHands()
{
	m_CursorActive = false;
	//m_HandMesh->SetVisibility(true);
}

void AMotionControllerVR::InitPawnOwner(APawnVR * _pawn)
{
	if (!m_owningPawn)
		m_owningPawn = _pawn;
}

APawnVR * AMotionControllerVR::GetOwningPawn()
{
	return m_owningPawn;
}

void AMotionControllerVR::DrawInteractionLine()
{
	FVector start = m_WidgetInteraction->GetComponentLocation();
	auto hit = m_WidgetInteraction->GetLastHitResult();
	FVector end = hit.ImpactPoint;
	if (hit.Distance == 0)
		end = start + m_WidgetInteraction->GetForwardVector() * m_WidgetInteraction->InteractionDistance;
	GetWorld()->LineBatcher->DrawLine(start, end, m_cursorColor, SDPG_World, m_cursorThickness * (m_clickActive?2:1));
}

bool AMotionControllerVR::IsHoldingAnObject()
{
	return m_AttachedActor != nullptr;
}

void AMotionControllerVR::EnableInteractions()
{
	m_interactionEnabled = true;
	m_GrabSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMotionControllerVR::DisableInteractions()
{
	ReleaseActor();
	m_interactionEnabled = false;
	m_GrabSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMotionControllerVR::MoveUp(float _axis)
{
	FVector direction = m_MotionController->GetUpVector();
	m_MotionController->SetWorldLocation(m_MotionController->GetComponentLocation() + direction * _axis);
}

void AMotionControllerVR::Move(FVector _direction)
{
	m_MotionController->SetWorldLocation(m_MotionController->GetComponentLocation() + _direction);
}


void AMotionControllerVR::SetupRoomScaleOutline()
{

}


AInteractableActor * AMotionControllerVR::GetNearestActor()
{
	TArray<AActor*> m_Tab;
	m_GrabSphere->GetOverlappingActors(m_Tab);
	AInteractableActor * nearestActor = nullptr;

	for (AActor * actor : m_Tab)
	{
		AInteractableActor * interactableActor = Cast<AInteractableActor>(actor);
		if (interactableActor && interactableActor->CanGetPick(m_GrabSphere))
		{
			if (nearestActor == nullptr)
				nearestActor = interactableActor;
			else if (FVector::Distance(m_GrabSphere->GetComponentLocation(), nearestActor->GetActorLocation()) > FVector::Distance(m_GrabSphere->GetComponentLocation(), interactableActor->GetActorLocation()))
			{
				nearestActor = interactableActor;
			}
		}
	}


	return nearestActor;
}