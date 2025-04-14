#pragma once

#include "CoreMinimal.h"
#include "AudioSettingsStructs.generated.h"

USTRUCT(BlueprintType)
struct FAudioCategorySetting
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMuted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Volume = 1.0f; // 0.0 ~ 1.0
};

USTRUCT(BlueprintType)
struct FAudioUserSettings
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAudioCategorySetting Master;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAudioCategorySetting BGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAudioCategorySetting SFX;
};
