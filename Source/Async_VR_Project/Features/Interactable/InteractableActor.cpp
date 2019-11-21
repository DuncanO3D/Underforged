#include "InteractableActor.h"

#include "Components/StaticMeshComponent.h"

#include "Character/PlayerStateVR.h"
#include "Character/PlayerControllerVR.h"

#include "Character/PawnVR.h"
#include "Character/MotionControllerVR.h"
#include "Features/Interactable/Usable/Tongs.h"

#include "Engine/Engine.h"
#include "Engine/Classes/Materials/MaterialInstance.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

#include "GameCore/OnlineGameInstance.h"

#include "Components/TextRenderComponent.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;

	m_Root = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	m_Root->SetupAttachment(RootComponent);

	m_textRoot = CreateDefaultSubobject<USceneComponent>("TextRoot");
	m_textRoot->SetupAttachment(m_Root);

	m_textHeader = CreateDefaultSubobject<UTextRenderComponent>("TextHeader");
	m_textHeader->SetupAttachment(m_textRoot);
	m_textHeader->SetHorizontalAlignment(EHTA_Center);
	m_textHeader->SetVerticalAlignment(EVRTA_TextCenter);
	m_textHeader->SetWorldSize(16);
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	RegisterRespawnPosition();

	m_textHeader->SetText(FText::FromString(m_name));
	m_textHeader->SetVisibility(false);
}

void AInteractableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TextHeaderLookAtPlayer();
	m_textRoot->SetWorldRotation(FRotator::ZeroRotator);

	if (m_NumberOfSelection == 0)
		m_timerDisappear += DeltaSeconds;
	if (m_timerDisappear > m_disappearTime)
		m_textHeader->SetVisibility(false);
}

AInteractableActor * AInteractableActor::TryGetPick(USceneComponent * _toAttach)
{
	if (CanGetPick(_toAttach))
		return GetPick(_toAttach);
	return nullptr;
}

bool AInteractableActor::TryDrop(USceneComponent * _toDetach)
{
	if (CanDrop(_toDetach))
	{
		Drop(_toDetach);
		return true;
	}
	return false;
}

USceneComponent * AInteractableActor::GetAttachedComponent()
{
	return m_AttachedComponent;
}

