#include "ShootingRangeCharacter.h"
#include "ShootingRange_CPP.h"
#include "ShootingRangeHUD.h"
#include "ShootingRangeSaveGame.h"
#include "ShootingRangeTarget.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "Components/BoxComponent.h"

AShootingRangeCharacter::AShootingRangeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	Camera->SetupAttachment(GetCapsuleComponent());
	CharacterMesh->SetupAttachment(Camera);

	Camera->bUsePawnControlRotation = true;
	CharacterMesh->SetCastShadow(false);
	GetCharacterMovement()->SetWalkableFloorAngle(60.0f);
}

void AShootingRangeCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetInitialGameType();
	GameReady();
}

void AShootingRangeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentProgressType)
	{
	case EProgressType::None:
		UE_LOG(ShootingRange, Error, TEXT("ProgressType is 'None'"));
		break;

	case EProgressType::Play:
		PassedTime = PassedTime + DeltaTime;
		CurrentTimeRecord = PassedTime;

		switch (CurrentGameType)
		{
		case EGameType::None:
			UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
			break;
			
		case EGameType::Stand:
			if (PassedTime > LimitTime)
				GameEnd();
			else
				HUD->UpdateProgress(FMath::RoundToFloat(LimitTime - PassedTime), 0);
			break;
			
		case EGameType::Walk:
			HUD->UpdateCurrentTimeRecord(CurrentTimeRecord);
			break;
		}
		break;
	}
}

void AShootingRangeCharacter::SetInitialGameType()
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	if (CurrentLevelName == TEXT("ShootingRange_Stand"))
	{
		SetCurrentGameType(EGameType::Stand);
	}
	else
	{
		if (CurrentLevelName == TEXT("ShootingRange_Walk"))
		{
			SetCurrentGameType(EGameType::Walk);
		}
		else
		{
			SetCurrentGameType(EGameType::None);
			UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		}
	}
}

void AShootingRangeCharacter::GameReady()
{
	SetCurrentProgressType(EProgressType::Ready);

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	HUD = CreateWidget<UShootingRangeHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HUDClass);
	HUD->AddToViewport();
	HUD->HUDReady();

	RemainTargetCount = Targets.Num();
	if (RemainTargetCount == 0)
	{
		UE_LOG(ShootingRange, Error, TEXT("Targets Length is '0'"));
	}
	else
	{
		UShootingRangeSaveGame* SaveGame = Cast<UShootingRangeSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot"), 0));
		if (SaveGame == nullptr)
		{
			BestScoreRecord = 0;
			BestTimeRecord = 0.0f;
		}
		else
		{
			BestScoreRecord = SaveGame->BestScoreRecord;
			BestTimeRecord = SaveGame->BestTimeRecord;
		}
	}
}

void AShootingRangeCharacter::GamePlay()
{
	SetCurrentProgressType(EProgressType::Play);

	HUD->HUDPlay(CurrentMagazineAmmoCount, BestScoreRecord, CurrentScoreRecord, BestTimeRecord, CurrentTimeRecord, FMath::RoundToFloat(LimitTime), RemainTargetCount);
	TargetUp();
	CharacterMesh->GetAnimInstance()->Montage_Play(DrawMontage);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]() -> void
	{
		CanFiring = true;
	});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.8f, false);
}

