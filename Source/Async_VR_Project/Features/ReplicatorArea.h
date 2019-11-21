#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ReplicatorArea.generated.h"

UCLASS()
class ASYNC_VR_PROJECT_API AReplicatorArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AReplicatorArea();

	UFUNCTION(Blueprintcallable)
		void OnTriggerEnter(AActor * toReplicate);

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent * m_root;

protected:
	virtual void BeginPlay() override;
};
