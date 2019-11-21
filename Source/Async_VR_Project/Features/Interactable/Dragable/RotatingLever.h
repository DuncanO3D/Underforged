#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/Interactable/Dragable/Dragable.h"
#include "RotatingLever.generated.h"

UCLASS()
class ASYNC_VR_PROJECT_API ARotatingLever : public ADragable
{
	GENERATED_BODY()

public:
	ARotatingLever();

	UPROPERTY(EditAnywhere)
		class UArrowComponent * m_arrow;
	UPROPERTY(EditAnywhere, Category = "Rotation")
		class USceneComponent * m_HandleStartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		TArray<class AGrindStone*> m_ToRotate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		TArray<AActor *> m_gearing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float m_speedFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float m_pitchValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float m_yawValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float m_rollValue = 0.f;

	UFUNCTION()
		void Drag_Implementations(FVector dragVelocity);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Drag(FVector dragVelocity) override;

	bool test = true;

private:
	UFUNCTION()
		void RotateAllAttachedActor(float angleDeg);
	UFUNCTION()
		float GetActualAngle();
	UFUNCTION()
		float GetSignedAngleDeg(FVector a, FVector b, FVector axis);
	UFUNCTION()
		float GetAngleDeg(FVector a, FVector b);
};
