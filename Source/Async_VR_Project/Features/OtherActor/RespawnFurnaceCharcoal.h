// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnFurnaceCharcoal.generated.h"

class UBoxComponent;
class AFurnaceCharcoal;

UCLASS()
class ASYNC_VR_PROJECT_API ARespawnFurnaceCharcoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnFurnaceCharcoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Charcoal")
		TSubclassOf<AFurnaceCharcoal> m_furnaceCharcoalClass;
	UPROPERTY(VisibleAnywhere, Category = "Charcoal")
		AFurnaceCharcoal * m_furnaceCharcoal;
	UPROPERTY(EditAnywhere, Category = "Position")
		FVector m_relativeDefaultRespawnPos;
	UPROPERTY(EditAnywhere, Category = "Position")
		FRotator m_relativeDefautRepawnRot;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnCharcoal();

	UFUNCTION()
		void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private : 
	UPROPERTY(VisibleAnywhere, Category = "Collider")
		UBoxComponent * m_boxSpawn;

};
