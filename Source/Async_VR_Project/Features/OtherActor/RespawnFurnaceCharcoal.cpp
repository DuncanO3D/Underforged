// Fill out your copyright notice in the Description page of Project Settings.

#include "RespawnFurnaceCharcoal.h"
#include "Components/BoxComponent.h"

#include "Features/OtherActor/FurnaceCharcoal.h"
#include "Character/PlayerStateVR.h"

#include "Engine/Engine.h"
#include "Engine/World.h"

// Sets default values
ARespawnFurnaceCharcoal::ARespawnFurnaceCharcoal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_boxSpawn = CreateDefaultSubobject<UBoxComponent>("SpawnBox");
	m_boxSpawn->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARespawnFurnaceCharcoal::BeginPlay()
{
	Super::BeginPlay();

	m_boxSpawn->OnComponentEndOverlap.AddDynamic(this, &ARespawnFurnaceCharcoal::OnBoxOverlapEnd);
}

// Called every frame
void ARespawnFurnaceCharcoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_furnaceCharcoal == nullptr)
		SpawnCharcoal();
}

void ARespawnFurnaceCharcoal::SpawnCharcoal()
{
	m_furnaceCharcoal = GetWorld()->SpawnActor<AFurnaceCharcoal>(m_furnaceCharcoalClass, this->GetActorLocation(), this->GetActorRotation());
}

void ARespawnFurnaceCharcoal::OnBoxOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
	AFurnaceCharcoal * furnaceCharcoal = Cast<AFurnaceCharcoal>(OtherActor);

	//if (playerStateVR)
	//{
	//	playerStateVR->RespawnCharcoalOnServ(this);
	//	playerStateVR->RespawnCharcoalOnNetMulticast(this);
	//}
}
