#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Features/Factory/BlacksmithFactory.h"
#include "Features/Recipe.h"
#include "Engine/World.h"

#include "PlayerStateVR.generated.h"

class ALiftingRope;
class ARotatingLever;
class AGrindStone;
class ABellow;
class AIngot;
class ARespawnFurnaceCharcoal;
class AFinalProduct;
class AEnchantment;
class AFoundryBowl;
class ACraftingMaterial;
class AMold;
class AInteractableActor;
class ADeliveryPoint;

UCLASS()
class ASYNC_VR_PROJECT_API APlayerStateVR : public APlayerState
{
	GENERATED_BODY()
	
public:

	APlayerStateVR();

	UFUNCTION(Server, Reliable, WithValidation)
		void DestroyActorOnServ(AActor * actor);
	UFUNCTION(NetMulticast, Reliable)
		void DestroyActorNetMulticast(AActor * actor);

	//Respawn Interactable Actor
	UFUNCTION(Server, Reliable, WithValidation)
		void RespawnInteractableActorOnServ(AInteractableActor * interactable);
	UFUNCTION(NetMulticast, Reliable)
		void RespawnInteractableActorOnNetMulticast(AInteractableActor * interactable);

	//LiftingRope
	UFUNCTION(Server, Reliable, WithValidation)
		void LiftingRopeDragOnServ(ALiftingRope * liftingRope, FVector dragVelocity);
	UFUNCTION(NetMulticast, Reliable)
		void LiftingRopeDragOnNetMulticast(ALiftingRope * liftingRope, FVector dragVelocity);
	
	//Rotatinglever
	UFUNCTION(Server, Reliable, WithValidation)
		void RotatingLeverDragOnServ(ARotatingLever * rotatingLever, FVector dragVelocity);
	UFUNCTION(NetMulticast, Reliable)
		void RotatingLeverDragOnNetMulticast(ARotatingLever * rotatingLever, FVector dragVelocity);
	
	//GrindingStone
	UFUNCTION(Server, Reliable, WithValidation)
		void RotatingGrindingStoneDragOnServ(AGrindStone * grindingStone, float angularSpeed);
	UFUNCTION(NetMulticast, Reliable)
		void RotatingGrindingStoneDragOnNetMulticast(AGrindStone * grindingStone, float angularSpeed);
	
	//Bellow
	UFUNCTION(Server, Reliable, WithValidation)
		void BelowDragOnServ(ABellow * below, FVector dragVelocity);
	UFUNCTION(NetMulticast, Reliable)
		void BelowDragOnNetMulticast(ABellow * below, FVector dragVelocity);
	
	// Final product
	UFUNCTION(Server, Reliable, WithValidation)
		void FinalProductTrySharpenedOnServ(AFinalProduct * finalProduct, float _SharpeningPower);
	UFUNCTION(NetMulticast, Reliable)
		void FinalProductTrySharpenedOnNetMulticast(AFinalProduct * finalProduct, float _SharpeningPower);

	//FinalProduct / Enchant
	UFUNCTION(Server, Reliable, WithValidation)
		void EnchantFinalProductOnServ(AFinalProduct * _FinalProduct, AEnchantment * _Enchantement);
	UFUNCTION(NetMulticast, Reliable)
		void EnchantFinalProductOnNetMulticast(AFinalProduct * _FinalProduct, AEnchantment * _Enchantement);

	//Ingot
	UFUNCTION(Server, Reliable, WithValidation)
		void HitIngotOnServ(AIngot * _Ingot, bool _Succeded);
	UFUNCTION(NetMulticast, Reliable)
		void HitIngotOnMulticast(AIngot * _Ingot, bool _Succeded);

	//Respawn furnace charcoal
	UFUNCTION(Server, Reliable, WithValidation)
		void RespawnCharcoalOnServ(ARespawnFurnaceCharcoal * _RespawnFurnaceCharcoal);
	UFUNCTION(NetMulticast, Reliable)
		void RespawnCharcoalOnNetMulticast(ARespawnFurnaceCharcoal * _RespawnFurnaceCharcoal);

	//Foundry bowl
	UFUNCTION(Server, Reliable, WithValidation)
		void LoadRecipeFoundryBowlOnServ(AFoundryBowl * _FoundryBowl, FRecipe _Recipe);
	UFUNCTION(NetMulticast, Reliable)
		void LoadRecipeFoundryBowlOnNetMulticast(AFoundryBowl * _FoundryBowl, FRecipe _Recipe);