void AShootingRangeCharacter::GameEnd()
{
	SetCurrentProgressType(EProgressType::End);

	CharacterMesh->GetAnimInstance()->Montage_Play(HideMontage);
	CanFiring = false;
	
	switch (CurrentGameType)
	{
	case EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;
		
	case EGameType::Stand:
		if (CurrentScoreRecord > BestScoreRecord)
		{
			BestScoreRecord = CurrentScoreRecord;

			UShootingRangeSaveGame* SaveGame = Cast<UShootingRangeSaveGame>(UGameplayStatics::CreateSaveGameObject(UShootingRangeSaveGame::StaticClass()));
			SaveGame->BestScoreRecord = BestScoreRecord;
			SaveGame->BestTimeRecord = BestTimeRecord;
			UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Slot"), 0);
		}
		break;
		
	case EGameType::Walk:
		GetCharacterMovement()->StopMovementImmediately();

		if ((CurrentTimeRecord < BestTimeRecord) || (FMath::IsNearlyEqual(BestTimeRecord, 0.0f, 0.001f)))
		{
			BestTimeRecord = CurrentTimeRecord;

			UShootingRangeSaveGame* SaveGame = Cast<UShootingRangeSaveGame>(UGameplayStatics::CreateSaveGameObject(UShootingRangeSaveGame::StaticClass()));
			SaveGame->BestScoreRecord = BestScoreRecord;
			SaveGame->BestTimeRecord = BestTimeRecord;
			UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Slot"), 0);
		}
		break;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->bShowMouseCursor = true;
	
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(HUD->GetCachedWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);

	HUD->HUDEnd(BestScoreRecord, CurrentScoreRecord, BestTimeRecord, CurrentTimeRecord);
}

void AShootingRangeCharacter::TargetUp()
{
	int32 Index;
	
	switch (CurrentGameType)
	{
	case EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;
		
	case EGameType::Stand:
		Index = UKismetMathLibrary::RandomIntegerInRange(0, Targets.Num() - 1);
		if (Targets[Index] == nullptr)
			return;

		Targets[Index]->TargetTransform(true);
		break;
		
	case EGameType::Walk:
		if (NextTargetIndex >= Targets.Num())
			return;

		const int32 Condition = NextTargetIndex + TargetSetCount - 1;
		for (int32 i = NextTargetIndex; i <= Condition; i++)
		{
			if (Targets[i] == nullptr)
				return;

			Targets[i]->TargetTransform(true);
			NextTargetIndex++;
		}
		break;
	}
}

AShootingRangeCharacter::EGameType AShootingRangeCharacter::GetCurrentGameType()
{
	return CurrentGameType;
}

void AShootingRangeCharacter::SetCurrentGameType(EGameType NewGameType)
{
	CurrentGameType = NewGameType;
}

AShootingRangeCharacter::EProgressType AShootingRangeCharacter::GetCurrentProgressType()
{
	return CurrentProgressType;
}

void AShootingRangeCharacter::SetCurrentProgressType(EProgressType NewProgressType)
{
	CurrentProgressType = NewProgressType;
}

void AShootingRangeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShootingRangeCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShootingRangeCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("GameStart_Stand"), EInputEvent::IE_Pressed, this, &AShootingRangeCharacter::GameStart_Stand);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShootingRangeCharacter::FireInput);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShootingRangeCharacter::ReloadInput);
}

void AShootingRangeCharacter::MoveForward(float NewAxisValue)
{
	switch (CurrentGameType)
	{
	case EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;

	case EGameType::Walk:
		switch (CurrentProgressType)
		{
		case EProgressType::None:
			UE_LOG(ShootingRange, Error, TEXT("ProgressType is 'None'"));
			break;

		case EProgressType::Ready:
		case EProgressType::Play:
			AddMovementInput(GetActorForwardVector(), NewAxisValue);
			break;

		case EProgressType::End:
			break;
		}
		break;
	}
}

void AShootingRangeCharacter::MoveRight(float NewAxisValue)
{
	switch (CurrentGameType)
	{
	case EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;

	case EGameType::Walk:
		switch (CurrentProgressType)
		{
		case EProgressType::None:
			UE_LOG(ShootingRange, Error, TEXT("ProgressType is 'None'"));
			break;

		case EProgressType::Ready:
		case EProgressType::Play:
			AddMovementInput(GetActorRightVector(), NewAxisValue);
			break;

		case EProgressType::End:
			break;
		}
		break;
	}
}

void AShootingRangeCharacter::GameStart_Stand()
{
	switch (CurrentGameType)
	{
	case EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;
		
	case EGameType::Stand:
		switch (CurrentProgressType)
		{
		case EProgressType::None:
			UE_LOG(ShootingRange, Error, TEXT("ProgressType is 'None'"));
			break;
			
		case EProgressType::Ready:
			GamePlay();
			break;
		}
		break;
	}
}

