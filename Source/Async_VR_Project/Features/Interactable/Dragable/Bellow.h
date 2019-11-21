#pragma once

#include "CoreMinimal.h"
#include "Features/Interactable/Dragable/Dragable.h"
#include "Bellow.generated.h"

class AFurnace;

UCLASS()
class ASYNC_VR_PROJECT_API ABellow : public ADragable
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent * m_bone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_maxHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_minHeight;

	UPROPERTY(EditAnywhere)
		TArray<AFurnace *> m_furnaces;

	ABellow();

	UFUNCTION()
		void Drag_Implementations(FVector dragVelocity);

	 virtual void Select() override;
	 virtual void Deselect() override;

protected:
	virtual void Drag(FVector dragVelocity) override;

};
