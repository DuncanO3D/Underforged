#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/Dragable/Dragable.h"
#include "UnrealNetwork.h"
#include "LiftingRope.generated.h"

UCLASS()
class ASYNC_VR_PROJECT_API ALiftingRope : public ADragable
{
	GENERATED_BODY()

public:
	ALiftingRope();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
		TArray<AActor*> m_ActorsToLift;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
		float m_MaxHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
		float m_MinHeight;
	
	UFUNCTION()
		void Drag_Implementations(FVector dragVelocity);

protected:
	virtual void Drag(FVector dragVelocity) override;

private:
	UFUNCTION()
		void DragAllActorToLift(FVector dragVelocity);
	
};
