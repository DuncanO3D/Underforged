#include "PlayerStateVR.h"
#include "GameFramework/PlayerController.h"

#include "Features/Interactable/Dragable/LiftingRope.h"
#include "Features/Interactable/Dragable/RotatingLever.h"
#include "Features/GrindStone.h"
#include "Features/Interactable/Dragable/Bellow.h"
#include "Features/Interactable/CraftingMaterial/Ingot.h"
#include "Features/OtherActor/RespawnFurnaceCharcoal.h"
#include "Features/Interactable/FinalProduct/FinalProduct.h"
#include "Features/Interactable/Enchantment.h"
#include "Features/Interactable/FoundryBowl.h"
#include "Features/Interactable/CraftingMaterial/CraftingMaterial.h"
#include "Features/Interactable/Usable/Mold.h"
#include "Features/OtherActor/DeliveryPoint.h"

#include "Features/Interactable/CraftingMaterial/Ingot.h"
#include "Features/Interactable/WeaponElement/Blade.h"
#include "Features/Interactable/WeaponElement/WeaponElement.h"

#include "Features/Interactable/Usable/Tongs.h"

#include "GameCore/OnlineGameInstance.h"

#include "Engine/Engine.h"
#include "Engine/World.h"

APlayerStateVR * APlayerStateVR::m_PlayerStateVR = nullptr;

APlayerStateVR::APlayerStateVR()
{
	bReplicates = true;
}

bool APlayerStateVR::DestroyActorOnServ_Validate(AActor * actor)
{
	return true;
}

void APlayerStateVR::DestroyActorOnServ_Implementation(AActor * actor)
{
	if (actor != nullptr)
		actor->Destroy();
}

void APlayerStateVR::DestroyActorNetMulticast_Implementation(AActor * actor)
{
	if (actor != nullptr)
		actor->Destroy();
}

bool APlayerStateVR::LiftingRopeDragOnServ_Validate(ALiftingRope * liftingRope, FVector dragVelocity)
{
	return true;
}

void APlayerStateVR::LiftingRopeDragOnServ_Implementation(ALiftingRope * liftingRope, FVector dragVelocity)
{
	if (liftingRope)
	{
		liftingRope->Drag_Implementations(dragVelocity);
	}
}

void APlayerStateVR::LiftingRopeDragOnNetMulticast_Implementation(ALiftingRope * liftingRope, FVector dragVelocity)
{
	if (liftingRope)
	{
		liftingRope->Drag_Implementations(dragVelocity);
	}
}

bool APlayerStateVR::RotatingLeverDragOnServ_Validate(ARotatingLever * rotatingLever, FVector dragVelocity)
{
	return true;
}

void APlayerStateVR::RotatingLeverDragOnServ_Implementation(ARotatingLever * rotatingLever, FVector dragVelocity)
{
	if (rotatingLever)
	{
		rotatingLever->Drag_Implementations(dragVelocity);
	}
}

void APlayerStateVR::RotatingLeverDragOnNetMulticast_Implementation(ARotatingLever * rotatingLever, FVector dragVelocity)
{
	if (rotatingLever)
	{
		rotatingLever->Drag_Implementations(dragVelocity);
	}
}

bool APlayerStateVR::RotatingGrindingStoneDragOnServ_Validate(AGrindStone * grindingStone, float angularSpeed)
{
	return true;
}

void APlayerStateVR::RotatingGrindingStoneDragOnServ_Implementation(AGrindStone * grindingStone, float angularSpeed)
{
	if (grindingStone)
	{
		grindingStone->ReplicateRotate(angularSpeed);
	}
}

void APlayerStateVR::RotatingGrindingStoneDragOnNetMulticast_Implementation(AGrindStone * grindingStone, float angularSpeed)
{
	if (grindingStone)
	{
		grindingStone->ReplicateRotate(angularSpeed);
	}
}

bool APlayerStateVR::BelowDragOnServ_Validate(ABellow * below, FVector dragVelocity)
{
	return true;
}

void APlayerStateVR::BelowDragOnServ_Implementation(ABellow * below, FVector dragVelocity)
{
	if (below)
	{
		below->Drag_Implementations(dragVelocity);
	}
}

