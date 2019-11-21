#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/Interactable/InteractableActor.h"
#include "Enchantment.generated.h"

UCLASS()
class ASYNC_VR_PROJECT_API AEnchantment : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	AEnchantment();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UParticleSystemComponent * m_fx;

	float m_distancePercentage;
	float m_directionAngleErrorMarging;
	float m_distanceToEnchante;
	bool m_bIsCollidingWithEnchantable;
	bool m_bCanEnchante;
	class AFinalProduct * m_enchantmentTarget;
	FVector m_enchanteDirection;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		virtual AInteractableActor * GetPick(USceneComponent * _AttachTo) override;
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		virtual void Drop(USceneComponent * _AttachTo) override;
		
	UFUNCTION(BlueprintCallable, Category = "Enchantment")
		void OnBeginOverlapWithEnchantable(class AFinalProduct * _FinalProduct);
	UFUNCTION(BlueprintCallable, Category = "Enchantment")
		void OnEndOverlapWithEnchantable(class AFinalProduct * _FinalProduct);

	USceneComponent * GetAttachedComponent();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void OnDrag(FVector value);

	FVector GetDragVector();
	float GetSignedAngleRad(FVector a, FVector b, FVector axis);
	float GeAngleRad(FVector a, FVector b);

	FVector m_pos_FirstFrame;
	FVector m_pos_SecondFrame;
	float m_actualDistanceToEnchante;
};
