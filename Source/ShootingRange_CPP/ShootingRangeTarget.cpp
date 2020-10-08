#include "ShootingRangeTarget.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AShootingRangeTarget::AShootingRangeTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ARROW"));
	Board = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOARD"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOXCOLLISION"));

	SetRootComponent(Arrow);
	Board->SetupAttachment(Arrow);
	BoxCollision->SetupAttachment(Board);

	Arrow->SetVisibleFlag(false);
	BoxCollision->SetCollisionProfileName(TEXT("BlockAll"));
}

void AShootingRangeTarget::BeginPlay()
{
	Super::BeginPlay();

	if (DegreeCurve != nullptr)
	{
		FOnTimelineFloat TimelineDelegate;
		TimelineDelegate.BindUFunction(this, TEXT("SetBoardRotation"));
		TargetTimeline.AddInterpFloat(DegreeCurve, TimelineDelegate);
		TargetTimeline.SetLooping(false);
	}
}

void AShootingRangeTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TargetTimeline.TickTimeline(DeltaTime);
}

void AShootingRangeTarget::TargetTransform(bool IsUp)
{
	if (IsUp)
	{
		CanHit = true;
		TargetTimeline.Play();
	}
	else
	{
		CanHit = false;
		TargetTimeline.Reverse();
	}
}

void AShootingRangeTarget::SetBoardRotation(float Value)
{
	Board->SetRelativeRotation(FRotator(Value, 0.0f, 0.0f));
}