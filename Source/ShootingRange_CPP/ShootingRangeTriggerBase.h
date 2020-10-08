#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingRangeTriggerBase.generated.h"

UCLASS()
class SHOOTINGRANGE_CPP_API AShootingRangeTriggerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShootingRangeTriggerBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category=Collision)
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category=Render)
	class UTextRenderComponent* TextRender;
};