// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/Factory/BlacksmithFactory.h"

#include "DeliveryPoint.generated.h"

class UBoxComponent;
class UTextRenderComponent;

UCLASS()
class ASYNC_VR_PROJECT_API ADeliveryPoint : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Caract")
		USceneComponent * m_scene = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Caract")
		UBoxComponent * m_triggerBox = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Caract")
		UTextRenderComponent * m_orderText = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Caract")
		UTextRenderComponent * m_bladeMatText = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Caract")
		UTextRenderComponent * m_hiltMatText = nullptr;

	UPROPERTY(BlueprintReadWrite)
		int32 m_nbSword;
	UPROPERTY(EditAnywhere)
		int32 m_scoreForSword = 1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		FOrderElement m_orderElement;


	ADeliveryPoint();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
		FText GetNbSwordText();
	UFUNCTION()
		void ScorePoint();
	UFUNCTION(BlueprintImplementableEvent)
		void OnOrderSuccessed();
	UFUNCTION(BlueprintImplementableEvent)
		void OnOrderFailed();

	UFUNCTION(BlueprintImplementableEvent)
		void OnNotSharpened();

	void SetTextOrder(FOrderElement textOrder);

private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Order(APlayerStateVR * playerState);
};