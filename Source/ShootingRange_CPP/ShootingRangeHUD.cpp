#include "ShootingRangeHUD.h"
#include "ShootingRange_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "ShootingRangeCharacter.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UShootingRangeHUD::NativeConstruct()
{
	Super::NativeConstruct();

	RetryButton->OnClicked.AddDynamic(this, &UShootingRangeHUD::RetryGame);
	QuitButton->OnClicked.AddDynamic(this, &UShootingRangeHUD::QuitGame);
}

void UShootingRangeHUD::HUDReady()
{
	ShootingRangeCharacter = Cast<AShootingRangeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	// Hidden
	CrosshairImage->SetVisibility(ESlateVisibility::Hidden);
	AmmoBorder->SetVisibility(ESlateVisibility::Hidden);
	ProgressBorder->SetVisibility(ESlateVisibility::Hidden);
	BestRecordBorder->SetVisibility(ESlateVisibility::Hidden);
	CurrentRecordBorder->SetVisibility(ESlateVisibility::Hidden);
	RetryButton->SetVisibility(ESlateVisibility::Hidden);
	QuitButton->SetVisibility(ESlateVisibility::Hidden);

	// Visible
	InfoBorder->SetVisibility(ESlateVisibility::Visible);

	switch (ShootingRangeCharacter->GetCurrentGameType())
	{
	case AShootingRangeCharacter::EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None"));
		break;

	case AShootingRangeCharacter::EGameType::Stand:
		InfoText->SetText(FText::FromString(TEXT("[스페이스 바]를 누르면 게임이 시작됩니다")));
		break;

	case AShootingRangeCharacter::EGameType::Walk:
		InfoText->SetText(FText::FromString(TEXT("START 지점을 통과하면 게임이 시작됩니다")));
		break;
	}
}

