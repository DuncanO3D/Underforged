// Fill out your copyright notice in the Description page of Project Settings.

#include "BookBladeModel.h"
#include "Components/StaticMeshComponent.h"
#include "Features/Anvil.h"
#include "Engine/Engine.h"


ABookBladeModel::ABookBladeModel()
{

}

AInteractableActor * ABookBladeModel::GetPick(USceneComponent * _toAttach)
{
	return Super::GetPick(_toAttach, true, false);
}

void ABookBladeModel::Drop(USceneComponent * _toDetach)
{
	TArray<AActor *> overlappingActors;
	m_Root->GetOverlappingActors(overlappingActors);
	AAnvil * anvilTriggered = nullptr;
	for (AActor* actor : overlappingActors) {
		AAnvil * anvil = Cast<AAnvil>(actor);
		if (anvil) {
			anvilTriggered = anvil;
			break;
		}
	}
	if (anvilTriggered)
		OnDropOnAnvil(anvilTriggered);
	else
		OnDropOnVoid();
	Destroy();
}

TSubclassOf<class ABladeModel> ABookBladeModel::GetModel()
{
	return m_bladeModel;
}

void ABookBladeModel::OnDropOnAnvil_Implementation(AAnvil * _anvilOverlapped)
{
	_anvilOverlapped->SetupBladeModel(this);
}
