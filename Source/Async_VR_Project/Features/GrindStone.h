#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GrindStone.generated.h"

class APlayerStateVR;

UCLASS()
class ASYNC_VR_PROJECT_API AGrindStone : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrindStone();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent * m_Wheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sharpening")
		TSet<class AFinalProduct*> m_collidingFinalProducts;
	UPROPERTY(EditAnywhere, Category = "Sharpening")
		float m_SharpeningPower;

	UPROPERTY(BlueprintReadWrite)
		bool m_IsRotating;

	UFUNCTION(BlueprintCallable)
		bool TryRotate(float angularSpeed, APlayerStateVR * playerState);

	UFUNCTION()
		void ReplicateRotate(float angularSpeed);
	
	UFUNCTION(BlueprintCallable)
		void OnStartActorOverlap(AActor * overlapedActor);
	UFUNCTION(BlueprintCallable)
		void OnEndActorOverlap(AActor * overlapedActor);

	UPROPERTY(EditAnywhere)
		TArray<AActor *> m_gearing;

private:
	UFUNCTION()
		bool CanRotate(float angularSpeed, APlayerStateVR * playerState);
	UFUNCTION()
		void Rotate(float angularSpeed, APlayerStateVR * playerState);

	
};
