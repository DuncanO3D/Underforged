#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class APlayerStateVR;
class APlayerControllerVR;

UCLASS()
class ASYNC_VR_PROJECT_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableActor();

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
		AInteractableActor * TryGetPick(USceneComponent * _toAttach);
	UFUNCTION(BlueprintCallable, BlueprintPure)			// True by default
		virtual bool CanGetPick(USceneComponent * _toAttach);

	UFUNCTION(BlueprintCallable)
		bool TryDrop(USceneComponent * _toDetach);
	UFUNCTION(BlueprintCallable, BlueprintPure)			// True by default
		virtual bool CanDrop(USceneComponent * _toDetach);

	UFUNCTION(BlueprintCallable)
		USceneComponent * GetAttachedComponent();

	UFUNCTION(BlueprintCallable)
		void NoCollision();

	UFUNCTION(BlueprintCallable)
		void PhysicalCollision();

	UFUNCTION()
		void TryRegisterPlayerStateVR(APlayerStateVR * playerStateVR);

	UFUNCTION(BlueprintCallable)
		virtual void Select();
	UFUNCTION(BlueprintCallable)
		virtual void Deselect();
	
	UFUNCTION(BlueprintImplementableEvent)
		void EventSelect();

	UFUNCTION(BlueprintImplementableEvent)
		void EventDeselect();

	UFUNCTION()
		APlayerStateVR * GetPlayerState();

	UFUNCTION()
		void RegisterRespawnPosition();

	UFUNCTION()
		void RegisterRespawnPositionForRpc(FVector newRespawnPosition);

	UFUNCTION(BlueprintCallable)
		void Respawn();


protected:
	virtual AInteractableActor * GetPick(USceneComponent * _toAttach); 
	virtual AInteractableActor * GetPick(USceneComponent * _toAttach, bool _snapToTarget, bool _disablePhysicsQuery);

	virtual void Drop(USceneComponent * _toDetach);
	
	virtual void SnapTo(USceneComponent * _SceneComponent);

	UFUNCTION(BlueprintImplementableEvent)
		void OnPickUp(USceneComponent * _toAttach);
	UFUNCTION(BlueprintImplementableEvent)
		void OnDrop(USceneComponent * _toDetach);

	bool TryTransferFromGrabbingHand(USceneComponent * _toAttach);
	bool TryTransferFromHoldingTong(USceneComponent * _toAttach);

	/*** PROPERTY ***/

	UPROPERTY()
		int m_NumberOfSelection = 0;

	UPROPERTY()
		APlayerStateVR * m_PlayerStateVR;
	UPROPERTY()
		APlayerControllerVR * m_PlayerControllerVR;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent * m_Root;

	UPROPERTY(BlueprintReadWrite)
		USceneComponent * m_AttachedComponent;

	UPROPERTY(VisibleAnywhere)
		FVector m_RespawnPosition;

	UPROPERTY()
		float updateTimer = 0;

private:

#pragma region TextHeader

	void TextHeaderLookAtPlayer();

	FVector GetNearestPlayerCameraLocation();

	float m_timerDisappear;
	UPROPERTY(EditAnywhere, meta = (ClampMax = 5, ClampMin = 0, DisplayName = "Disappear time", UIMax = 5, UIMin = 0, Units = s))
		float m_disappearTime = 1.f;

	FVector m_lastCamLocation;

	UPROPERTY(VisibleAnywhere)
		class UTextRenderComponent * m_textHeader;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent * m_textRoot;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Display Name"))
		FString m_name;

#pragma endregion

};
