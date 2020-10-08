#include "ShootingRangeAnimInstance.h"
#include "ShootingRangeCharacter.h"

void UShootingRangeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AShootingRangeCharacter* Character = Cast<AShootingRangeCharacter>(TryGetPawnOwner());

	if (::IsValid(Character))
	{
		IsFiring = Character->IsFiring;
		IsReloading = Character->IsReloading;
		CanFiring = Character->CanFiring;
	}
}