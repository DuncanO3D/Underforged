// Fill out your copyright notice in the Description page of Project Settings.

#include "Order.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"

#include "Features/Factory/BlacksmithFactory.h"
#include "GameCore/OnlineGameInstance.h"

#include "Features/Interactable/WeaponElement/Blade.h"
#include "Features/Interactable/WeaponElement/Hilt.h"
#include "Features/Interactable/FinalProduct/FinalProduct.h"


// Sets default values
AOrder::AOrder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_orderText = CreateDefaultSubobject<UTextRenderComponent>("TEXT");

}

// Called when the game starts or when spawned
void AOrder::BeginPlay()
{
	Super::BeginPlay();
	MakeOrder();
}

// Called every frame
void AOrder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AOrder::MakeOrder()
{
	UOnlineGameInstance * gameInstance = UOnlineGameInstance::GetInstance(GetWorld());
	if (gameInstance == nullptr)
		return false;

	FOrderElement orderElement = gameInstance->GetBlacksmithFactory()->GetRandomOrder();
	return true;
}
