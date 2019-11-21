// Fill out your copyright notice in the Description page of Project Settings.

#include "Blade.h"

#include "Components/StaticMeshComponent.h"

#include "Character/MotionControllerVR.h"

ABlade::ABlade()
{
	m_MeshHeat = CreateDefaultSubobject<UStaticMeshComponent>("Mesh heat");
	m_MeshHeat->SetupAttachment(m_Root);
}

bool ABlade::IsCool()
{
	return !IsHot();
}

void ABlade::BeginPlay()
{
	Super::BeginPlay();

	ActualiseMeshHeat();
}

void ABlade::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABlade::SetHot(bool _IsHot)
{
	m_IsHot = _IsHot;
	ActualiseMeshHeat();
}

void ABlade::SetTimeHeat(float _TimeHeat)
{
	m_HeatUpTime = _TimeHeat;
}

bool ABlade::CanGetPick(USceneComponent * _toAttach)
{
	if (IsHot())
	{
		AMotionControllerVR * motionControllerVR = Cast<AMotionControllerVR>(_toAttach->GetOwner());
		if (motionControllerVR)
		{
			motionControllerVR->RumbleController(0.3f);
			return false;
		}
	}
	return true;
}

void ABlade::BeginCool()
{
	if (!IsHot())
	{
		m_currentHeatTimer = 0;
	}
}

void ABlade::UpdateCool(float _deltaTime)
{
	if (IsHot())
	{
		m_currentHeatTimer += _deltaTime;
		if (m_currentHeatTimer >= m_CoolDownTime)
		{
			m_IsHot = false;
			OnFinishCool();
		}
	}
}

void ABlade::UpdateHeat(float _deltaTime)
{
	if (!IsHot())
	{
		DropIfHand();
		m_currentHeatTimer += _deltaTime;
		if (m_currentHeatTimer >= m_HeatUpTime)
		{
			m_IsHot = true;
			OnFinishHeat();
		}
	}
}

void ABlade::BeginHeat()
{
	if (IsHot())
	{
		m_currentHeatTimer = 0;
	}
}

bool ABlade::IsHot()
{
	return m_IsHot;
}

float ABlade::GetTimeHeat()
{
	return m_HeatUpTime;
}

void ABlade::DropIfHand()
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

void ABlade::ActualiseMeshHeat()
{
	m_MeshHeat->SetVisibility(m_IsHot);
}

void ABlade::OnFinishHeat_Implementation()
{
	ActualiseMeshHeat();
}

void ABlade::OnFinishCool_Implementation()
{
	ActualiseMeshHeat();
}
