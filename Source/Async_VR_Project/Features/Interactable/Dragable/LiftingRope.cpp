#include "LiftingRope.h"
#include "Engine/Engine.h"
#include "Character/PlayerStateVR.h"
#include "Character/PlayerControllerVR.h"

ALiftingRope::ALiftingRope() : ADragable()
{
	bReplicates = true;
	bReplicateMovement = true;
}

void ALiftingRope::Drag(FVector dragVelocity)
{
	APlayerStateVR * playerStateVR = GetPlayerState();
	if (playerStateVR)
	{
		// Functions to call in PlayerStateVR for sync
		playerStateVR->LiftingRopeDragOnServ(this, dragVelocity);
		playerStateVR->LiftingRopeDragOnNetMulticast(this, dragVelocity);
	}
}

void ALiftingRope::DragAllActorToLift(FVector dragVelocity)
{
	for (int i = 0; i < m_ActorsToLift.Num(); i++)
	{
		m_ActorsToLift[i]->SetActorLocation(m_ActorsToLift[i]->GetActorLocation() + dragVelocity);
	}
}

void ALiftingRope::Drag_Implementations(FVector dragVelocity)
{
	if (dragVelocity != FVector::ZeroVector)
	{
		FVector ZdragVelocity = dragVelocity;
		ZdragVelocity.X = 0;
		ZdragVelocity.Y = 0;
		ZdragVelocity.Z = FMath::Clamp(dragVelocity.Z, -1.f, 1.f);

		FVector newLocation = GetActorLocation() + ZdragVelocity;

		if (newLocation.Z >= m_MinHeight && newLocation.Z <= m_MaxHeight)
		{
			SetActorLocation(GetActorLocation() + ZdragVelocity);
			DragAllActorToLift(ZdragVelocity * -1);
			OnDrag(ZdragVelocity);
		}
	}
}