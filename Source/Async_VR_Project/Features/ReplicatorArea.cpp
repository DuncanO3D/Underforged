#include "ReplicatorArea.h"
#include "Character/PlayerStateVR.h"

AReplicatorArea::AReplicatorArea()
{
	PrimaryActorTick.bCanEverTick = true;

	m_root = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	m_root->SetupAttachment(RootComponent);
}

void AReplicatorArea::OnTriggerEnter(AActor * toReplicate)
{
	if (toReplicate != nullptr && toReplicate != this)
	{
		APlayerStateVR * playerState = APlayerStateVR::GetPlayerState(GetWorld());

		if (playerState != nullptr)
		{
			playerState->UpdateTransformOnServ(toReplicate, toReplicate->GetActorLocation(), toReplicate->GetActorRotation());
			playerState->UpdateTransformOnNetMulticast(toReplicate, toReplicate->GetActorLocation(), toReplicate->GetActorRotation());
		}
	}
}

void AReplicatorArea::BeginPlay()
{
	Super::BeginPlay();	
}