#pragma once

#include "CoreMinimal.h"
#include "ShootingRangeTriggerBase.h"
#include "ShootingRangeTriggerStart.generated.h"

UCLASS()
class SHOOTINGRANGE_CPP_API AShootingRangeTriggerStart : public AShootingRangeTriggerBase
{
	GENERATED_BODY()

public:
	AShootingRangeTriggerStart();
	
	UFUNCTION()
	void BeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};