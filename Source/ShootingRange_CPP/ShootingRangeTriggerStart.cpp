#include "ShootingRangeTriggerStart.h"

#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "ShootingRangeCharacter.h"

AShootingRangeTriggerStart::AShootingRangeTriggerStart()
{
	TextRender->Text = FText::FromString(TEXT("START"));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AShootingRangeTriggerStart::BeginOverlap);
}

void AShootingRangeTriggerStart::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AShootingRangeCharacter* Character = Cast<AShootingRangeCharacter>(OtherActor);
	if (!::IsValid(Character))
		return;
	
	Character->GameStart_Walk();
	Destroy();
}