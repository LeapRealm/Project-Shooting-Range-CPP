#include "ShootingRangeTriggerBase.h"

#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

AShootingRangeTriggerBase::AShootingRangeTriggerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(20.0f, 145.0f, 120.0f));
	Trigger->SetCollisionProfileName(TEXT("Trigger"));
	
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXTRENDER"));
	TextRender->Text = FText::FromString(TEXT("*SET TEXT*"));
	TextRender->SetHorizontalAlignment(EHTA_Center);
	TextRender->SetVerticalAlignment(EVRTA_TextCenter);
	TextRender->SetWorldSize(80.0f);
}

void AShootingRangeTriggerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AShootingRangeTriggerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}