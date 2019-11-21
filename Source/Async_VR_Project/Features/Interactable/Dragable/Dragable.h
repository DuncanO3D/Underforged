#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/Interactable/InteractableActor.h"
#include "Dragable.generated.h"

UCLASS()
class ASYNC_VR_PROJECT_API ADragable : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	ADragable();

	UFUNCTION(BlueprintCallable)
		bool TryDrag(FVector dragVelocity);
	UFUNCTION(BlueprintCallable)
		FVector GetDragVelocity();

	UPROPERTY(BlueprintReadWrite)
		bool IsDrag;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual AInteractableActor * GetPick(USceneComponent * _toAttach) override;

	virtual void Drop(USceneComponent * _toDetach) override;

	virtual bool CanDrag(FVector dragVelocity);
	virtual void Drag(FVector dragVelocity);

	UFUNCTION(BlueprintImplementableEvent)
		void OnDrag(FVector dragVelocity);

private:	
	FVector m_PosAtFrameOne;
	FVector m_PosAtFrameTwo;
};
