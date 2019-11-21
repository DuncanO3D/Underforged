// Fill out your copyright notice in the Description page of Project Settings.

#include "Hilt.h"
#include "Components/BoxComponent.h"

void AHilt::BeginPlay()
{
	Super::BeginPlay();
	m_attachTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHilt::OnOverlapBegin);
}