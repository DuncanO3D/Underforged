#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/World.h"

#include "MicroOnde.generated.h"

UCLASS()
class ASYNC_VR_PROJECT_API AMicroOnde : public AActor
{
	GENERATED_BODY()
	
public:	
	AMicroOnde();

	UPROPERTY(EditAnywhere, Replicated)
		AMicroOnde * Output;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		TSet<class AInteractableActor *> InputsActor;

	UFUNCTION(BlueprintCallable)
		void OnTriggerEnter(class AInteractableActor * inputActor);
	UFUNCTION(BlueprintCallable)
		void OnTriggerExit(class AInteractableActor * outputActor);
	UFUNCTION(BlueprintCallable)
		void OnButtonPressed(class AMotionControllerVR * hand);

};