void APlayerStateVR::BelowDragOnNetMulticast_Implementation(ABellow * below, FVector dragVelocity)
{
	if (below)
	{
		below->Drag_Implementations(dragVelocity);
	}
}

bool APlayerStateVR::HitIngotOnServ_Validate(AIngot * _Ingot, bool _Succeded)
{
	return true;
}

void APlayerStateVR::HitIngotOnServ_Implementation(AIngot * _Ingot, bool _Succeded)
{
	//GEngine->AddOnScreenDebugMessage(4, 100, FColor::Red, "OnServ: OnHitIngot");
	if (_Ingot)
		_Ingot->OnHit(_Succeded);
}

void APlayerStateVR::HitIngotOnMulticast_Implementation(AIngot * _Ingot, bool _Succeded)
{
	//GEngine->AddOnScreenDebugMessage(4, 100, FColor::Red, "OnNetMulticast: OnHitIngot");
	if (_Ingot)
		_Ingot->OnHit(_Succeded);
}

bool APlayerStateVR::RespawnCharcoalOnServ_Validate(ARespawnFurnaceCharcoal * _RespawnFurnaceCharcoal)
{
	return true;
}

void APlayerStateVR::RespawnCharcoalOnServ_Implementation(ARespawnFurnaceCharcoal * _RespawnFurnaceCharcoal)
{
	if (_RespawnFurnaceCharcoal)
		_RespawnFurnaceCharcoal->SpawnCharcoal();
}

void APlayerStateVR::RespawnCharcoalOnNetMulticast_Implementation(ARespawnFurnaceCharcoal * _RespawnFurnaceCharcoal)
{
	if (_RespawnFurnaceCharcoal)
		_RespawnFurnaceCharcoal->SpawnCharcoal();
}

bool APlayerStateVR::LoadRecipeFoundryBowlOnServ_Validate(AFoundryBowl * _FoundryBowl, FRecipe _Recipe)
{
	return true;
}

void APlayerStateVR::LoadRecipeFoundryBowlOnServ_Implementation(AFoundryBowl * _FoundryBowl, FRecipe _Recipe)
{
	if (_FoundryBowl)
	{
		_FoundryBowl->LoadRecipeForReplicate(_Recipe);
	}
}

void APlayerStateVR::LoadRecipeFoundryBowlOnNetMulticast_Implementation(AFoundryBowl * _FoundryBowl, FRecipe _Recipe)
{
	if (_FoundryBowl)
	{
		_FoundryBowl->LoadRecipeForReplicate(_Recipe);
	}
}

bool APlayerStateVR::SpawnRecipeMoldOnServ_Validate(AMold * _Mold)
{
	return true;
}

void APlayerStateVR::SpawnRecipeMoldOnServ_Implementation(AMold * _Mold)
{
	//GEngine->AddOnScreenDebugMessage(4, 100, FColor::Red, "OnServ: SpawnRecipe");
	if (_Mold)
	{
		FVector location = _Mold->m_MeshCubeHeat->GetComponentLocation();
		FRotator rotation = _Mold->m_MeshCubeHeat->GetComponentRotation();
		AInteractableActor * m_SpawnedCraftingMaterial = Cast<AInteractableActor>(GetWorld()->SpawnActor(_Mold->m_ClassMaterialToSpawn, &location, &rotation));
		m_SpawnedCraftingMaterial->TryGetPick(_Mold->GetRootComponent());
		m_SpawnedCraftingMaterial->NoCollision();

		//m_SpawnedCraftingMaterial->SetActorLocationAndRotation(_Mold->m_MeshBaseForme->GetComponentLocation(), _Mold->m_MeshBaseForme->GetComponentRotation());

		_Mold->m_SpawnedCraftingMaterial = m_SpawnedCraftingMaterial;
	}
}

