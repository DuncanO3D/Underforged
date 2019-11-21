#include "RotatingLever.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Features/GrindStone.h"
#include "Character/PlayerStateVR.h"

#include "Kismet/KismetMathLibrary.h"

#include "Engine/Engine.h"

ARotatingLever::ARotatingLever() : ADragable()
{
	m_arrow = CreateDefaultSubobject<UArrowComponent>("Forward");
	m_arrow->SetupAttachment(m_Root);

	m_HandleStartLocation = CreateDefaultSubobject<USceneComponent>("StartHandle");
	m_HandleStartLocation->SetupAttachment(m_Root);

	m_speedFactor = 1;

	bReplicates = true;
}

void ARotatingLever::Drag_Implementations(FVector dragVelocity)
{
	if (dragVelocity != FVector::ZeroVector)
	{
		float angle = GetActualAngle();

		FRotator rotation = FQuat(GetActorForwardVector(), angle).Rotator();

		//GEngine->AddOnScreenDebugMessage(8, 3, FColor::Green, FString::SanitizeFloat(FMath::RadiansToDegrees(angle)));

		AddActorWorldRotation(rotation);

		RotateAllAttachedActor(angle);
		OnDrag(dragVelocity);
	}
}

void ARotatingLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotatingLever::Drag(FVector dragVelocity)
{
	Drag_Implementations(dragVelocity);
}

void ARotatingLever::RotateAllAttachedActor(float angleDeg)
{
	for (int i = 0; i < m_ToRotate.Num(); i++)
	{
		m_ToRotate[i]->TryRotate(angleDeg, m_PlayerStateVR);
	}
	for (int j = 0; j < m_gearing.Num(); j++)
	{
		FRotator NewRotation = FQuat(m_gearing[j]->GetActorUpVector(), angleDeg).Rotator();

		m_gearing[j]->AddActorWorldRotation(NewRotation);
	}
}

float ARotatingLever::GetActualAngle()
{
	if (m_AttachedComponent == nullptr)
		return 0;

	FVector AngleVector = (GetActorUpVector());
	FVector Axis = GetActorForwardVector();
	FVector AttachVector = (m_AttachedComponent->GetComponentLocation() - GetActorLocation());

	AngleVector.Normalize();
	AttachVector.Normalize();
	
	float angleToRotate = GetSignedAngleDeg(AngleVector, AttachVector, Axis);
	
	return angleToRotate;
}

float ARotatingLever::GetSignedAngleDeg(FVector a, FVector b, FVector axis)
{
	a = FVector::VectorPlaneProject(a, axis);
	b = FVector::VectorPlaneProject(b, axis);

	a.Normalize();
	b.Normalize();

	float angle = FQuat::FindBetween(a, b).GetAngle();

	FVector n_cross = FVector::CrossProduct(a, b).GetSafeNormal();
		
	if (FVector::DotProduct(n_cross, axis) < 0)
	{
		angle *= -1;
	}
	
	return angle;
}

float ARotatingLever::GetAngleDeg(FVector a, FVector b)
{
	a.Normalize();
	b.Normalize();

	float denominateur = (a.Size() * b.Size());
	if (denominateur == 0)
		return 0;

	float CosAngle = FVector::DotProduct(a, b) / (a.Size() * b.Size());
	
	CosAngle = FMath::Clamp<float>(CosAngle, -1, 1);

	float angle = FMath::Acos(CosAngle);
	angle = FMath::RadiansToDegrees(angle);

	return FMath::Abs(angle);
}
