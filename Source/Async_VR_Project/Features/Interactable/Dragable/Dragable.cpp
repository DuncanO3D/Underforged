#include "Dragable.h"

ADragable::ADragable()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	m_PosAtFrameOne = FVector::ZeroVector;
	m_PosAtFrameTwo = FVector::ZeroVector;

	bReplicates = true;
	IsDrag = false;
}

void ADragable::BeginPlay()
{
	Super::BeginPlay();	

	m_PosAtFrameOne = GetActorLocation();
	m_PosAtFrameTwo = m_PosAtFrameOne;
}

AInteractableActor * ADragable::GetPick(USceneComponent * _toAttach)
{
	m_AttachedComponent = _toAttach;

	m_PosAtFrameOne = m_AttachedComponent->GetComponentLocation();
	m_PosAtFrameTwo = m_PosAtFrameOne;

	OnPickUp(_toAttach);
	return this;
}

void ADragable::Drop(USceneComponent * _toDetach)
{
	m_AttachedComponent = nullptr;
	OnDrop(_toDetach);
}

bool ADragable::CanDrag(FVector dragVelocity)
{
	if (m_AttachedComponent != nullptr)
		return true;
	return false;
}

void ADragable::Drag(FVector dragVelocity)
{	
	if (dragVelocity != FVector::ZeroVector)
	{
		SetActorLocation(GetActorLocation() + dragVelocity);
		OnDrag(dragVelocity);
	}
}

bool ADragable::TryDrag(FVector dragVelocity)
{
	if (CanDrag(dragVelocity))
	{
		Drag(dragVelocity);
		return true;
	}
	return false;
}

FVector ADragable::GetDragVelocity()
{
	return m_PosAtFrameTwo - m_PosAtFrameOne;
}

void ADragable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	if (m_AttachedComponent != nullptr)
	{
		m_PosAtFrameOne = m_PosAtFrameTwo;
		m_PosAtFrameTwo = m_AttachedComponent->GetComponentLocation();

		FVector velocity = GetDragVelocity();
		TryDrag(velocity);

		if (velocity != FVector::ZeroVector)
			IsDrag = true;
		else
			IsDrag = false;
	}
}