	UFUNCTION(Server, Reliable, WithValidation)
		void DeverseFoundryBowlToOnServ(AFoundryBowl * _FoundryBowl, AMold * _Mold);
	UFUNCTION(NetMulticast, Reliable)
		void DeverseFoundryBowlToOnNetMulticast(AFoundryBowl * _FoundryBowl, AMold * _Mold);

	//Mold
	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnRecipeMoldOnServ(AMold * _Mold);
	UFUNCTION(NetMulticast, Reliable)
		void SpawnRecipeMoldOnNetMulticast(AMold * _Mold);

	//DeliveryPoint
	UFUNCTION(Server, Reliable, WithValidation)
		void DeliveredOnServ(ADeliveryPoint *_DeliveryPoint);
	UFUNCTION(NetMulticast, Reliable)
		void DeliveredOnNetMulticast(ADeliveryPoint * _DeliveryPoint);
	UFUNCTION(NetMulticast, Reliable)
		void SetOrder(ADeliveryPoint * _DeliveryPoint, FOrderElement orderElement);

	//Static getPlayerState
	UFUNCTION()
		static APlayerStateVR * GetPlayerState(UWorld * worldRef);
	//Update Transform On All Client and server
	UFUNCTION(Server, Reliable, WithValidation)
		void UpdateTransformOnServ(AActor * actor, FVector newLocation, FRotator newRotation);
	UFUNCTION(NetMulticast, Reliable)
		void UpdateTransformOnNetMulticast(AActor * actor, FVector newLocation, FRotator newRotation);


	UFUNCTION(Server, Reliable, WithValidation)
		void FillWaterMoldOnServer(AMold * moldToFill);
	
	UFUNCTION(NetMulticast, Reliable)
		void FillWaterMoldOnMulticast(AMold * moldToFill);


	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnNetworkActorOnServ(UClass * actorClass, FVector location);
	UFUNCTION(NetMulticast, Reliable)
		void SpawnNetworkActorOnNetMulticast(UClass * actorClass, FVector location);
	UFUNCTION()
		void SpawnNetworkActorOnLocal(UClass * actorClass, FVector location);


	UFUNCTION(Server, Reliable, WithValidation)
		void UpdateIngotOnServ(UClass * _Class, bool _IsHot, float _TimeHeat);

	UFUNCTION(NetMulticast, Reliable)
		void UpdateIngotOnNet(UClass * _Class, bool _IsHot, float _TimeHeat);

	UFUNCTION(Server, Reliable, WithValidation)
		void UpdateFinalProductOnServ(UClass * _Class, bool _IsSharpened, float _TimeSharpened);

	UFUNCTION(NetMulticast, Reliable)
		void UpdateFinalProductOnNet(UClass * _Class, bool _IsSharpened, float _TimeSharpened);



	UFUNCTION(Server, Reliable, WithValidation)
		void CreateElementFromIngotOnServ(AIngot * ingotFrom ,UClass * weaponElementClass, USceneComponent * attachedComponent);


	UFUNCTION()
		static void RegisterPlayerStateVR(APlayerStateVR * playerStateVR);
		
	static APlayerStateVR * m_PlayerStateVR;


#pragma region Ready

	UFUNCTION()
		bool ToogleReadyState();
	UFUNCTION()
		void SetReadyState();
	UFUNCTION()
		void SetNotReadyState();
	UFUNCTION()
		bool IsReady();

	UFUNCTION(BlueprintCallable)
		void StartGame();
	UFUNCTION(BlueprintCallable)
		void EndGame();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GameIsRunning();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GameHasEnded();

private:

	UFUNCTION(Server, Reliable, WithValidation)
		void OnlineReady_S();
	UFUNCTION(NetMulticast, Reliable)
		void OnlineReady_M();
	UFUNCTION(Server, Reliable, WithValidation)
		void OnlineNotReady_S();
	UFUNCTION(NetMulticast, Reliable)
		void OnlineNotReady_M();
	UFUNCTION(Server, Reliable, WithValidation)
		void OnlineStartGame_S();
	UFUNCTION(NetMulticast, Reliable)
		void OnlineStartGame_M();
	UFUNCTION(Server, Reliable, WithValidation)
		void OnlineEndGame_S();
	UFUNCTION(NetMulticast, Reliable)
		void OnlineEndGame_M();

#pragma endregion

	UPROPERTY()
		bool m_isReady;
	UPROPERTY()
		bool m_gameIsRunning;
	UPROPERTY()
		bool m_gameHasEnded;

};
