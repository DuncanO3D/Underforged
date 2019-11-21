// Fill out your copyright notice in the Description page of Project Settings.

#include "Ingot.h"
#include "Components/StaticMeshComponent.h"
#include "Features/Interactable/WeaponElement/WeaponElement.h"
#include "Engine/Engine.h"
#include "Features/Interactable/WeaponElement/Blade.h"
#include "Features/Interactable/Usable/Tongs.h"
#include "Features/Anvil.h"

#include "Character/PlayerStateVR.h"

#include "GameCore/OnlineGameInstance.h"

#include "Character/MotionControllerVR.h"

AIngot::AIngot()
{

}

void AIngot::SetUp()
{
	UOnlineGameInstance * gameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		for (int i = 0; i < gameInstance->m_ListSpawnClass.Num(); i++)
		{
			if (GetClass() == gameInstance->m_ListSpawnClass[i].m_UClass)
			{
				SetHotAndTime(gameInstance->m_ListSpawnClass[i].m_IsHot, gameInstance->m_ListSpawnClass[i].m_TimeHeat);
				gameInstance->m_ListSpawnClass.RemoveAt(i);
				return;
			}
		}
	}
}

bool AIngot::IsCool()
{
	return !IsHot();
}

void AIngot::BeginPlay()
{
	Super::BeginPlay();

	ActualizeHeatMesh();
}

void AIngot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (m_HasStart == false)
	{
		SetUp();
		m_HasStart = true;
	}
}

void AIngot::SetHotAndTime(bool _IsHot, float _TimeHeat)
{
	m_IsHot = _IsHot;
	m_HeatUpTime = _TimeHeat;
	ActualizeHeatMesh();
}

bool AIngot::CanGetPick(USceneComponent * _toAttach)
{
	if (IsHot() || m_currentHeatTimer > 0)
	{
		AMotionControllerVR * motionControllerVR = Cast<AMotionControllerVR>(_toAttach->GetOwner());
		if (motionControllerVR)
		{
			motionControllerVR->RumbleController(0.3f);
			return false;
		}
	}
	return m_snappedAnvil == nullptr;
}

void AIngot::Drop(USceneComponent * _toDetach)
{
	TArray<AActor *> overlappingActors;
	m_Root->GetOverlappingActors(overlappingActors);
	AAnvil * anvilTriggered = nullptr;
	for (AActor* actor : overlappingActors) {
		AAnvil * anvil = Cast<AAnvil>(actor);
		if (anvil && anvil->IsAvailable()) {
			anvilTriggered = anvil;
			break;
		}
	}

	Super::Drop(_toDetach);
	if (anvilTriggered && anvilTriggered->SetupIngot(this))
		OnDropOnAnvil(anvilTriggered);
}

void AIngot::OnDropOnAnvil_Implementation(AAnvil * _anvil)
{
	m_Root->SetSimulatePhysics(false);
	m_snappedAnvil = _anvil;
}

void AIngot::SetTransformationsData(FTransformationsData _data)
{
	m_transformationsData = _data;
}

void AIngot::OnHit(bool _succeeded)
{
	if (!IsHot())
	{
		OnHitWhileCold();
		return;
	}

	if (m_transformationsData.finalForm == nullptr)
	{
		OnHitNoModel();
		return;
	}

	if (_succeeded) {
		m_hitCount++;
		finalState = m_currentStep >= m_transformationsData.steps.Num();
		if (finalState && m_hitCount >= m_transformationsData.hitCountFinalRequired || !finalState && m_hitCount >= m_transformationsData.steps[m_currentStep].hitCountRequired) {
			if (finalState) {
				CreateElement();
				m_snappedAnvil->Free();
				OnHitFinal();
			}
			else {
				m_Root->SetStaticMesh(m_transformationsData.steps[m_currentStep].mesh);
				ActualizeHeatMesh();
				OnHitTransformed();
			}
			m_currentStep++;
			m_hitCount = 0;
		}
		else OnHitSucceeded();
	}
	else OnHitFailed();
}

void AIngot::UpdateHeat(float _deltaTime)
{
	if (!IsHot())
	{
		DropIfHand();

		m_currentHeatTimer += _deltaTime;
		if (m_currentHeatTimer >= m_HeatUpTime)
		{
			m_IsHot = true;
			m_currentHeatTimer = 0;
			OnFinishHeat();
		}
	}
}

void AIngot::BeginHeat()
{
	if (IsHot())
	{
		m_currentHeatTimer = 0;
	}
}

void AIngot::BeginCool()
{
	if (!IsHot())
	{
		m_currentHeatTimer = 0;
	}
}

void AIngot::UpdateCool(float _deltaTime)
{
	if (IsHot())
	{
		m_currentHeatTimer += _deltaTime;
		if (m_currentHeatTimer >= m_CoolDownTime)
		{
			m_currentHeatTimer = 0;
			m_IsHot = false;
			OnFinishCool();
		}
	}
}

void AIngot::DropIfHand()
{
	if (m_AttachedComponent)
	{
		AMotionControllerVR * motionController = Cast<AMotionControllerVR>(m_AttachedComponent->GetOwner());

		if (motionController)
		{
			motionController->RumbleController(0.3f);
			motionController->ReleaseActor();
		}
	}
}

void AIngot::CreateElement()
{
	AWeaponElement * element = Cast<AWeaponElement>(GetWorld()->SpawnActor(m_transformationsData.finalForm));
	if (element)
	{
		ABlade * blade = Cast<ABlade>(element);
		if (blade)
			blade->SetHot(true);
		element->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());
		element->RegisterRespawnPositionForRpc(GetActorLocation());
	}

	Destroy();
}

void AIngot::ActualizeHeatMesh()
{
	UpdateHeatMesh(m_IsHot);
}

bool AIngot::IsHot()
{
	return m_IsHot;
}

float AIngot::GetTimeHeat()
{
	return m_HeatUpTime;
}

void AIngot::OnFinishHeat_Implementation()
{
	ActualizeHeatMesh();
}

void AIngot::OnFinishCool_Implementation()
{
	ActualizeHeatMesh();
}
