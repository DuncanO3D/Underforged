#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/Interactable/InteractableActor.h"
#include "FinalProduct.generated.h"

class ABlade;
class AHilt;
class UStaticMeshComponent;

UCLASS()
class ASYNC_VR_PROJECT_API AFinalProduct : public AInteractableActor
{
	GENERATED_BODY()

public:
	AFinalProduct();

	void SetUp();

	void SetSharpenPropertiesRPC(bool _IsSharpened, float _SharpeningPercentage);
	void SetSharpenProperties(bool _IsSharpened, float _SharpeningPercentage);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UParticleSystemComponent * m_fx;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UParticleSystemComponent * m_assemblefx;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		USceneComponent * m_enchantmentStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		USceneComponent * m_enchantmentEnd;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent * m_BladeMesh;

	UPROPERTY()
		FVector m_enchantmentDirection;
	UPROPERTY()
		float m_enchantmentDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enchantment")
		float m_distancePercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enchantment")
		float m_directionAngleErrorMarging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sharpening")
		bool m_IsSharpened;
	UPROPERTY(VisibleAnywhere, Category = "Sharpening")
		float m_SharpeningPercentage;

	UFUNCTION(BlueprintCallable, Category = "Enchantment")
		bool TryEnchanted(class AEnchantment * _enchantment);
	UFUNCTION(BlueprintCallable, Category = "Enchantment")
		bool TrySharpened(float _addPercentage);

	virtual void Tick(float DeltaTime) override;

	void Assemble(ABlade * _Blade, AHilt * _Hilt);

	virtual bool CanBeEnchanted(class AEnchantment * _enchantment);
	virtual void Enchante(class AEnchantment * _enchantment);

	UFUNCTION(BlueprintImplementableEvent)
		void OnInSharpening();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSharpeningDone();

	UFUNCTION(BlueprintImplementableEvent)
		void OnAssembleFX();

	UFUNCTION(BlueprintImplementableEvent)
		void OnFxSharpeningDone();

protected:
	virtual void BeginPlay() override;

	void SetUpSceneEnchantWithMesh();
	void SetUpBeginEnchant();

	virtual bool CanBeSharpened(float _addPercentage);
	virtual void Sharpened(float _addPercentage);



private:
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void ReplicateEnchante(class AEnchantment * _enchantment);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void ReplicateSharpened(float _addPercentage);

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<ABlade> m_BladeUsed;
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<AHilt> m_HiltUsed;

	bool m_HasStart = false;
};