void UShootingRangeHUD::HUDPlay(int32 CurrentMagazineAmmoCount, int32 BestScoreRecord, int32 CurrentScoreRecord,
	float BestTimeRecord, float CurrentTimeRecord, int32 RemainTime, int32 RemainTargetCount)
{
	UpdateProgress(RemainTime, RemainTargetCount);
	UpdateCurrentMagazineAmmoCount(CurrentMagazineAmmoCount);

	switch (ShootingRangeCharacter->GetCurrentGameType())
	{
	case AShootingRangeCharacter::EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None"));
		break;
		
	case AShootingRangeCharacter::EGameType::Stand:
		UpdateBestScoreRecord(BestScoreRecord);
		UpdateCurrentScoreRecord(CurrentScoreRecord);
		break;
		
	case AShootingRangeCharacter::EGameType::Walk:
		UpdateBestTimeRecord(BestTimeRecord);
		UpdateCurrentTimeRecord(CurrentTimeRecord);
		break;
	}

	// Visible
	CrosshairImage->SetVisibility(ESlateVisibility::Visible);
	AmmoBorder->SetVisibility(ESlateVisibility::Visible);
	ProgressBorder->SetVisibility(ESlateVisibility::Visible);
	BestRecordBorder->SetVisibility(ESlateVisibility::Visible);
	CurrentRecordBorder->SetVisibility(ESlateVisibility::Visible);

	// Hidden
	InfoBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UShootingRangeHUD::HUDEnd(int32 BestScoreRecord, int32 CurrentScoreRecord, float BestTimeRecord,
	float CurrentTimeRecord)
{
	FString AppendString;
	FText AppendText;
	
	switch (ShootingRangeCharacter->GetCurrentGameType())
	{
	case AShootingRangeCharacter::EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None"));
		break;
		
	case AShootingRangeCharacter::EGameType::Stand:
		AppendString = TEXT("게임 결과\n최고 점수 기록 : ");
		AppendString.Append(FString::FromInt(BestScoreRecord));
		AppendString.Append(TEXT(" 점\n현재 점수 기록 : "));
		AppendString.Append(FString::FromInt(CurrentScoreRecord));
		AppendString.Append(TEXT(" 점"));

		InfoText->SetText(FText::FromString(AppendString));
		break;
		
	case AShootingRangeCharacter::EGameType::Walk:
		AppendString = TEXT("게임 결과\n최고 시간 기록 : ");
		AppendText = UKismetTextLibrary::Conv_FloatToText(BestTimeRecord, ERoundingMode::HalfToEven, false, true, 1, 324, 2, 2);
		AppendString.Append(AppendText.ToString());
		AppendString.Append(TEXT(" 초\n현재 시간 기록 : "));
		AppendText = UKismetTextLibrary::Conv_FloatToText(CurrentTimeRecord, ERoundingMode::HalfToEven, false, true, 1, 324, 2, 2);
		AppendString.Append(AppendText.ToString());
		AppendString.Append(TEXT(" 초"));

		InfoText->SetText(FText::FromString(AppendString));
		break;
	}

	// Hidden
	CrosshairImage->SetVisibility(ESlateVisibility::Hidden);
	AmmoBorder->SetVisibility(ESlateVisibility::Hidden);
	ProgressBorder->SetVisibility(ESlateVisibility::Hidden);
	BestRecordBorder->SetVisibility(ESlateVisibility::Hidden);
	CurrentRecordBorder->SetVisibility(ESlateVisibility::Hidden);

	// Visible
	InfoBorder->SetVisibility(ESlateVisibility::Visible);
	RetryButton->SetVisibility(ESlateVisibility::Visible);
	QuitButton->SetVisibility(ESlateVisibility::Visible);
}

void UShootingRangeHUD::UpdateProgress(int32 RemainTime, int32 RemainTargetCount)
{
	FString AppendString;
	
	switch (ShootingRangeCharacter->GetCurrentGameType())
	{
	case AShootingRangeCharacter::EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;
		
	case AShootingRangeCharacter::EGameType::Stand:
		AppendString = TEXT("남은 시간 : ");
		AppendString.Append(FString::FromInt(RemainTime));
		AppendString.Append(TEXT(" 초"));
		ProgressText->SetText(FText::FromString(AppendString));
		break;
		
	case AShootingRangeCharacter::EGameType::Walk:
		AppendString = TEXT("남은 타겟 수 : ");
		AppendString.Append(FString::FromInt(RemainTargetCount));
		AppendString.Append(TEXT(" 개"));
		ProgressText->SetText(FText::FromString(AppendString));
		break;
	}
}

void UShootingRangeHUD::UpdateCurrentMagazineAmmoCount(int32 CurrentMagazineAmmoCount)
{
	FString AppendString = FString::FromInt(CurrentMagazineAmmoCount).Append(TEXT(" / ∞"));
	AmmoText->SetText(FText::FromString(AppendString));
}

void UShootingRangeHUD::UpdateBestScoreRecord(int32 BestScoreRecord)
{
	FString AppendString = TEXT("최고 점수 기록 : ");
	AppendString.Append(FString::FromInt(BestScoreRecord));
	AppendString.Append(TEXT(" 점"));
	BestRecordText->SetText(FText::FromString(AppendString));
}

void UShootingRangeHUD::UpdateCurrentScoreRecord(int32 CurrentScoreRecord)
{
	FString AppendString = TEXT("현재 점수 기록 : ");
	AppendString.Append(FString::FromInt(CurrentScoreRecord));
	AppendString.Append(TEXT(" 점"));
	CurrentRecordText->SetText(FText::FromString(AppendString));
}

void UShootingRangeHUD::UpdateBestTimeRecord(float BestTimeRecord)
{
	if (FMath::IsNearlyEqual(BestTimeRecord, 0.0f, 0.001f))
	{
		BestRecordText->SetText(FText::FromString(TEXT("최고 시간 기록 : 없음")));
	}
	else
	{
		FText AppendText = UKismetTextLibrary::Conv_FloatToText(BestTimeRecord, ERoundingMode::HalfToEven, false, true, 1, 324, 2, 2);
		FString AppendString = TEXT("최고 시간 기록 : ");
		AppendString.Append(AppendText.ToString());
		AppendString.Append(TEXT(" 초"));
		BestRecordText->SetText(FText::FromString(AppendString));
	}
}

void UShootingRangeHUD::UpdateCurrentTimeRecord(float CurrentTimeRecord)
{
	FText AppendText = UKismetTextLibrary::Conv_FloatToText(CurrentTimeRecord, ERoundingMode::HalfToEven, false, true, 1, 324, 2, 2);
	FString AppendString = TEXT("현재 시간 기록 : ");
	AppendString.Append(AppendText.ToString());
	AppendString.Append(TEXT(" 초"));
	CurrentRecordText->SetText(FText::FromString(AppendString));
}

void UShootingRangeHUD::RetryGame()
{
	switch (ShootingRangeCharacter->GetCurrentGameType())
	{
	case AShootingRangeCharacter::EGameType::None:
		UE_LOG(ShootingRange, Error, TEXT("GameType is 'None'"));
		break;
		
	case AShootingRangeCharacter::EGameType::Stand:
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("ShootingRange_Stand"));
		break;
		
	case AShootingRangeCharacter::EGameType::Walk:
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("ShootingRange_Walk"));
		break;
	}
}

void UShootingRangeHUD::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}