void APlayerStateVR::SpawnRecipeMoldOnNetMulticast_Implementation(AMold * _Mold)
{
	//GEngine->AddOnScreenDebugMessage(5, 100, FColor::Red, "OnNetMulticast: SpawnRecipe");
	//if (_Mold)
	//{
	//	AInteractableActor * m_SpawnedCraftingMaterial = Cast<AInteractableActor>(GetWorld()->SpawnActor<AInteractableActor>(_Mold->m_ClassMaterialToSpawn));
	//	m_SpawnedCraftingMaterial->SetActorLocationAndRotation(_Mold->m_MeshBaseForme->GetComponentLocation(), _Mold->m_MeshBaseForme->GetComponentRotation());
	//	_Mold->SpawnInteractableFromRecipe(m_SpawnedCraftingMaterial);
	//}
}

bool APlayerStateVR::DeliveredOnServ_Validate(ADeliveryPoint *_DeliveryPoint)
{
	return true;
}

void APlayerStateVR::DeliveredOnServ_Implementation(ADeliveryPoint *_DeliveryPoint)
{
	if (_DeliveryPoint)
	{
		_DeliveryPoint->ScorePoint();
	}
}

void APlayerStateVR::DeliveredOnNetMulticast_Implementation(ADeliveryPoint *_DeliveryPoint)
{
	if (_DeliveryPoint)
	{
		_DeliveryPoint->ScorePoint();
	}
}

void APlayerStateVR::SetOrder_Implementation(ADeliveryPoint * _DeliveryPoint, FOrderElement orderElement)
{
	if (_DeliveryPoint)
		_DeliveryPoint->SetTextOrder(orderElement);
}

APlayerStateVR * APlayerStateVR::GetPlayerState(UWorld * worldRef)
{
	if (m_PlayerStateVR != nullptr)
		return m_PlayerStateVR;

	if (worldRef != nullptr)
	{
		APlayerController * first = worldRef->GetFirstPlayerController();
		if (first != nullptr)
			return Cast<APlayerStateVR>(first->PlayerState);
	}
	return nullptr;
}

bool APlayerStateVR::CreateElementFromIngotOnServ_Validate(AIngot * ingotFrom, UClass * weaponElementClass, USceneComponent * attachedComponent)
{
	return true;
}

void APlayerStateVR::CreateElementFromIngotOnServ_Implementation(AIngot * ingotFrom, UClass * weaponElementClass, USceneComponent * attachedComponent)
{
	if (ingotFrom == nullptr)
		return;

	AWeaponElement * element = Cast<AWeaponElement>(GetWorld()->SpawnActor(weaponElementClass));
	if (element)
	{
		ABlade * blade = Cast<ABlade>(element);
		if (blade)
			blade->SetHot(true);
		element->SetActorLocationAndRotation(ingotFrom->GetActorLocation() - element->GetCenterPosition(), ingotFrom->GetActorRotation());
	}

	if (attachedComponent)
	{
		ATongs * tong = Cast<ATongs>(attachedComponent->GetOwner());
		if (tong)
		{
			bool stop = tong->TryStopUse();
			bool begin = tong->TryBeginUse(element);
		}
		//else
		//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "not a tong");
	}
}

void APlayerStateVR::RegisterPlayerStateVR(APlayerStateVR * playerStateVR)
{
	m_PlayerStateVR = playerStateVR;
}

bool APlayerStateVR::UpdateTransformOnServ_Validate(AActor * actor, FVector newLocation, FRotator newRotation)
{
	return true;
}

void APlayerStateVR::UpdateTransformOnServ_Implementation(AActor * actor, FVector newLocation, FRotator newRotation)
{
	if (actor)
	{
		actor->SetActorLocation(newLocation);
		actor->SetActorRotation(newRotation);
	}
}

void APlayerStateVR::UpdateTransformOnNetMulticast_Implementation(AActor * actor, FVector newLocation, FRotator newRotation)
{
	if (actor)
	{
		actor->SetActorLocation(newLocation);
		actor->SetActorRotation(newRotation);
	}
}

bool APlayerStateVR::FinalProductTrySharpenedOnServ_Validate(AFinalProduct * finalProduct, float _SharpeningPower)
{
	return true;
}

void APlayerStateVR::FinalProductTrySharpenedOnServ_Implementation(AFinalProduct * finalProduct, float _SharpeningPower)
{
	if (finalProduct)
		finalProduct->TrySharpened(_SharpeningPower);
}

