// Fill out your copyright notice in the Description page of Project Settings.

#include "Book.h"
#include "Character/MotionControllerVR.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"


ABook::ABook()
{

}

void ABook::BeginPlay()
{
	Super::BeginPlay();

	m_Root->OnComponentBeginOverlap.AddDynamic(this, &ABook::OnOverlapBegin);
	m_Root->OnComponentEndOverlap.AddDynamic(this, &ABook::OnOverlapEnd);
}

AInteractableActor * ABook::GetPick(USceneComponent * _toAttach, bool _snapToTarget, bool _disablePhysicsQuery)
{
	AInteractableActor * model = SpawnModel(_toAttach->GetComponentLocation());
	if(!model)
		return nullptr;
	return model->TryGetPick(_toAttach);
}

AInteractableActor * ABook::SpawnModel(FVector _position)
{
	FVector localPosition = ToLocalPosition(_position);
	if (FMath::Abs(localPosition.Y) < m_middleMargin)
		return nullptr;

	int index = (m_currentPage - 1) * 2;
	
	if (localPosition.Y < 0)
		index--;

	if (index < 0 || index >= m_models.Num())
		return nullptr;

	return GetWorld()->SpawnActor<AInteractableActor>(m_models[index], _position, FRotator::ZeroRotator);
}

FVector ABook::ToLocalPosition(FVector _worldPosition)
{
	return GetTransform().InverseTransformPosition(_worldPosition);
}

void ABook::FlipLeftPage()
{
	if (m_currentPage > 0)
		m_currentPage--;
	//GEngine->AddOnScreenDebugMessage(2, 1, FColor::Magenta, "Flip left : " + FString::FromInt(m_currentPage));
}

void ABook::FlipRightPage()
{
	if (m_currentPage < GetMaxPagesCount())
		m_currentPage++;
	//GEngine->AddOnScreenDebugMessage(2, 1, FColor::Magenta, "Flip right : " + FString::FromInt(m_currentPage));
}

int ABook::GetMaxPagesCount()
{
	return (m_models.Num()+3) /2;
}

void ABook::OnOverlapBegin(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComponent, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult)
{
	AMotionControllerVR * hand = Cast<AMotionControllerVR>(_otherActor);
	if (!hand || hand->IsHoldingAnObject())
		return;
	m_handOverlapStartPoint = ToLocalPosition(_otherComponent->GetComponentLocation());
}

void ABook::OnOverlapEnd(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComponent, int32 _otherBodyIndex)
{
	AMotionControllerVR * hand = Cast<AMotionControllerVR>(_otherActor);
	if (!hand || hand->IsHoldingAnObject())
		return;
	FVector handOverlapEndPoint = ToLocalPosition(_otherComponent->GetComponentLocation());
	float sideMovement = handOverlapEndPoint.Y - m_handOverlapStartPoint.Y;
	if (FMath::Abs(sideMovement) > m_flipDistance)
		if (sideMovement < 0 && m_handOverlapStartPoint.Y > 0)
			FlipRightPage();
		else if (sideMovement > 0 && m_handOverlapStartPoint.Y < 0)
			FlipLeftPage();
}
