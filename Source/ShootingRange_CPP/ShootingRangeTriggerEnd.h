#pragma once

#include "CoreMinimal.h"
#include "ShootingRangeTriggerBase.h"
#include "ShootingRangeTriggerEnd.generated.h"

UCLASS()
class SHOOTINGRANGE_CPP_API AShootingRangeTriggerEnd : public AShootingRangeTriggerBase
{
	GENERATED_BODY()

public:
	AShootingRangeTriggerEnd();

	UFUNCTION()
	void BeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};