void APlayerStateVR::FinalProductTrySharpenedOnNetMulticast_Implementation(AFinalProduct * finalProduct, float _SharpeningPower)
{
	if (finalProduct)
		finalProduct->TrySharpened(_SharpeningPower);
}

bool APlayerStateVR::EnchantFinalProductOnServ_Validate(AFinalProduct * _FinalProduct, AEnchantment * _Enchantement)
{
	return true;
}

void APlayerStateVR::EnchantFinalProductOnServ_Implementation(AFinalProduct * _FinalProduct, AEnchantment * _Enchantement)
{
	if (_FinalProduct != nullptr)
	{
		_FinalProduct->Enchante(_Enchantement);
	}
}

void APlayerStateVR::EnchantFinalProductOnNetMulticast_Implementation(AFinalProduct * _FinalProduct, AEnchantment * _Enchantement)
{
	if (_FinalProduct != nullptr)
	{
		_FinalProduct->Enchante(_Enchantement);
	}
}

bool APlayerStateVR::RespawnInteractableActorOnServ_Validate(AInteractableActor * interactable)
{
	return true;
}

void APlayerStateVR::RespawnInteractableActorOnServ_Implementation(AInteractableActor * interactable)
{
	if (interactable)
		interactable->Respawn();
}

void APlayerStateVR::RespawnInteractableActorOnNetMulticast_Implementation(AInteractableActor * interactable)
{
	if (interactable)
		interactable->Respawn();
}

bool APlayerStateVR::DeverseFoundryBowlToOnServ_Validate(AFoundryBowl * _FoundryBowl, AMold * _Mold)
{
	return true;
}

void APlayerStateVR::DeverseFoundryBowlToOnServ_Implementation(AFoundryBowl * _FoundryBowl, AMold * _Mold)
{
	//GEngine->AddOnScreenDebugMessage(4, 100, FColor::Red, "OnServ: DeverseToBowl");
	if (_FoundryBowl)
		_FoundryBowl->DeverseTo(_Mold);
}

void APlayerStateVR::DeverseFoundryBowlToOnNetMulticast_Implementation(AFoundryBowl * _FoundryBowl, AMold * _Mold)
{
	//GEngine->AddOnScreenDebugMessage(5, 100, FColor::Red, "OnNetMulticast: DeverseToBowl");
	if (_FoundryBowl)
		_FoundryBowl->DeverseTo(_Mold);
}

bool APlayerStateVR::SpawnNetworkActorOnServ_Validate(UClass * actorClass, FVector location)
{
	return true;
}




void APlayerStateVR::SpawnNetworkActorOnServ_Implementation(UClass * actorClass, FVector location)
{
	AActor * actor = GetWorld()->SpawnActor(actorClass, &location, &FRotator::ZeroRotator);

	AInteractableActor * interactable = Cast<AInteractableActor>(actor);
	if (interactable)
	{
		interactable->RegisterRespawnPositionForRpc(location);
	}
}

void APlayerStateVR::SpawnNetworkActorOnNetMulticast_Implementation(UClass * actorClass, FVector location)
{
	AActor * actor = GetWorld()->SpawnActor(actorClass, &location, &FRotator::ZeroRotator);
	
	AInteractableActor * interactable = Cast<AInteractableActor>(actor);
	if (interactable)
	{
		interactable->RegisterRespawnPositionForRpc(location);
	}
}

void APlayerStateVR::SpawnNetworkActorOnLocal(UClass * actorClass, FVector location)
{
	GetWorld()->SpawnActor(actorClass, &location, &FRotator::ZeroRotator);
}



bool APlayerStateVR::UpdateIngotOnServ_Validate(UClass * _Class, bool _IsHot, float _TimeHeat)
{
	return true;
}

void APlayerStateVR::UpdateIngotOnServ_Implementation(UClass * _Class, bool _IsHot, float _TimeHeat)
{
	UOnlineGameInstance * gameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		FOptionSpawnClass option = FOptionSpawnClass();
		option.m_UClass = _Class;
		option.m_IsHot = _IsHot;
		option.m_TimeHeat = _TimeHeat;
		gameInstance->m_ListSpawnClass.Add(option);
	}
}

