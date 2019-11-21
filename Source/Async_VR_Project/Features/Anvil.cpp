// Fill out your copyright notice in the Description page of Project Settings.

#include "Anvil.h"
#include "Features/Interactable/CraftingMaterial/Ingot.h"
#include "Features/Interactable/BookBladeModel.h"
#include "Features/BladeModel.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values
AAnvil::AAnvil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_root = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	m_root->SetupAttachment(RootComponent);
	m_setupZone = CreateDefaultSubobject<UBoxComponent>("Setup Zone");
	m_setupZone->SetupAttachment(m_root);

}

// Called when the game starts or when spawned
void AAnvil::BeginPlay()
{
	Super::BeginPlay();
	Free();
}

// Called every frame
void AAnvil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAnvil::SetupBladeModel(ABookBladeModel * _model)
{
	TSubclassOf<ABladeModel> model = _model->GetModel();
	ABladeModel * spawned = GetWorld()->SpawnActor<ABladeModel>(model);
	spawned->SetActorLocationAndRotation(m_setupZone->GetComponentLocation() - spawned->GetCenterPosition(), m_setupZone->GetComponentRotation());
}

bool AAnvil::SetupIngot(AIngot * _ingot)
{
	if (!m_isAvailable)
		return false;

	if (!_ingot->IsHot())
		return false;
	
	for (int i = 0; i < m_ArrayTransformationsData.Num(); i++)
	{
		if (_ingot->GetClass() == m_ArrayTransformationsData[i].m_compatibleIngot)
		{
			_ingot->SetTransformationsData(m_ArrayTransformationsData[i]);
			_ingot->SetActorLocationAndRotation(m_setupZone->GetComponentLocation(), m_setupZone->GetComponentRotation());
			m_isAvailable = false;

			m_ingot = _ingot;

			return true;
		}
	}
	return false;
}

void AAnvil::Free()
{
	m_isAvailable = true;
}

bool AAnvil::IsAvailable()
{
	return m_isAvailable;
}

