// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponElement.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Features/Interactable/WeaponElement/Blade.h"
#include "Features/Interactable/WeaponElement/Hilt.h"
#include "Features/Interactable/FinalProduct/FinalProduct.h"

#include "Features/Factory/BlacksmithFactory.h"
#include "GameCore/OnlineGameInstance.h"

#include "Character/MotionControllerVR.h"
#include "Features/Interactable/Usable/Tongs.h"

#include "Character/PlayerStateVR.h"

#include "Engine/Engine.h"

#include "TimerManager.h"

AWeaponElement::AWeaponElement()
{
	m_attachTrigger = CreateDefaultSubobject<UBoxComponent>("Attach Zone");
	m_attachTrigger->SetupAttachment(m_Root);
}

void AWeaponElement::Drop(USceneComponent * _toDetach)
{
	Super::Drop(_toDetach);
	
	//OnDropTryAssemble();

}

void AWeaponElement::OnDropTryAssemble()
{
	TArray<AActor*> overlappingActors;
	m_attachTrigger->GetOverlappingActors(overlappingActors);

	for (int i = 0; i < overlappingActors.Num(); i++)
	{
		AWeaponElement * weaponElement = Cast<AWeaponElement>(overlappingActors[i]);
		if (weaponElement)
		{
			if (TryAssembleToFinalProduct(weaponElement))
				break;
		}
	}
}

UStaticMesh * AWeaponElement::GetStaticMesh()
{
	return m_Root->GetStaticMesh();
}

TArray<UMaterialInterface*> AWeaponElement::GetMaterials()
{
	return m_Root->GetMaterials();
}

FVector AWeaponElement::GetCenterPosition()
{
	return m_center;
}

void AWeaponElement::ForceDrop()
{
	AMotionControllerVR * hand = Cast<AMotionControllerVR>(m_AttachedComponent);
	ATongs * tong = Cast<ATongs>(m_AttachedComponent);
	if (hand)
		hand->ReleaseActor();
	else if (tong)
		tong->TryStopUse();
}

void AWeaponElement::BeginPlay()
{
	Super::BeginPlay();
	//m_attachTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWeaponElement::OnOverlapBegin);
}

bool AWeaponElement::TryAssembleToFinalProduct(AWeaponElement * _OtherWeaponElement)
{
	ABlade * blade = nullptr;
	AHilt * hilt = nullptr;

	blade = Cast<ABlade>(this);
	if (blade == nullptr)
		blade = Cast<ABlade>(_OtherWeaponElement);

	hilt = Cast<AHilt>(this);
	if (hilt == nullptr)
		hilt = Cast<AHilt>(_OtherWeaponElement);

	if (hilt == nullptr || blade == nullptr || blade->IsHot())
		return false;
	else
	{
		return AssembleToFinalProduct(blade, hilt);
	}
		

}

bool AWeaponElement::AssembleToFinalProduct(ABlade * _blade, AHilt * _hilt)
{
	UOnlineGameInstance * gameInstance = UOnlineGameInstance::GetInstance(GetWorld());
	if (gameInstance == nullptr)
		return false;

	TSubclassOf<AFinalProduct> classFinalProduct = gameInstance->GetBlacksmithFactory()->GetCorrespondandOrder(_blade,_hilt).m_finalProductClass;
	AFinalProduct * finalProduct = Cast<AFinalProduct>(GetWorld()->SpawnActor(classFinalProduct));
	if (finalProduct)
	{
		//finalProduct->SetActorLocationAndRotation(_hilt->GetActorLocation(), _hilt->GetActorRotation());
		_blade->ForceDrop();
		finalProduct->Assemble(_blade, _hilt);

		AMotionControllerVR * hand = nullptr;
		ATongs * tong = nullptr;
		if (_blade->m_AttachedComponent) {
			hand = Cast<AMotionControllerVR>(_blade->m_AttachedComponent->GetOwner());
			tong = Cast<ATongs>(_blade->m_AttachedComponent->GetOwner());
		}
		else if (_hilt->m_AttachedComponent) {
			hand = Cast<AMotionControllerVR>(_hilt->m_AttachedComponent->GetOwner());
			tong = Cast<ATongs>(_hilt->m_AttachedComponent->GetOwner());
		}

		_blade->Destroy();
		_hilt->Destroy();

		APlayerStateVR * playerStateVR = APlayerStateVR::GetPlayerState(GetWorld());
		if (playerStateVR)
		{
			playerStateVR->DestroyActorOnServ(_blade);
			playerStateVR->DestroyActorNetMulticast(_blade);
			playerStateVR->DestroyActorOnServ(_hilt);
			playerStateVR->DestroyActorNetMulticast(_hilt);
		}

		if (hand) {
			hand->ReleaseActor();
			hand->GrabActor(finalProduct);
		}
		else if (tong) {
			tong->TryStopUse();
			tong->TryBeginUse(finalProduct);
		}


		return true;
	}
	else
		return false;
	
}

void AWeaponElement::OnOverlapBegin(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComponent, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult)
{
	UBoxComponent * boxTrigger = Cast<UBoxComponent>(_otherComponent);
	AWeaponElement * weaponElement = Cast<AWeaponElement>(_otherActor);
	if (boxTrigger && weaponElement)
		TryAssembleToFinalProduct(weaponElement);
}
