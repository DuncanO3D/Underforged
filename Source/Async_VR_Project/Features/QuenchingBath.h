// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Features/Interface/CoolableInterface.h"
#include "Features/Interactable/Usable/Mold.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuenchingBath.generated.h"

class UBoxComponent;
class USceneComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundCue;

UCLASS()
class ASYNC_VR_PROJECT_API AQuenchingBath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuenchingBath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
		TArray<TScriptInterface<ICoolableInterface>> m_ListCoolables;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent * m_StaticMesh;

private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		bool HasSomethingInCoolingProcess();

	//TArray<TScriptInterface<ICoolableInterface>> entities;
	   	 
	UPROPERTY(VisibleAnywhere)
		USceneComponent * m_Root;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent * m_TriggerCollider;

	UPROPERTY(VisibleAnywhere, Category = "Feedback")
		UParticleSystemComponent * m_feedback = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Feedback")
		bool m_feedbackOn = false;
	UPROPERTY(VisibleAnywhere, Category = "Feedback")
		UAudioComponent * m_audioComponent = nullptr;

};