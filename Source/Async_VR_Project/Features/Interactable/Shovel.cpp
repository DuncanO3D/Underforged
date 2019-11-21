#include "Shovel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"


AShovel::AShovel()
{
	PrimaryActorTick.bCanEverTick = true;
}

AInteractableActor * AShovel::GetPick(USceneComponent * _toAttach)
{
	return Super::GetPick(_toAttach, true, false);
}


void AShovel::SnapTo(USceneComponent * _SceneComponent)
{

	FRotator rot = FRotationMatrix::MakeFromZY(_SceneComponent->GetForwardVector(), -_SceneComponent->GetRightVector()).Rotator();
	SetActorLocationAndRotation(_SceneComponent->GetComponentLocation(), rot);
}