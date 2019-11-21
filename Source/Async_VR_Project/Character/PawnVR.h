#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Features/PlayerTeleporter.h"
#include "PawnVR.generated.h"

class USceneComponent;
class UWidgetComponent;
class AMotionControllerVR;
class UCameraComponent;

UENUM()
enum class Possessed : uint8
{
	Player,
	LeftHand,
	RightHand,
};

UCLASS()
class ASYNC_VR_PROJECT_API APawnVR : public APawn
{
	GENERATED_BODY()

public:
	APawnVR();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnSetUpPlayerHeightBP();

	FVector GetCameraLocation();

#pragma region Enable/Disable motion interactions

	UFUNCTION(BlueprintCallable)
		void EnableMotionInteractions();
	UFUNCTION(BlueprintCallable)
		void DisableMotionInteractions();

	UFUNCTION(Server, Reliable, WithValidation)
		void OnlineEnableInteractions_S();
	UFUNCTION(NetMulticast, Reliable)
		void OnlineEnableInteractions_M();
	UFUNCTION(Server, Reliable, WithValidation)
		void OnlineDisableInteractions_S();
	UFUNCTION(NetMulticast, Reliable)
		void OnlineDisableInteractions_M();

#pragma endregion

#pragma region Ready

	UFUNCTION(BlueprintCallable)
		bool ToogleReadyState();
	UFUNCTION(BlueprintCallable)
		void SetReadyState();
	UFUNCTION(BlueprintCallable)
		void SetNotReadyState();
	UFUNCTION(BlueprintCallable)
		bool IsReady();

#pragma endregion

protected:

#pragma region VR Initialization

	void SetupPlayerHeight();
	void SpawnAndAttachMotionControllers();

#pragma endregion

#pragma region Player Actions

	UFUNCTION(BlueprintCallable)
		void GrabRight();
	UFUNCTION(BlueprintCallable)
		void GrabLeft();
	UFUNCTION(BlueprintCallable)
		void ReleaseRight();
	UFUNCTION(BlueprintCallable)
		void ReleaseLeft();
	void ActivateRightObject();
	void ActivateLeftObject();
	void DeactivateRightObject();
	void DeactivateLeftObject();

	void Grab(AMotionControllerVR * _MotionController);
	void Release(AMotionControllerVR * _MotionController);
	void ActivateObject(AMotionControllerVR * _MotionController);
	void DeactivateObject(AMotionControllerVR * _MotionController);

	void MoveRight(float _Value);
	void MoveForward(float _Value);

#pragma endregion

#pragma region Debug Player Actions

	void DebugGrab();
	void DebugRelease();
	void DebugActivate();
	void DebugDeactivate();

	void DebugTurnYaw(float _axis);
	void DebugTurnPitch(float _axis);
	void DebugWheel(float _axis);

	void DebugControlPlayer();
	void DebugControlLeftHand();
	void DebugControlRightHand();

#pragma endregion

#pragma region VR Menu Fuctions

	void UpdateVRMenu(float _deltaTime);
	void KeepMenuSupportPitchOrientationInView(float _maxPitch, float _deltaTime);
	void KeepMenuSupportYawOrientationInView(float _maxYaw, float _deltaTime);
	void SetMenuSupportAtCameraHeight();
	UFUNCTION(BlueprintCallable)
		void ToogleVRMenuVisibility();
	UFUNCTION(BlueprintCallable)
		void HideVRMenu();
	UFUNCTION(BlueprintCallable)
		void ShowVRMenu();
	void ResetVRMenuOrientation();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool MenuOpened();

#pragma endregion

#pragma region Properties

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMotionControllerVR> PrefabRightController;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AMotionControllerVR> PrefabLeftController;

	UPROPERTY()
		USceneComponent * VROrigin;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent * m_CameraComponent;
	UPROPERTY(BlueprintReadWrite)
		AMotionControllerVR * LeftController;
	UPROPERTY(BlueprintReadWrite)
		AMotionControllerVR * RightController;
	UPROPERTY(EditAnywhere)
		float m_Speed = 2;

	UPROPERTY(VisibleAnywhere)
		USceneComponent * m_MenuSupport;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UWidgetComponent * m_MenuWidget;

	UPROPERTY(EditAnywhere, meta = (ClampMax = 90, ClampMin = 0, Category = "Menu", DisplayName = "Yaw Max", UIMax = 90, UIMin = 0, Units = deg))
		float m_YawMax = 0;
	UPROPERTY(EditAnywhere, meta = (ClampMax = 90, ClampMin = 0, Category = "Menu", DisplayName = "Pitch Max", UIMax = 90, UIMin = 0, Units = deg))
		float m_PitchMax = 0;

	UPROPERTY()
		bool m_HasRegisterPlayerStateVR = false;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Enable interactions on begin play", ToolTip = "If unchecked, the player needs to wait that the game starts to be allowed to interact"))
		bool m_interactionEnabledOnBeginPlay;

#pragma endregion

	bool m_isReady;

	//Debug
	Possessed m_possesed;


};
