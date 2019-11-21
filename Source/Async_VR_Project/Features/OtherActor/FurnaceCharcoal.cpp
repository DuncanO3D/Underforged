// Fill out your copyright notice in the Description page of Project Settings.

#include "FurnaceCharcoal.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/Engine.h"

// Sets default values
AFurnaceCharcoal::AFurnaceCharcoal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_scene = CreateDefaultSubobject<USceneComponent>("Root");
	m_scene->SetupAttachment(RootComponent);

	m_sphereComponent = CreateDefaultSubobject<USphereComponent>("Collider");
	m_sphereComponent->SetupAttachment(m_scene);

	m_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_staticMeshComponent->SetupAttachment(m_sphereComponent);
}

// Called when the game starts or when spawned
void AFurnaceCharcoal::BeginPlay()
{
	Super::BeginPlay();
	m_staticMeshComponent->SetMaterial(0, m_coolMaterial);
}

// Called every frame
void AFurnaceCharcoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DecreaseHeatingCharcoal(DeltaTime);
}

bool AFurnaceCharcoal::Heating()
{
	return m_isHeating;
}

bool AFurnaceCharcoal::Cooling()
{
	return m_isCooling;
}

bool AFurnaceCharcoal::Consuming()
{
	return m_isConsume;
}

void AFurnaceCharcoal::BeginHeat()
{
	if (m_isCooling == true && m_timeToBeHeat > 0)
	{
		m_timeToBeHeat -= 1;
	}
	else if (m_timeToBeHeat <= 0)
	{
		m_isHeating = true;
		m_isCooling = false;
		m_staticMeshComponent->SetMaterial(0, m_heatMaterial);
	}
}

void AFurnaceCharcoal::UpdateHeat(float _deltaTime)
{
	if (m_isHeating == false)
	{
		m_timeToBeHeat -= _deltaTime;
		m_staticMeshComponent->SetMaterial(0, m_heatMaterial);
	}
}

void AFurnaceCharcoal::EndHeat()
{
	if (m_timeToBeHeat > 0)
	{
		m_timeToBeHeat -= 0.0001;
	}
	else if (m_timeToBeHeat <= 0)
	{
		m_isHeating = true;
		m_isCooling = false;
	}

}

void AFurnaceCharcoal::BeginCool()
{
}

void AFurnaceCharcoal::UpdateCool(float _deltaTime)
{
}

void AFurnaceCharcoal::EndCool()
{

}

void AFurnaceCharcoal::HeatingCharcoal()
{
	if (m_isHeating == false && m_timeToBeHeat > 0)
	{
		m_timeToBeHeat -= 1;
	}
	else if (m_timeToBeHeat <= 0)
	{
		m_isHeating = true;
		m_isCooling = false;
		m_staticMeshComponent->SetMaterial(0, m_heatMaterial);
	}
	m_timeToBeCool = 15.0;
}

void AFurnaceCharcoal::CoolingCharcoal()
{
	if (m_isHeating == true && m_timeToBeCool > 0)
	{
		m_timeToBeCool -= 1;
	}
	else if (m_timeToBeCool <= 0)
	{
		m_isHeating = false;
		m_isCooling = true;
		m_staticMeshComponent->SetMaterial(0, m_coolMaterial);
	}
	m_timeToBeHeat = 5.0;
}

void AFurnaceCharcoal::DecreaseHeatingCharcoal(float _value)
{
	m_timeToBeCool -= _value;
	if (m_timeToBeCool <= 0)
	{
		m_staticMeshComponent->SetMaterial(0, m_coolMaterial);
		m_isConsume = true;
		Destroy();
	}
}

void AFurnaceCharcoal::Respawn()
{
	SetActorLocation(m_respawnPosition);
}


