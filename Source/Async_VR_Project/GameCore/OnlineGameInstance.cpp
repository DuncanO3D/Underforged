#include "OnlineGameInstance.h"

#include "Features/Factory/BlacksmithFactory.h"
#include "Engine/World.h"

UOnlineGameInstance::UOnlineGameInstance()
{

}

UOnlineGameInstance::~UOnlineGameInstance()
{

}

UOnlineGameInstance * UOnlineGameInstance::GetInstance(UWorld * _WorldObject)
{
	if (_WorldObject)
	{
		return Cast<UOnlineGameInstance>(_WorldObject->GetGameInstance());
	}
	return nullptr;
}

void UOnlineGameInstance::Init()
{
	Super::Init();

	m_BlacksmithFactory = NewObject<UBlacksmithFactory>(this, m_BsFactoryClass);
}

UBlacksmithFactory * UOnlineGameInstance::GetBlacksmithFactory()
{
	return m_BlacksmithFactory;
}

UMaterialInstance * UOnlineGameInstance::GetOutlineMaterial()
{
	return m_OutlineMaterial;
}
