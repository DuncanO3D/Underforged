// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "RespawnItemBoundary.generated.h"

class UBoxComponent;

UCLASS()
class ASYNC_VR_PROJECT_API ARespawnItemBoundary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnItemBoundary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBoundaryOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true), BlueprintReadWrite)
		FVector m_RelativeDefaultRespawnPos;

private:
	UPROPERTY(VisibleAnywhere)
		UBoxComponent * m_BoxBoundary;

	
};