void AShootingRangeCharacter::GameStart_Walk()
{
	switch (CurrentGameType)
	{
	case EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;
		
	case EGameType::Walk:
		switch (CurrentProgressType)
		{
		case EProgressType::None:
			UE_LOG(ShootingRange, Error, TEXT("ProgressType is 'None'"));
			break;

		case EProgressType::Ready:
			GamePlay();
			break;
		}
		break;
	}
}

void AShootingRangeCharacter::FireInput()
{
	switch (CurrentProgressType)
	{
	case EProgressType::None:
		UE_LOG(ShootingRange, Error, TEXT("ProgressType is 'None'"));
		break;
		
	case EProgressType::Play:
		if (IsFiring || IsReloading || !CanFiring)
			return;

		if (CurrentMagazineAmmoCount >= 1)
		{
			IsFiring = true;
			
			Fire(false);

			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([this]() -> void
			{
				IsFiring = false;
			});
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.33f, false);
		}
		else
		{
			Fire(true);
		}
		break;
	}
}

void AShootingRangeCharacter::Fire(bool IsEmptyFire)
{
	if (IsEmptyFire)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), EmptyFireSound);
		return;
	}

	CurrentMagazineAmmoCount--;
	HUD->UpdateCurrentMagazineAmmoCount(CurrentMagazineAmmoCount);

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, CharacterMesh, TEXT("MuzzleFlash"));
	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);

	FHitResult HitResult;
	FVector Start = Camera->GetComponentLocation();
	FVector End = (Camera->GetComponentLocation()) + (Camera->GetForwardVector() * 10000.0f);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam);
	if (!IsHit)
		return;

	AShootingRangeTarget* Target = Cast<AShootingRangeTarget>(HitResult.GetActor());
	if (!::IsValid(Target))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ConcreteImpact, HitResult.ImpactPoint, UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WoodImpact, HitResult.ImpactPoint, UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

	UBoxComponent* BoxComponent = Cast<UBoxComponent>(HitResult.GetComponent());
	if (BoxComponent == nullptr)
		return;

	FVector DistanceVector = HitResult.ImpactPoint - BoxComponent->GetComponentLocation();
	float DistanceLength = DistanceVector.Size();
	if (!Target->CanHit || DistanceLength > 45.0)
		return;

	switch (CurrentGameType)
	{
	case EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;
		
	case EGameType::Stand:
		if (DistanceLength <= 8.0f)
			HitProcess(Target, 15);
		else if (DistanceLength <= 19.5f)
			HitProcess(Target, 10);
		else
			HitProcess(Target, 5);
		break;
		
	case EGameType::Walk:
		HitProcess(Target, 0);
		break;
	}
}

void AShootingRangeCharacter::HitProcess(AShootingRangeTarget* Target, int32 Score)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	
	switch (CurrentGameType)
	{
	case EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;
		
	case EGameType::Stand:
		CurrentScoreRecord += Score;
		HUD->UpdateCurrentScoreRecord(CurrentScoreRecord);
		Target->TargetTransform(false);

		TimerDelegate.BindLambda([this]() -> void
		{
			TargetUp();
		});
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.3f, false);
		break;
		
	case EGameType::Walk:
		Target->TargetTransform(false);
		
		RemainTargetCount--;
		HUD->UpdateProgress(0, RemainTargetCount);
		
		HitProcessCount++;
		if (HitProcessCount != TargetSetCount)
			return;

		HitProcessCount = 0;
		TargetUp();
		break;
	}
}

void AShootingRangeCharacter::ReloadInput()
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	
	switch (CurrentProgressType)
	{
	case EProgressType::None:
		UE_LOG(ShootingRange, Error, TEXT("ProgressType is 'None'"));
		break;
		
	case EProgressType::Play:
		if (IsReloading || IsFiring || CurrentMagazineAmmoCount >= MaxMagazineAmmoCount)
			return;

		IsReloading = true;
		Reload();

		TimerDelegate.BindLambda([this]() -> void
		{
			IsReloading = false;
		});
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.87f, false);
		break;
	}
}

void AShootingRangeCharacter::Reload()
{
	CurrentMagazineAmmoCount = MaxMagazineAmmoCount;
	HUD->UpdateCurrentMagazineAmmoCount(CurrentMagazineAmmoCount);
	UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound);
}