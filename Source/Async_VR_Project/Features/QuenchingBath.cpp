#include "QuenchingBath.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AQuenchingBath::AQuenchingBath()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Root = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	m_Root->SetupAttachment(RootComponent);

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	m_StaticMesh->SetupAttachment(m_Root);

	m_TriggerCollider = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	m_TriggerCollider->SetupAttachment(m_Root);

	m_feedback = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	m_feedback->SetupAttachment(m_Root);	

	m_audioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	m_audioComponent->SetupAttachment(m_Root);
}

void AQuenchingBath::BeginPlay()
{
	Super::BeginPlay();

	m_feedback->KillParticlesForced();
	m_feedback->DeactivateSystem();
	m_feedbackOn = false;
	
	m_TriggerCollider->OnComponentBeginOverlap.AddDynamic(this,&AQuenchingBath::OnBeginOverlap);
	m_TriggerCollider->OnComponentEndOverlap.AddDynamic(this,&AQuenchingBath::OnEndOverlap);
}

void AQuenchingBath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasSomethingInCoolingProcess())
	{
		if (m_feedbackOn == false)
		{
			m_feedbackOn = true;
			//m_feedback->ResetToDefaults();
			m_feedback->ActivateSystem();
			if (!m_audioComponent->IsPlaying())
				m_audioComponent->Play();
		}
	}
	else
	{
		if (m_feedbackOn == true)
		{
			m_feedbackOn = false;
			//m_feedback->KillParticlesForced();
			m_feedback->DeactivateSystem();
			m_audioComponent->Stop();
		}
	}

	for (int i = 0; i < m_ListCoolables.Num(); i++)
	{
		m_ListCoolables[i]->UpdateCool(DeltaTime);
	}
}

void AQuenchingBath::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ICoolableInterface * collableActor = Cast<ICoolableInterface>(OtherActor);
	if (collableActor)
	{
		collableActor->BeginCool();
		m_ListCoolables.AddUnique(OtherActor);
	}
}

void AQuenchingBath::OnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ICoolableInterface * collableActor = Cast<ICoolableInterface>(OtherActor);
	if (collableActor)
	{
		collableActor->EndCool();
		m_ListCoolables.Remove(OtherActor);
	}
}

bool AQuenchingBath::HasSomethingInCoolingProcess()
{
	for (int i = 0; i < m_ListCoolables.Num(); i++)
	{
		if (m_ListCoolables[i]->IsCool() == false)
			return true;
	}
	return false;
}

