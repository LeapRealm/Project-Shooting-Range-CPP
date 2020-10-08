#include "ShootingRangeTriggerEnd.h"

#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "ShootingRangeCharacter.h"

AShootingRangeTriggerEnd::AShootingRangeTriggerEnd()
{
	TextRender->Text = FText::FromString(TEXT("END"));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AShootingRangeTriggerEnd::BeginOverlap);
}

void AShootingRangeTriggerEnd::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AShootingRangeCharacter* Character = Cast<AShootingRangeCharacter>(OtherActor);
	if (!::IsValid(Character))
		return;
	
	if (Character->RemainTargetCount <= 0)
	{
		Character->GameEnd();
		Destroy();
	}
}