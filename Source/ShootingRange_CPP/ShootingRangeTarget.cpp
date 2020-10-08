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

	if (CurveFloat != nullptr)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, TEXT("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(false);
	}
}

void AShootingRangeTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);
}

void AShootingRangeTarget::TargetTransform(bool IsUp)
{
	if (IsUp)
	{
		CanHit = true;
		CurveTimeline.Play();
	}
	else
	{
		CanHit = false;
		CurveTimeline.Reverse();
	}
}

void AShootingRangeTarget::TimelineProgress(float Value)
{
	Board->SetRelativeRotation(FRotator(Value, 0.0f, 0.0f));
}