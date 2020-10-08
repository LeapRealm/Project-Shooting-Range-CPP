#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShootingRangeAnimInstance.generated.h"

UCLASS()
class SHOOTINGRANGE_CPP_API UShootingRangeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Condition, meta=(AllowPrivateAccess=true))
	bool IsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Condition, meta=(AllowPrivateAccess=true))
	bool IsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Condition, meta=(AllowPrivateAccess=true))
	bool CanFiring;
};