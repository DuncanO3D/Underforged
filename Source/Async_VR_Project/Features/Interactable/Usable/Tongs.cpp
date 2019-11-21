// Fill out your copyright notice in the Description page of Project Settings.

#include "Tongs.h"
#include "Components/SphereComponent.h"
#include "Character/MotionControllerVR.h"

#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"

ATongs::ATongs()
{
	m_grabCollider = CreateDefaultSubobject<USphereComponent>("Grab Collider");
	m_grabCollider->SetupAttachment(m_Root);
}

bool ATongs::TryBeginUse()
{
	OnBeginUse();
	if (CanBeginUse())
	{
		return BeginUse();
	}
	return false;
}

bool ATongs::TryBeginUse(AInteractableActor * _interactableActor)
{
	if (CanBeginUse())
		return BeginUse(_interactableActor);
	return false;
}

bool ATongs::TryStopUse()
{
	OnStopUse();
	if (CanStopUse())
	{
		return StopUse();
	}
	return false;
}

bool ATongs::TransferGrabbedActorTo(AMotionControllerVR * _hand)
{
	if (!_hand->TryGetTransferedActor(m_pickedActor))
		return false;
	m_pickedActor = nullptr;
	return true;
}

bool ATongs::TransferGrabbedActorTo(ATongs * _tong)
{
	if (!_tong->TryGetTransferedActor(m_pickedActor))
		return false;
	m_pickedActor = nullptr;
	return true;
}

bool ATongs::TryGetTransferedActor(AInteractableActor * _transferedActor)
{
	if (m_pickedActor)
		return false;
	m_pickedActor = _transferedActor;
	return true;
}

void ATongs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_pickedActor == nullptr && m_AttachedComponent)
	{
		AInteractableActor * nearestActor = GetNearestInteractableActor();
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

AInteractableActor * ATongs::GetPick(USceneComponent * _toAttach)
{
	if (m_AttachedComponent != nullptr)
	{
		AMotionControllerVR * motionController = Cast<AMotionControllerVR>(m_AttachedComponent->GetOwner());
		motionController->ActivateGrabbedActor();
	}

	return Super::GetPick(_toAttach, true, true);
}

void ATongs::Drop(USceneComponent * _toDetach)
{
	TryStopUse();
	Super::Drop(_toDetach);
}

void ATongs::SnapTo(USceneComponent * _SceneComponent)
{
	FRotator rot = FRotationMatrix::MakeFromZY(_SceneComponent->GetForwardVector(), _SceneComponent->GetRightVector()).Rotator();
	SetActorLocationAndRotation(_SceneComponent->GetComponentLocation(), rot);
}

bool ATongs::CanBeginUse()
{
	return m_pickedActor == nullptr;
}

bool ATongs::BeginUse()
{
	AInteractableActor * pickedActor = GetNearestInteractableActor();
	return BeginUse(pickedActor);
}

bool ATongs::BeginUse(AInteractableActor * _pickedActor)
{
	if (_pickedActor && _pickedActor->TryGetPick(m_grabCollider)) {
		m_pickedActor = _pickedActor;
		return true;
	}
	return false;
}

bool ATongs::CanStopUse()
{
	return true;
}

bool ATongs::StopUse()
{
	if (m_pickedActor && m_pickedActor->TryDrop(m_grabCollider)) {
		m_pickedActor = nullptr;
		return true;
	}
	return false;
}

AInteractableActor * ATongs::GetNearestInteractableActor()
{
	FVector location = m_grabCollider->GetComponentLocation();
	TArray<AActor*> overlappingActors;
	m_grabCollider->GetOverlappingActors(overlappingActors);
	AInteractableActor * nearestActor = nullptr;
	for (AActor* actor : overlappingActors)
	{
		AInteractableActor * interactable = Cast<AInteractableActor>(actor);
		if (!interactable)
			continue;
		if (interactable == this)
			continue;
		if (!interactable->CanGetPick(m_grabCollider))
			continue;
		if (m_incompatibleObjects.Contains(interactable->GetClass()))
			continue;

		if (!nearestActor)
			nearestActor = interactable;
		else if(FVector::Dist(nearestActor->GetActorLocation(), location) > FVector::Dist(interactable->GetActorLocation(), location))
			nearestActor = interactable;
	}
	return nearestActor;
}

bool ATongs::HasPickedActor()
{
	return m_pickedActor != nullptr && m_pickedActor->GetAttachParentActor() == this;
}

AMotionControllerVR * ATongs::GetHand()
{
	if (!m_AttachedComponent)
		return nullptr;
	return Cast<AMotionControllerVR>(m_AttachedComponent->GetOwner());
}
