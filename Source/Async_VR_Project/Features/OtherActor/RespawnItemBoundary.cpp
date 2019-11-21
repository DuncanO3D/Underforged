// Fill out your copyright notice in the Description page of Project Settings.

#include "RespawnItemBoundary.h"

#include "Components/BoxComponent.h"
#include "Features/Interactable/InteractableActor.h"

#include "Character/PlayerStateVR.h"

#include "Engine/Engine.h"
#include "Features/Interactable/FoundryBowl.h"

// Sets default values
ARespawnItemBoundary::ARespawnItemBoundary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_BoxBoundary = CreateDefaultSubobject<UBoxComponent>("BoxBoundary");
	m_BoxBoundary->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARespawnItemBoundary::BeginPlay()
{
	Super::BeginPlay();
	
	m_BoxBoundary->OnComponentEndOverlap.AddDynamic(this, &ARespawnItemBoundary::OnBoundaryOverlapEnd);

	m_RelativeDefaultRespawnPos += GetActorLocation();
}

// Called every frame
void ARespawnItemBoundary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARespawnItemBoundary::OnBoundaryOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, OtherComp->GetName());
	AInteractableActor * interactableActor = Cast<AInteractableActor>(OtherActor);
	if (interactableActor)
	{
		APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
		if (playerStateVR)
		{
			playerStateVR->RespawnInteractableActorOnServ(interactableActor);
			playerStateVR->RespawnInteractableActorOnNetMulticast(interactableActor);
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, "RespawnItemOutOfBoundary");
		}
		
	}
}

