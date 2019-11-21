// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Order.generated.h"

class UTextRenderComponent;

UCLASS()
class ASYNC_VR_PROJECT_API AOrder : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AOrder();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		bool MakeOrder();
private : 
	UPROPERTY()
		UTextRenderComponent * m_orderText;
};