void APlayerStateVR::UpdateIngotOnNet_Implementation(UClass * _Class, bool _IsHot, float _TimeHeat)
{
	UOnlineGameInstance * gameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		FOptionSpawnClass option = FOptionSpawnClass();
		option.m_UClass = _Class;
		option.m_IsHot = _IsHot;
		option.m_TimeHeat = _TimeHeat;
		gameInstance->m_ListSpawnClass.Add(option);
	}
}

bool APlayerStateVR::UpdateFinalProductOnServ_Validate(UClass * _Class, bool _IsSharpened, float _TimeSharpened)
{
	return true;
}

void APlayerStateVR::UpdateFinalProductOnServ_Implementation(UClass * _Class, bool _IsSharpened, float _TimeSharpened)
{
	UOnlineGameInstance * gameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		FOptionSpawnClass option = FOptionSpawnClass();
		option.m_UClass = _Class;
		option.m_IsSharpen = _IsSharpened;
		option.m_SharpeningPercentage = _TimeSharpened;
		gameInstance->m_ListSpawnClass.Add(option);
	}
}

void APlayerStateVR::UpdateFinalProductOnNet_Implementation(UClass * _Class, bool _IsSharpened, float _TimeSharpened)
{
	UOnlineGameInstance * gameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		FOptionSpawnClass option = FOptionSpawnClass();
		option.m_UClass = _Class;
		option.m_IsSharpen = _IsSharpened;
		option.m_SharpeningPercentage = _TimeSharpened;
		gameInstance->m_ListSpawnClass.Add(option);
	}
}



bool APlayerStateVR::FillWaterMoldOnServer_Validate(AMold * moldToFill)
{
	return true;
}

void APlayerStateVR::FillWaterMoldOnServer_Implementation(AMold * moldToFill)
{
	if (moldToFill)
	{
		moldToFill->AddWater();
	}
}

void APlayerStateVR::FillWaterMoldOnMulticast_Implementation(AMold * moldToFill)
{
	if (moldToFill)
	{
		moldToFill->AddWater();
	}
}

#pragma region Ready

bool APlayerStateVR::ToogleReadyState()
{
	if (m_isReady)
		SetReadyState();
	else
		SetNotReadyState();
	return m_isReady;
}

void APlayerStateVR::SetReadyState()
{
	OnlineReady_S();
	OnlineReady_M();
}

void APlayerStateVR::SetNotReadyState()
{
	OnlineNotReady_S();
	OnlineNotReady_M();
}

bool APlayerStateVR::IsReady()
{
	return m_isReady;
}

void APlayerStateVR::StartGame()
{
	OnlineStartGame_S();
	OnlineStartGame_M();
}

void APlayerStateVR::EndGame()
{
	OnlineEndGame_S();
	OnlineEndGame_M();
}

bool APlayerStateVR::GameIsRunning()
{
	return m_gameIsRunning;
}

bool APlayerStateVR::GameHasEnded()
{
	return m_gameHasEnded;
}

void APlayerStateVR::OnlineReady_S_Implementation()
{
	m_isReady = true;
}

bool APlayerStateVR::OnlineReady_S_Validate()
{
	return true;
}

void APlayerStateVR::OnlineReady_M_Implementation()
{
	m_isReady = true;
}

void APlayerStateVR::OnlineNotReady_S_Implementation()
{
	m_isReady = false;
}

bool APlayerStateVR::OnlineNotReady_S_Validate()
{
	return true;
}

void APlayerStateVR::OnlineNotReady_M_Implementation()
{
	m_isReady = false;
}

void APlayerStateVR::OnlineStartGame_S_Implementation()
{
	m_gameIsRunning = true;
}

bool APlayerStateVR::OnlineStartGame_S_Validate()
{
	return true;
}

void APlayerStateVR::OnlineStartGame_M_Implementation()
{
	m_gameIsRunning = true;
}

void APlayerStateVR::OnlineEndGame_S_Implementation()
{
	m_gameIsRunning = false;
	m_gameHasEnded = true;
}

bool APlayerStateVR::OnlineEndGame_S_Validate()
{
	return m_gameIsRunning;
}

void APlayerStateVR::OnlineEndGame_M_Implementation()
{
	m_gameIsRunning = false;
	m_gameHasEnded = true;
}

#pragma endregion
