#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputCoreTypes.h"
#include "MotionControllerVR.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AInteractableActor;
class USteamVRChaperoneComponent;
class UMotionControllerComponent;
class USkeletalMeshComponent;
class USplineComponent;
class USplineMeshComponent;
class UWidgetInteractionComponent;
class APlayerStateVR;

UENUM(BlueprintType)
enum class EHand : uint8
{
	Right,
	Left
};

UCLASS()
class ASYNC_VR_PROJECT_API AMotionControllerVR : public AActor
{
	GENERATED_BODY()
	
public:	
	AMotionControllerVR();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void RumbleController(float _Intensity);

	void GrabActor();
	void GrabActor(AInteractableActor * interactableActorToGrab);
	void ReleaseActor();
	bool TransferGrabbedActorTo(AMotionControllerVR * _hand);
	bool TransferGrabbedActorTo(class ATongs * _tong);
	bool TryGetTransferedActor(AInteractableActor * _transferedActor);

	void ActivateGrabbedActor();
	void DeactivateGrabbedActor();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool HasSomethingInHand();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsHandOpen();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool CanTakeObject();

	void SetHand(EHand _Hand);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		UMotionControllerComponent * GetMotionController();

	FRotator GetInitialControllerRotation();
	void SetInitialControllerRotation(FRotator _Rotation);

	void SetInputOwner(int32 _playerIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		EControllerHand GetControllerHand();

	void RegisterPlayerStateVR(APlayerStateVR * playerStateVR);

	void ClickOnUI();
	void ReleaseOnUI();
	void ShowLineAndHideHands();
	void HideLineAndShowHands();

	void DrawInteractionLine();

	bool IsHoldingAnObject();

	void EnableInteractions();
	void DisableInteractions();


	void InitPawnOwner(class APawnVR * _pawn);
	class APawnVR * GetOwningPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AInteractableActor * m_AttachedActor = nullptr;

#pragma region Debug

	void MoveUp(float _axis);
	void Move(FVector _direction);

#pragma endregion


private:

	void SetupRoomScaleOutline();

	AInteractableActor * GetNearestActor();

	EControllerHand m_ControllerHand;

	FRotator m_InitialControllerRotation;

	bool m_clickActive;

	UPROPERTY()
		class APawnVR * m_owningPawn;

	UPROPERTY()
		TArray<USplineMeshComponent*> m_SplineMeshes;


	
	UPROPERTY()
		AInteractableActor * m_ActorWhoCanBeSelected = nullptr;

	UPROPERTY(VisibleAnywhere)
		USceneComponent * m_Scene = nullptr;
	UPROPERTY(VisibleAnywhere)
		UMotionControllerComponent * m_MotionController = nullptr;
	UPROPERTY(VisibleAnywhere)
		USphereComponent * m_GrabSphere = nullptr;
	//UPROPERTY(VisibleAnywhere)
	//	USkeletalMeshComponent * m_HandMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent * m_ControllerMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
		USteamVRChaperoneComponent * m_SteamVRChaperone = nullptr;
	UPROPERTY(VisibleAnywhere)
		UWidgetInteractionComponent * m_WidgetInteraction;

	UPROPERTY(EditAnywhere, meta = (ClampMax = 2, ClampMin = 0, DisplayName = "Cursor line thickness", UIMax = 2, UIMin = 0))
		float m_cursorThickness;

	UPROPERTY(EditAnywhere)
		FLinearColor m_cursorColor;

	UPROPERTY(EditAnywhere)
		class UHapticFeedbackEffect_Base * m_rumbleEffect;

	UPROPERTY(VisibleAnywhere)
		bool m_HandOpen = true;

	UPROPERTY()
		APlayerStateVR * m_PlayerStateVR;

	bool m_CursorActive = false;

	bool m_interactionEnabled;
};