void AInteractableActor::NoCollision()
{
	m_Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AInteractableActor::PhysicalCollision()
{
	m_Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

bool AInteractableActor::CanGetPick(USceneComponent * _toAttach)
{
	return true;
}

AInteractableActor * AInteractableActor::GetPick(USceneComponent * _toAttach)
{
	return GetPick(_toAttach, false, false);
}

AInteractableActor * AInteractableActor::GetPick(USceneComponent * _toAttach, bool _snapToTarget, bool _disablePhysicsQuery)
{
	if (!TryTransferFromGrabbingHand(_toAttach))
		TryTransferFromHoldingTong(_toAttach);
	m_Root->SetSimulatePhysics(false);
	if(_disablePhysicsQuery)
		m_Root->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent->AttachToComponent(_toAttach, FAttachmentTransformRules::KeepWorldTransform);
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, _toAttach->GetName());
	if (_snapToTarget)
		SnapTo(_toAttach);
	m_AttachedComponent = _toAttach;
	
	OnPickUp(_toAttach);
	return this;
}

bool AInteractableActor::CanDrop(USceneComponent * _toDetach)
{
	return true;
}

void AInteractableActor::Drop(USceneComponent * _toDetach)
{
	if (m_AttachedComponent != _toDetach)
		return;
	
	m_Root->SetSimulatePhysics(true);
	m_Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DetachFromActor(FDetachmentTransformRules(FDetachmentTransformRules::KeepWorldTransform));
	m_AttachedComponent = nullptr;
	
	OnDrop(_toDetach);
}

void AInteractableActor::SnapTo(USceneComponent * _SceneComponent)
{
	RootComponent->SetWorldLocationAndRotation(_SceneComponent->GetComponentLocation(), _SceneComponent->GetComponentRotation());
}

bool AInteractableActor::TryTransferFromGrabbingHand(USceneComponent * _toAttach)
{
	if (!m_AttachedComponent)
		return false;
	AMotionControllerVR * hand = Cast<AMotionControllerVR>(m_AttachedComponent->GetOwner());
	if (!hand)
		return false;
	AMotionControllerVR * newHand = Cast<AMotionControllerVR>(_toAttach->GetOwner());
	if (newHand) {
		hand->TransferGrabbedActorTo(newHand);
		return true;
	}
	ATongs * newTong = Cast<ATongs>(_toAttach->GetOwner());
	if (newTong) {
		hand->TransferGrabbedActorTo(newTong);
		return true;
	}
	return false;
}

bool AInteractableActor::TryTransferFromHoldingTong(USceneComponent * _toAttach)
{
	if (!m_AttachedComponent)
		return false;
	ATongs * tong = Cast<ATongs>(m_AttachedComponent->GetOwner());
	if (!tong)
		return false;
	AMotionControllerVR * newHand = Cast<AMotionControllerVR>(_toAttach->GetOwner());
	if (newHand) {
		tong->TransferGrabbedActorTo(newHand);
		return true;
	}
	ATongs * newTong = Cast<ATongs>(_toAttach->GetOwner());
	if (newTong) {
		tong->TransferGrabbedActorTo(newTong);
		return true;
	}
	return false;
}

void AInteractableActor::TextHeaderLookAtPlayer()
{
	/*/
	TArray<AActor*> tab;
	m_Root->GetOverlappingActors(tab);
	AMotionControllerVR * hand = nullptr;
	for (AActor * actor : tab) {
		AMotionControllerVR * h = nullptr;
		ATongs * t = Cast<ATongs>(actor);
		if (t)
			h = t->GetHand();
		h = h ? h : Cast<AMotionControllerVR>(actor);
		if (h) {
			hand = h;
			break;
		}
	}

	if (hand && hand->GetOwningPawn())
		m_lastCamLocation = hand->GetOwningPawn()->GetCameraLocation();

	/**/
	FVector target = GetNearestPlayerCameraLocation();
	FVector textPos = m_textHeader->GetComponentLocation();
	FVector dir = target - textPos;
	FRotator rotation = FRotationMatrix::MakeFromX(dir).Rotator();
	rotation.Roll = 0;
	m_textHeader->SetWorldRotation(rotation);
}

FVector AInteractableActor::GetNearestPlayerCameraLocation()
{
	TArray<APlayerState*> ar = GetWorld()->GetGameState()->PlayerArray;
	FVector nearest = FVector::ZeroVector;
	for (int i = 0; i < ar.Num(); i++) {
		APawnVR* p = Cast<APawnVR>(ar[i]->GetPawn());
		if (!p) break;
		if (i == 0)
			nearest = p->GetCameraLocation();
		else {
			FVector pCam = p->GetCameraLocation();
			FVector self = GetActorLocation();
			float distP = (self - pCam).Size();
			float distNear = (self - nearest).Size();
			if (distP < distNear)
				nearest = pCam;
		}
	}
	return nearest;
}

void AInteractableActor::TryRegisterPlayerStateVR(APlayerStateVR * playerStateVR)
{
	if (playerStateVR == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(5, 5, FColor::Orange, "playerStateVR null");
	}

	m_PlayerStateVR = playerStateVR;
}

void AInteractableActor::Select()
{
	m_NumberOfSelection++;
	
	bool outline = m_NumberOfSelection > 0;
	m_Root->SetRenderCustomDepth(outline);

	if (outline)
		EventSelect();
	else
		EventDeselect();


	m_textHeader->SetVisibility(true);
	m_timerDisappear = 0;
}

void AInteractableActor::Deselect()
{
	m_NumberOfSelection--;
	if (m_NumberOfSelection < 0)
		m_NumberOfSelection = 0;

	bool outline = m_NumberOfSelection > 0;
	m_Root->SetRenderCustomDepth(outline);

	if (outline)
		EventSelect();
	else
		EventDeselect();
}

APlayerStateVR * AInteractableActor::GetPlayerState()
{
	if (m_PlayerStateVR != nullptr)
		return m_PlayerStateVR;

	if (m_AttachedComponent != nullptr)
	{
		m_PlayerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
	}

	//if (m_PlayerStateVR == nullptr)
	//	GEngine->AddOnScreenDebugMessage(10, 10, FColor::Orange, "Nullref playerStateVR");

	return m_PlayerStateVR;
}

void AInteractableActor::RegisterRespawnPosition()
{
	m_RespawnPosition = GetActorLocation();
}

void AInteractableActor::RegisterRespawnPositionForRpc(FVector newRespawnPosition)
{
	m_RespawnPosition = newRespawnPosition;
}

void AInteractableActor::Respawn()
{
	SetActorLocation(m_RespawnPosition);
	m_Root->ComponentVelocity = FVector::ZeroVector;
}
