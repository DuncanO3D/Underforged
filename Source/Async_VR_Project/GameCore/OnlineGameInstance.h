#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Engine/Classes/Materials/MaterialInstance.h"

#include "OnlineGameInstance.generated.h"

class UBlacksmithFactory;

USTRUCT(BlueprintType)
struct FOptionSpawnClass
{
	GENERATED_BODY()
public:
	//Constructor
	FOptionSpawnClass()
	{
		m_UClass = nullptr;
		m_IsHot = false;
		m_TimeHeat = 0;
		m_SharpeningPercentage = 0;
	}

	UClass * m_UClass;

	// Objets chauffables
	bool m_IsHot = false;
	float m_TimeHeat = 0;

	// Final product
	bool m_IsSharpen = false;
	float m_SharpeningPercentage = 0;


};

UCLASS()
class ASYNC_VR_PROJECT_API UOnlineGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UOnlineGameInstance();
	~UOnlineGameInstance();

	static UOnlineGameInstance * GetInstance(UWorld * _WorldObject);

	virtual void Init() override;

	UBlacksmithFactory * GetBlacksmithFactory();

	UMaterialInstance * GetOutlineMaterial();

	TArray<FOptionSpawnClass> m_ListSpawnClass;

private:
	UPROPERTY(EditAnywhere, Category = "BlacksmithFactory")
		TSubclassOf<UBlacksmithFactory> m_BsFactoryClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "BlacksmithFactory")
		UBlacksmithFactory * m_BlacksmithFactory = nullptr;

	UPROPERTY(EditAnywhere)
		UMaterialInstance * m_OutlineMaterial;
};
