#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootingRangeHUD.generated.h"

UCLASS()
class SHOOTINGRANGE_CPP_API UShootingRangeHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void HUDReady();
	void HUDPlay(int32 CurrentMagazineAmmoCount, int32 BestScoreRecord, int32 CurrentScoreRecord, float BestTimeRecord, float CurrentTimeRecord, int32 RemainTime, int32 RemainTargetCount);
	void HUDEnd(int32 BestScoreRecord, int32 CurrentScoreRecord, float BestTimeRecord, float CurrentTimeRecord);

	void UpdateProgress(int32 RemainTime, int32 RemainTargetCount);
	void UpdateCurrentMagazineAmmoCount(int32 CurrentMagazineAmmoCount);
	void UpdateBestScoreRecord(int32 BestScoreRecord);
	void UpdateCurrentScoreRecord(int32 CurrentScoreRecord);
	void UpdateBestTimeRecord(float BestTimeRecord);
	void UpdateCurrentTimeRecord(float CurrentTimeRecord);

	UFUNCTION()
	void RetryGame();

	UFUNCTION()
	void QuitGame();
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class AShootingRangeCharacter> ShootingRangeCharacter;

	UPROPERTY(meta = (BindWidget))
	class UBorder* AmmoBorder;

	UPROPERTY(meta = (BindWidget))
	class UBorder* BestRecordBorder;

	UPROPERTY(meta = (BindWidget))
	class UImage* CrosshairImage;

	UPROPERTY(meta = (BindWidget))
	class UBorder* CurrentRecordBorder;

	UPROPERTY(meta = (BindWidget))
	class UBorder* InfoBorder;

	UPROPERTY(meta = (BindWidget))
	class UBorder* ProgressBorder;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RetryButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BestRecordText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentRecordText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InfoText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProgressText;
};