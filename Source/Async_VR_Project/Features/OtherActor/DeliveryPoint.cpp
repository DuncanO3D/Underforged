#include "DeliveryPoint.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Character/PlayerStateVR.h"

#include "GameCore/OnlineGameInstance.h"
#include "UnrealNetwork.h"
#include "Features/Interactable/FinalProduct/FinalProduct.h"
#include "Features/OtherActor/FurnaceCharcoal.h"

// Sets default values
ADeliveryPoint::ADeliveryPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_scene = CreateDefaultSubobject<USceneComponent>("Root");
	m_scene->SetupAttachment(RootComponent);

	m_triggerBox = CreateDefaultSubobject<UBoxComponent>("Collider");
	m_triggerBox->SetupAttachment(m_scene);

	m_orderText = CreateDefaultSubobject<UTextRenderComponent>("OrderTEXT");
	m_orderText->SetupAttachment(m_scene);

	m_bladeMatText = CreateDefaultSubobject<UTextRenderComponent>("BladeTEXT");
	m_bladeMatText->SetupAttachment(m_scene);

	m_hiltMatText = CreateDefaultSubobject<UTextRenderComponent>("HiltTEXT");
	m_hiltMatText->SetupAttachment(m_scene);

	bReplicates = true;
}

void ADeliveryPoint::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeliveryPoint, m_orderElement);
}

// Called when the game starts or when spawned
void ADeliveryPoint::BeginPlay()
{
	Super::BeginPlay();
	auto gameState = GetWorld()->GetGameState();
	if (gameState)
	{
		for (size_t i = 0; i < gameState->PlayerArray.Num(); i++)
		{
			APlayerStateVR * playerState = Cast<APlayerStateVR>(gameState->PlayerArray[i]);
			if (playerState)
			{
				if (GetWorld()->GetAuthGameMode())
					Order(playerState);
			}
		}
	}
	m_triggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADeliveryPoint::OnOverlapBegin);
}

// Called every frame
void ADeliveryPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FText ADeliveryPoint::GetNbSwordText()
{
	FString NbSword = FString::FromInt(m_nbSword);
	FString SwordHUD = NbSword + FString(TEXT(""));
	FText SwordText = FText::FromString(SwordHUD);
	return SwordText;
}

void ADeliveryPoint::ScorePoint()
{
	m_nbSword += m_scoreForSword;
}

void ADeliveryPoint::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFinalProduct * finalProduct = Cast<AFinalProduct>(OtherActor);

	if (finalProduct)
	{
		if (!finalProduct->m_IsSharpened)
		{
			OnNotSharpened();
			return;
		}

		auto gameState = GetWorld()->GetGameState();
		if (gameState)
		{
			for (size_t i = 0; i < gameState->PlayerArray.Num(); i++)
			{
				APlayerStateVR * playerState = Cast<APlayerStateVR>(gameState->PlayerArray[i]);
				if (playerState)
				{
					if (finalProduct->GetClass() == m_orderElement.m_finalProductClass)
					{	
						if (!GetWorld()->GetAuthGameMode())	// if the caller is not server
							playerState->DeliveredOnServ(this);
						else
							Order(playerState);
						if (i == 0)
							playerState->DeliveredOnNetMulticast(this);

						OnOrderSuccessed();
					}
					else
					{
						OnOrderFailed();
					}

					playerState->DestroyActorOnServ(finalProduct);
					playerState->DestroyActorNetMulticast(finalProduct);
				}
			}
		}
	}
}

void ADeliveryPoint::Order(APlayerStateVR * playerState)
{
	UOnlineGameInstance * gameInstance = UOnlineGameInstance::GetInstance(GetWorld());
	if (gameInstance == nullptr)
		return;
	
	if (playerState)
	{
		m_orderElement = gameInstance->GetBlacksmithFactory()->GetRandomOrder();
		playerState->SetOrder(this, m_orderElement);
	}
}

void ADeliveryPoint::SetTextOrder(FOrderElement textOrder)
{
	m_orderElement = textOrder;
	m_orderText->SetText(FText::FromString(m_orderElement.m_swordName));
	m_bladeMatText->SetText(FText::FromString(m_orderElement.m_bladeName));
	m_hiltMatText->SetText(FText::FromString(m_orderElement.m_hiltName));
}

