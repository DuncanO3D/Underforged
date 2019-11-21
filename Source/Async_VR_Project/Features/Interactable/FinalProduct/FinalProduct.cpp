#include "FinalProduct.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Features/Interactable/Enchantment.h"
#include "Engine/Engine.h"

#include "Features/Interactable/WeaponElement/Blade.h"
#include "Features/Interactable/WeaponElement/Hilt.h"
#include "Character/MotionControllerVR.h"
#include "Engine/StaticMesh.h"

#include "GameCore/OnlineGameInstance.h"

#include "Character/PlayerStateVR.h"

AFinalProduct::AFinalProduct()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	m_fx = CreateDefaultSubobject<UParticleSystemComponent>("fx");
	m_fx->SetupAttachment(m_Root);

	m_enchantmentStart = CreateDefaultSubobject<USceneComponent>("EnchantmentStart");
	m_enchantmentStart->SetupAttachment(m_Root);

	m_enchantmentEnd = CreateDefaultSubobject<USceneComponent>("EnchantmentEnd");
	m_enchantmentEnd->SetupAttachment(m_Root);

	m_BladeMesh = CreateDefaultSubobject<UStaticMeshComponent>("BladeMesh");
	m_BladeMesh->SetupAttachment(m_Root);

	m_distancePercentage = 90;
	m_directionAngleErrorMarging = 30;

	m_IsSharpened = false;
	m_SharpeningPercentage = 0;

	bReplicates = true;
}

void AFinalProduct::BeginPlay()
{
	Super::BeginPlay();

	SetUpBeginEnchant();
}

void AFinalProduct::SetUpSceneEnchantWithMesh()
{
	m_enchantmentStart->SetRelativeLocation(FVector(0, 0, 0));
	FVector endBladePos = FVector(0, m_BladeMesh->GetStaticMesh()->GetBounds().BoxExtent.Y, 0);
	m_enchantmentEnd->SetRelativeLocation(endBladePos);
	m_fx->SetRelativeLocation(endBladePos / 2);

}

void AFinalProduct::SetUpBeginEnchant()
{
	FVector StartToEnt = m_enchantmentEnd->GetComponentLocation() - m_enchantmentStart->GetComponentLocation();
	m_enchantmentDirection = StartToEnt.GetSafeNormal();
	m_enchantmentDistance = StartToEnt.Size();
}

void AFinalProduct::SetUp()
{
	UOnlineGameInstance * gameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		for (int i = 0; i < gameInstance->m_ListSpawnClass.Num(); i++)
		{
			if (GetClass() == gameInstance->m_ListSpawnClass[i].m_UClass)
			{
				SetSharpenProperties(gameInstance->m_ListSpawnClass[i].m_IsSharpen, gameInstance->m_ListSpawnClass[i].m_SharpeningPercentage);
				gameInstance->m_ListSpawnClass.RemoveAt(i);
				return;
			}
		}
	}
}

void AFinalProduct::SetSharpenPropertiesRPC(bool _IsSharpened, float _SharpeningPercentage)
{
	//APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
	//if (playerStateVR)
	//{
	//	playerStateVR->UpdateFinalProductOnServ(this, _IsSharpened, _SharpeningPercentage);
	//	playerStateVR->UpdateFinalProductOnNet(this, _IsSharpened, _SharpeningPercentage);
	//}
	SetSharpenProperties(_IsSharpened, _SharpeningPercentage);
}

void AFinalProduct::SetSharpenProperties(bool _IsSharpened, float _SharpeningPercentage)
{
	m_IsSharpened = _IsSharpened;
	m_SharpeningPercentage = _SharpeningPercentage;
	if (m_SharpeningPercentage >= 100)
	{
		OnFxSharpeningDone();
	}
}

bool AFinalProduct::TryEnchanted(AEnchantment * _enchantment)
{
	if (CanBeEnchanted(_enchantment))
	{
		Enchante(_enchantment);
		return true;
	}
	return false;
}

bool AFinalProduct::TrySharpened(float _addPercentage)
{
	if (CanBeSharpened(_addPercentage))
	{
		Sharpened(_addPercentage);
		return true;
	}
	return false;
}

void AFinalProduct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_HasStart == false)
	{
		SetUp();
		m_HasStart = true;
	}

	FVector StartToEnt = m_enchantmentEnd->GetComponentLocation() - m_enchantmentStart->GetComponentLocation();
	m_enchantmentDirection = StartToEnt.GetSafeNormal();
}

void AFinalProduct::Assemble(ABlade * _Blade, AHilt * _Hilt)
{
	// Search if one has parent, attach it and set a good position
	if (_Blade->GetAttachParentActor() != nullptr)
	{
		AMotionControllerVR * motionController = Cast<AMotionControllerVR>(_Blade);
		if (motionController)
		{
			motionController->GrabActor(this);
		}

		SetActorLocationAndRotation(_Blade->GetActorLocation(), _Blade->GetActorRotation());
		RegisterRespawnPositionForRpc(_Blade->GetActorLocation());
	}
	else if (_Hilt->GetAttachParentActor() != nullptr)
	{
		AMotionControllerVR * motionController = Cast<AMotionControllerVR>(_Hilt);
		if (motionController)
		{
			motionController->GrabActor(this);
		}

		SetActorLocationAndRotation(_Hilt->GetActorLocation(), _Hilt->GetActorRotation());
		RegisterRespawnPositionForRpc(_Hilt->GetActorLocation());

	}
	else
	{
		SetActorLocationAndRotation(_Hilt->GetActorLocation(),_Hilt->GetActorRotation());
		RegisterRespawnPositionForRpc(_Hilt->GetActorLocation());
	}
	OnAssembleFX();

}

bool AFinalProduct::CanBeEnchanted(AEnchantment * _enchantment)
{
	if (m_AttachedComponent != nullptr && _enchantment->GetAttachedComponent() != nullptr)
	{
		if (m_fx->Template == nullptr)
			return true;
	}
	return false;
}

void AFinalProduct::Enchante(AEnchantment * _enchantment)
{
	ReplicateEnchante(_enchantment);
}

bool AFinalProduct::CanBeSharpened(float _addPercentage)
{
	if (m_IsSharpened == false)
		return true;
	return false;
}

void AFinalProduct::Sharpened(float _addPercentage)
{
	if (m_SharpeningPercentage >= 100)
		return;

	m_SharpeningPercentage += _addPercentage;

	if (m_SharpeningPercentage >= 100)
	{
		m_SharpeningPercentage = 100;
		m_IsSharpened = true;

		//GEngine->AddOnScreenDebugMessage(8000, 5, FColor::Red, "Sharpening done");
		OnSharpeningDone();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(8000, 5, FColor::Red, "InSharpening");
		OnInSharpening();
	}
}

bool AFinalProduct::ReplicateEnchante_Validate(AEnchantment * _enchantment)
{
	return true;
}
void AFinalProduct::ReplicateEnchante_Implementation(AEnchantment * _enchantment)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Enchantment done");

	m_fx->Template = _enchantment->m_fx->Template;
	m_fx->Activate(true);
}

bool AFinalProduct::ReplicateSharpened_Validate(float _addPercentage)
{
	return true;
}

void AFinalProduct::ReplicateSharpened_Implementation(float _addPercentage)
{
	m_SharpeningPercentage += _addPercentage;

	if (m_SharpeningPercentage >= 100)
	{
		m_SharpeningPercentage = 100;
		m_IsSharpened = true;

		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Sharpening done");
	}
}