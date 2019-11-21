#include "GrindStone.h"
#include "Features/Interactable/FinalProduct/FinalProduct.h"

#include "Engine/Engine.h"

#include "Character/PlayerStateVR.h"

AGrindStone::AGrindStone()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Wheel = CreateDefaultSubobject<UStaticMeshComponent>("Wheel");
	m_Wheel->SetupAttachment(RootComponent);

	m_SharpeningPower = 10;

	bReplicates = true;

	m_IsRotating = false;
}

bool AGrindStone::TryRotate(float angularSpeed, APlayerStateVR * playerState)
{
	if (angularSpeed != 0)
		m_IsRotating = true;
	else
		m_IsRotating = false;

	if (CanRotate(angularSpeed, playerState))
	{
		Rotate(angularSpeed, playerState);
		return true;
	}
	return false;
}

void AGrindStone::OnStartActorOverlap(AActor * overlapedActor)
{
	AFinalProduct * finalProduct = Cast<AFinalProduct>(overlapedActor);
	if (finalProduct != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "AddFinalProduct");
		m_collidingFinalProducts.Add(finalProduct);
	}
}

void AGrindStone::OnEndActorOverlap(AActor * overlapedActor)
{
	AFinalProduct * finalProduct = Cast<AFinalProduct>(overlapedActor);
	if (finalProduct != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "RemoveFinalProduct");
		m_collidingFinalProducts.Remove(finalProduct);
	}
}

bool AGrindStone::CanRotate(float angularSpeed, APlayerStateVR * playerState)
{
	return angularSpeed != 0.0f;
}

void AGrindStone::Rotate(float angularSpeed, APlayerStateVR * playerState)
{
	playerState = APlayerStateVR::GetPlayerState(GetWorld());
	if (playerState)
	{
		playerState->RotatingGrindingStoneDragOnServ(this, angularSpeed);
		playerState->RotatingGrindingStoneDragOnNetMulticast(this, angularSpeed);
	}
	
}

void AGrindStone::ReplicateRotate(float angularSpeed)
{
	FQuat rotation = FQuat(GetActorRightVector(), angularSpeed);

	m_Wheel->AddWorldRotation(rotation.Rotator());

	for (int j = 0; j < m_gearing.Num(); j++)
	{
		FRotator NewRotation = FQuat(m_gearing[j]->GetActorUpVector(), angularSpeed).Rotator();
	
		m_gearing[j]->AddActorWorldRotation(NewRotation);
	}

	APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
	if (playerStateVR)
	{
		auto KeyArray = m_collidingFinalProducts.Array();
		for (int i = 0; i < KeyArray.Num(); i++)
		{
			KeyArray[i]->TrySharpened(m_SharpeningPower);
		}
	}
	
}