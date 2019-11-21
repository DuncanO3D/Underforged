// Fill out your copyright notice in the Description page of Project Settings.

#include "Hammer.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Features/Interactable/CraftingMaterial/Ingot.h"
#include "Engine/Engine.h"

#include "Character/PlayerStateVR.h"

AHammer::AHammer()
{
	m_HeadCollider = CreateDefaultSubobject<UBoxComponent>("HeadCollider");
	m_HeadCollider->SetupAttachment(m_Root);

	m_lastPosition = GetActorLocation();
}

void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateVelocity(DeltaTime);
}

void AHammer::BeginPlay()
{
	Super::BeginPlay();
	m_HeadCollider->OnComponentBeginOverlap.AddDynamic(this, &AHammer::OnOverlapBegin);
}

AInteractableActor * AHammer::GetPick(USceneComponent * _toAttach)
{
	return Super::GetPick(_toAttach, true, true);
}

void AHammer::SnapTo(USceneComponent * _SceneComponent)
{
	FRotator rot = FRotationMatrix::MakeFromZY(_SceneComponent->GetForwardVector(), _SceneComponent->GetUpVector()).Rotator();
	SetActorLocationAndRotation(_SceneComponent->GetComponentLocation(), rot);
}

void AHammer::UpdateVelocity(float DeltaTime)
{
	FVector newVelocity = FMath::VInterpConstantTo(m_velocity, (m_lastPosition - GetActorLocation()) / DeltaTime, DeltaTime, m_interpSpeed);
	m_lastPosition = GetActorLocation();
	if (newVelocity.Z < 0 && m_velocity.Z >= 0)
		m_startPointHeight = m_lastPosition;
		
	m_velocity = newVelocity;
}

void AHammer::OnOverlapBegin(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComponent, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult)
{
	if (!m_AttachedComponent)
		return;

	AIngot * ingot = Cast<AIngot>(_otherActor);
	if (ingot) {
		float dist = (m_startPointHeight - m_lastPosition).Size();
		bool success = m_velocity.Z > m_requiredSpeed && dist > m_requiredDistance;

		ingot->OnHit(success);

		if (success) {
			m_startPointHeight = m_lastPosition;
			m_velocity = FVector::ZeroVector;
		}
	}
}
