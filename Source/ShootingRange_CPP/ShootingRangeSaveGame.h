#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShootingRangeSaveGame.generated.h"

UCLASS()
class SHOOTINGRANGE_CPP_API UShootingRangeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category=Record)
	int32 BestScoreRecord;
	
	UPROPERTY(VisibleAnywhere, Category=Record)
	float BestTimeRecord;
};