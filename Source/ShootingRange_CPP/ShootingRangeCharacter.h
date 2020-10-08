#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShootingRangeCharacter.generated.h"

UCLASS()
class SHOOTINGRANGE_CPP_API AShootingRangeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShootingRangeCharacter();

protected:
	virtual void BeginPlay() override;

public:
	enum class EGameType
	{
		None,
		Stand,
		Walk
	};

	EGameType GetCurrentGameType();
	void SetCurrentGameType(EGameType NewGameType);

	enum class EProgressType
	{
		None,
		Ready,
		Play,
		End
	};

	EProgressType GetCurrentProgressType();
	void SetCurrentProgressType(EProgressType NewProgressType);

protected:
	EGameType CurrentGameType = EGameType::None;
	EProgressType CurrentProgressType = EProgressType::None;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category=Visual)
	class USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* Camera;

	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void GameStart_Stand();
	void GameStart_Walk();
	void FireInput();
	void Fire(bool IsEmptyFire);
	void HitProcess(class AShootingRangeTarget* Target, int32 Score);
	void ReloadInput();
	void Reload();

	void SetInitialGameType();
	void TargetUp();
	
	void GameReady();
	void GamePlay();
	void GameEnd();

private:
	UPROPERTY(EditAnywhere, Category=Class)
	class TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere, Category=Montage)
	class UAnimMontage* DrawMontage;

	UPROPERTY(EditAnywhere, Category=Montage)
	class UAnimMontage* HideMontage;

	UPROPERTY(EditAnywhere, Category=Sound)
	class USoundBase* EmptyFireSound;

	UPROPERTY(EditAnywhere, Category=Sound)
	class USoundBase* FireSound;
	
	UPROPERTY(EditAnywhere, Category=Effect)
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category=Effect)
	class UParticleSystem* WoodImpact;

	UPROPERTY(EditAnywhere, Category=Effect)
	class UParticleSystem* ConcreteImpact;

	UPROPERTY(EditAnywhere, Category=Sound)
	class USoundBase* ReloadSound;
	
	UPROPERTY(EditAnywhere, Category=Record)
	float LimitTime;

	UPROPERTY(VisibleInstanceOnly, Category=Record)
	float PassedTime;

	UPROPERTY(VisibleInstanceOnly, Category=Record)
	int32 BestScoreRecord;

	UPROPERTY(VisibleInstanceOnly, Category=Record)
	int32 CurrentScoreRecord;
	
	UPROPERTY(VisibleInstanceOnly, Category=Record)
	float BestTimeRecord;

	UPROPERTY(VisibleInstanceOnly, Category=Record)
	float CurrentTimeRecord;

	UPROPERTY(EditAnywhere, Category=Ammo)
	int32 MaxMagazineAmmoCount;

	UPROPERTY(EditAnywhere, Category=Ammo)
	int32 CurrentMagazineAmmoCount;
	
	UPROPERTY(VisibleInstanceOnly, Category=Reference)
	class UShootingRangeHUD* HUD;

	UPROPERTY(EditAnywhere, Category=TargetManagement)
	int32 TargetSetCount;

	UPROPERTY(VisibleInstanceOnly, Category=TargetManagement)
	int32 NextTargetIndex;

	UPROPERTY(VisibleInstanceOnly, Category=TargetManagement)
	int32 HitProcessCount;

public:
	UPROPERTY(EditInstanceOnly, Category=Reference)
	TArray<class AShootingRangeTarget*> Targets;

	UPROPERTY(VisibleInstanceOnly, Category=Condition)
	bool CanFiring;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category=Condition)
	bool IsFiring;

	UPROPERTY(VisibleInstanceOnly, Category=Condition)
	bool IsReloading;

	UPROPERTY(VisibleInstanceOnly, Category=Record)
	int32 RemainTargetCount;
};