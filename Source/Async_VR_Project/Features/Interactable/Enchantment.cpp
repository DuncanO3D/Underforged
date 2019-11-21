#include "Enchantment.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "Features/Interactable/FinalProduct/FinalProduct.h"

#include "Character/PlayerStateVR.h"

#include "Engine/Engine.h"

AEnchantment::AEnchantment()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	m_fx = CreateDefaultSubobject<UParticleSystemComponent>("fx");
	m_fx->SetupAttachment(m_Root);

	m_distanceToEnchante = 500;
	m_pos_FirstFrame;
	m_pos_SecondFrame;
	m_actualDistanceToEnchante = 0;
	m_bIsCollidingWithEnchantable = false;
	m_bCanEnchante = false;
	m_enchantmentTarget = nullptr;

	m_distancePercentage = 90;
	m_directionAngleErrorMarging = 30;

	bReplicates = true;
}

void AEnchantment::BeginPlay()
{
	Super::BeginPlay();	
}

void AEnchantment::OnDrag(FVector value)
{
	if (value != FVector::ZeroVector)
	{
		float Angle_DragVector_to_EnchantDirection = GetSignedAngleRad(value, m_enchanteDirection, FVector::CrossProduct(value, m_enchanteDirection));
		Angle_DragVector_to_EnchantDirection = Angle_DragVector_to_EnchantDirection * 180 / PI;

		//30° is a tweek value for have the direction of the enchantment
		if (Angle_DragVector_to_EnchantDirection < m_directionAngleErrorMarging && Angle_DragVector_to_EnchantDirection > -m_directionAngleErrorMarging)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "The angle is OK");

			m_actualDistanceToEnchante += value.Size();
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "ActualDistanceToEnchante = " + FString::SanitizeFloat(m_actualDistanceToEnchante));

			//0.90 == 90% for the error marging
			if (m_actualDistanceToEnchante >= m_distanceToEnchante * m_distancePercentage / 100)
				m_bCanEnchante = true;
		}
	}
}

FVector AEnchantment::GetDragVector()
{
	FVector Delta = FVector::ZeroVector;

	m_pos_SecondFrame = GetActorLocation();
	Delta = m_pos_SecondFrame - m_pos_FirstFrame;
	m_pos_FirstFrame = m_pos_SecondFrame;

	return Delta;
}

float AEnchantment::GetSignedAngleRad(FVector a, FVector b, FVector axis)
{
	FVector n_cross = FVector::CrossProduct(a, b).GetSafeNormal();
	float angle = GeAngleRad(a, b);

	if (n_cross == axis)
	{
		return angle;
	}
	else
	{
		return -angle;
	}
}

float AEnchantment::GeAngleRad(FVector a, FVector b)
{
	if ((a.Size() * b.Size()) == 0)
		return 0;

	float CosAngle = FVector::DotProduct(a, b) / (a.Size() * b.Size());

	float angle = FMath::Acos(CosAngle);

	return FMath::Abs(angle);
}

AInteractableActor * AEnchantment::GetPick(USceneComponent * _AttachTo)
{
	m_Root->SetSimulatePhysics(false);
	RootComponent->AttachToComponent(_AttachTo, FAttachmentTransformRules::KeepWorldTransform);
	m_AttachedComponent = _AttachTo;
	OnPickUp(_AttachTo);

	m_pos_FirstFrame = GetActorLocation();
	m_pos_SecondFrame = m_pos_FirstFrame;
	m_bCanEnchante = false;

	return this;
}

void AEnchantment::Drop(USceneComponent * _AttachTo)
{
	m_actualDistanceToEnchante = 0;
	m_bIsCollidingWithEnchantable = false;
	m_bCanEnchante = false;

	m_Root->SetSimulatePhysics(true);
	DetachFromActor(FDetachmentTransformRules(FDetachmentTransformRules::KeepWorldTransform));
	m_AttachedComponent = nullptr;
	OnDrop(_AttachTo);
}

void AEnchantment::OnBeginOverlapWithEnchantable(AFinalProduct * enchantable)
{
	if (enchantable != nullptr)
	{
		m_enchantmentTarget = enchantable;
		m_bIsCollidingWithEnchantable = true;
		m_distanceToEnchante = enchantable->m_enchantmentDistance;
		m_enchanteDirection = enchantable->m_enchantmentDirection;
	}
}

void AEnchantment::OnEndOverlapWithEnchantable(AFinalProduct * enchantable)
{
	if (enchantable != nullptr)
	{
		m_actualDistanceToEnchante = 0;
		m_bIsCollidingWithEnchantable = false;
		m_bCanEnchante = false;
		m_enchantmentTarget = nullptr;
	}
}

USceneComponent * AEnchantment::GetAttachedComponent()
{
	return m_AttachedComponent;
}

void AEnchantment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_fx->SetWorldRotation(FRotator(0, 0, 0));

	if (m_enchantmentTarget != nullptr && m_AttachedComponent != nullptr)
	{
		m_enchanteDirection = m_enchantmentTarget->m_enchantmentDirection;
		m_distanceToEnchante = m_enchantmentTarget->m_enchantmentDistance;
		m_distancePercentage = m_enchantmentTarget->m_distancePercentage;
		m_directionAngleErrorMarging = m_enchantmentTarget->m_directionAngleErrorMarging;

		if (m_bIsCollidingWithEnchantable)
		{
			OnDrag(GetDragVector());

			if (m_bCanEnchante)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Can be Enchante");
				
				if (m_enchantmentTarget->CanBeEnchanted(this))
				{

					APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
					if (playerStateVR)
					{
						playerStateVR->EnchantFinalProductOnServ(m_enchantmentTarget, this);
						playerStateVR->EnchantFinalProductOnNetMulticast(m_enchantmentTarget, this);

						//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Enchante");

						playerStateVR->DestroyActorOnServ(this);
						playerStateVR->DestroyActorNetMulticast(this);
					}

				}
			}
		}
	}
}

