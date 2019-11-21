#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/Interface/HeatableInterface.h"
#include "Furnace.generated.h"

class UBoxComponent;
class UParticleSystemComponent;

UCLASS()
class ASYNC_VR_PROJECT_API AFurnace : public AActor, public IHeatableInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		USceneComponent * m_scene = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent * m_staticMeshComponent = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UParticleSystemComponent * m_flameParticle = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UBoxComponent * m_boxComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
		float m_flameFactor = 0.0f;
	UPROPERTY(EditAnywhere)
		float m_minFlame = 0;
	UPROPERTY(EditAnywhere)
		float m_maxFlame = 2;
	UPROPERTY()
		float m_maxDecreaseTime = 20.0;
	UPROPERTY()
		FVector m_flameParticlesScale;

	UPROPERTY(BlueprintReadWrite)
		bool m_isHeating = false;

	UPROPERTY(EditAnywhere)
		bool m_infiniteFlame = false;
	
	FTimerHandle m_menberTimerHandle;

	AFurnace();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool CanHeat();

	void IncreaseFlame(float _value);

	UFUNCTION(BlueprintCallable)
		void DecreaseFlame(float _decreaseTimer);

	UPROPERTY(BlueprintReadWrite)
		TArray<TScriptInterface<IHeatableInterface>> m_listHeatables;

private:

	UPROPERTY(EditAnywhere)
		float m_IncreaseRate = 0.05;

	UPROPERTY(EditAnywhere)
		float m_DecreaseRate = 0.01;



private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
