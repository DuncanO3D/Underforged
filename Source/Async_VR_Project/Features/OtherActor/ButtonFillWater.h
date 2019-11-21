// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ButtonFillWater.generated.h"

class UStaticMeshComponent;
class AMold;

UCLASS()
class ASYNC_VR_PROJECT_API AButtonFillWater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AButtonFillWater();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void PushButton();

	//UPROPERTY(VisibleAnywhere)
	//	UStaticMeshComponent * m_MeshRoot;

	UPROPERTY(EditAnywhere)
		TArray<AMold*> m_Mold;
};
