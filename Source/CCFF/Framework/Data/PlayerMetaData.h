#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/AudioSettingsWidget.h"
#include "PlayerMetaData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 SaveVersion = 1;	// ���� ���� ���� �� �񱳿�. GameInstance���� �ε� ���� �Ǵܿ� ���.

	// Local Login System
	UPROPERTY(BlueprintReadWrite)
	FString PlayerUniqueID;

	UPROPERTY(BlueprintReadWrite)
	FString Password;

	// InGame
	UPROPERTY(BlueprintReadWrite)
	FString Nickname;

	UPROPERTY(BlueprintReadWrite)
	FName SelectedCharacterID = "Cactus";

	// Audio Setting
	UPROPERTY(BlueprintReadWrite)
	FAudioUserSettings AudioSettings;

	// Graphic Setting
	UPROPERTY(BlueprintReadWrite)
	FIntPoint Resolution = FIntPoint(1280, 720);

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::Windowed;
};
