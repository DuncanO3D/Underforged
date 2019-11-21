#include "MicroOnde.h"
#include "Features/Interactable/InteractableActor.h"
#include "Character/PlayerStateVR.h"
#include "Character/MotionControllerVR.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"

#include "Features/Interactable/FinalProduct/FinalProduct.h"
#include "Features/Interactable/WeaponElement/Blade.h"
#include "Features/Interactable/CraftingMaterial/Ingot.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Character/PlayerStateVR.h"

#include "Engine/World.h"

AMicroOnde::AMicroOnde()
{
	bReplicates = true;
}

void AMicroOnde::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMicroOnde, Output);
	DOREPLIFETIME(AMicroOnde, InputsActor);
}

void AMicroOnde::OnTriggerEnter(AInteractableActor * inputActor)
{
	if (inputActor != nullptr)
	{
		InputsActor.Add(inputActor);
	}
}

void AMicroOnde::OnTriggerExit(AInteractableActor * outputActor)
{
	if (outputActor != nullptr)
	{
		InputsActor.Remove(outputActor);
	}
}

void AMicroOnde::OnButtonPressed(AMotionControllerVR * hand)
{
	if (hand == nullptr)
		return;

	auto gameState = GetWorld()->GetGameState();
	if (gameState)
	{
		for (size_t i = 0; i < gameState->PlayerArray.Num(); i++)
		{
			APlayerStateVR * playerState = Cast<APlayerStateVR>(gameState->PlayerArray[i]);
		}
	}

	APlayerStateVR * PlayerState = APlayerStateVR::GetPlayerState(GetWorld());
	if (PlayerState != nullptr)
	{
		auto ActorsArray = InputsActor.Array();

		for (int i = 0; i < ActorsArray.Num(); i++)
		{
			if (Cast<ABlade>(ActorsArray[i]))
				continue;

			PlayerState->SpawnNetworkActorOnServ(ActorsArray[i]->GetClass(), Output->GetActorLocation());

			AIngot * ingot = Cast<AIngot>(ActorsArray[i]);
			ABlade * blade = Cast<ABlade>(ActorsArray[i]);
			AFinalProduct * finalProduct = Cast<AFinalProduct>(ActorsArray[i]);

			bool onServ = Role == ROLE_Authority;

			if (ingot)
			{
				if (onServ)
					PlayerState->UpdateIngotOnNet(ingot->GetClass(), ingot->IsHot(), ingot->GetTimeHeat());
				else
					PlayerState->UpdateIngotOnServ(ingot->GetClass(), ingot->IsHot(), ingot->GetTimeHeat());
			}
			else if (finalProduct)
			{
				if (onServ)
					PlayerState->UpdateFinalProductOnNet(finalProduct->GetClass(), finalProduct->m_IsSharpened, finalProduct->m_SharpeningPercentage);
				else
					PlayerState->UpdateFinalProductOnServ(finalProduct->GetClass(), finalProduct->m_IsSharpened, finalProduct->m_SharpeningPercentage);
			}

			PlayerState->DestroyActorOnServ(ActorsArray[i]);
			PlayerState->DestroyActorNetMulticast(ActorsArray[i]);
		}
	}
}

