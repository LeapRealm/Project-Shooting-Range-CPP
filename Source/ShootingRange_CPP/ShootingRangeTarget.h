#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ShootingRangeTarget.generated.h"

UCLASS()
class SHOOTINGRANGE_CPP_API AShootingRangeTarget : public AActor
{
	GENERATED_BODY()

public:
	AShootingRangeTarget();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void TargetTransform(bool IsUp);

	UFUNCTION()
	void SetBoardRotation(float Value);

protected:
	FTimeline TargetTimeline;

	UPROPERTY(EditAnywhere, Category="Timeline")
	class UCurveFloat* DegreeCurve;
	
private:
	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UStaticMeshComponent* Board;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollision;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool CanHit;
};