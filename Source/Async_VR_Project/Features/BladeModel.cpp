// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeModel.h"
#include "Components/StaticMeshComponent.h"
#include "Interactable/CraftingMaterial/Ingot.h"


ABladeModel::ABladeModel()
{
	m_root = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	m_root->SetupAttachment(RootComponent);
}

FVector ABladeModel::GetCenterPosition()
{
	return m_center;
}

void ABladeModel::BeginPlay()
{
	m_root->OnComponentBeginOverlap.AddDynamic(this, &ABladeModel::OnOverlapBegin);
}

void ABladeModel::SetupModel(AIngot * _material)
{
	for (int i = 0; i < m_ArrayTransformationsData.Num(); i++)
	{
		if (_material->GetClass() == m_ArrayTransformationsData[i].m_compatibleIngot)
		{
			_material->SetTransformationsData(m_ArrayTransformationsData[i]);
			Destroy();
			return;
		}
	}
}


void ABladeModel::OnOverlapBegin(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComponent, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult)
{
	AIngot * material = Cast<AIngot>(_otherActor);
	if (material)
		SetupModel(material);
}

