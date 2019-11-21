// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/BladeModel.h"
#include "Anvil.generated.h"

UCLASS()
class ASYNC_VR_PROJECT_API AAnvil : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnvil();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupBladeModel(class ABookBladeModel * _model);
	bool SetupIngot(class AIngot * _ingot);

	void Free();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsAvailable();

	UPROPERTY(BlueprintReadWrite)
		class AIngot * m_ingot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent * m_root;

private:

	UPROPERTY(VisibleAnywhere)
		bool m_isAvailable;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent * m_setupZone;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Transformations"))
		TArray<FTransformationsData> m_ArrayTransformationsData;

};
