#include "Furnace.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AFurnace::AFurnace()
{
	PrimaryActorTick.bCanEverTick = true;

	m_scene = CreateDefaultSubobject<USceneComponent>("Root");
	m_scene->SetupAttachment(RootComponent);

	m_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_staticMeshComponent->SetupAttachment(m_scene);

	m_boxComponent = CreateDefaultSubobject<UBoxComponent>("Collider");
	m_boxComponent->SetupAttachment(m_scene);

	m_flameParticle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	m_flameParticle->SetupAttachment(m_boxComponent);
	m_flameParticle->SetWorldScale3D(m_flameParticlesScale.ZeroVector);
}

void AFurnace::BeginPlay()
{
	Super::BeginPlay();
	m_boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFurnace::OnOverlapBegin);
	m_boxComponent->OnComponentEndOverlap.AddDynamic(this, &AFurnace::OnOverlapEnd);
}

void AFurnace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DecreaseFlame(DeltaTime);

	if (CanHeat())
	{
		for (int i = 0; i < m_listHeatables.Num(); i++)
		{
			m_listHeatables[i]->UpdateHeat(DeltaTime);
		}
	}
}

bool AFurnace::CanHeat()
{
	return m_infiniteFlame || m_flameFactor > 0.1;
}


void AFurnace::IncreaseFlame(float _value)
{
	float absValue = FMath::Abs(_value);
	float flameFactorWithValue = absValue * m_IncreaseRate + m_flameFactor;
	float clampFlameFactor = FMath::Clamp(flameFactorWithValue, m_minFlame, m_maxFlame);
	m_flameFactor = clampFlameFactor;
	m_flameParticlesScale = FVector(m_flameFactor, m_flameFactor, m_flameFactor);

	m_flameParticle->SetWorldScale3D(m_flameParticlesScale);
}

void AFurnace::DecreaseFlame(float _decreaseTimer)
{
	if (--m_maxDecreaseTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(m_menberTimerHandle);
	}
	m_flameFactor -= m_DecreaseRate * _decreaseTimer;
	float clampFlameFactor = FMath::Clamp(m_flameFactor, m_minFlame, m_maxFlame);
	m_flameFactor = clampFlameFactor;
	FVector flameParticlesScale = FVector(m_flameFactor, m_flameFactor, m_flameFactor);

	m_flameParticle->SetWorldScale3D(flameParticlesScale);
}

void AFurnace::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IHeatableInterface * heatableActor = Cast<IHeatableInterface>(OtherActor);

	if (heatableActor)
	{
		heatableActor->BeginHeat();
		m_listHeatables.AddUnique(OtherActor);
	}
}

void AFurnace::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IHeatableInterface * heatableActor = Cast<IHeatableInterface>(OtherActor);

	if (heatableActor)
	{
		heatableActor->EndHeat();
		m_listHeatables.Remove(OtherActor);
	}
}