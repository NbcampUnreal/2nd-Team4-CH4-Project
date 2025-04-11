#pragma once

#include "CoreMinimal.h"
#include "Items/DataTable/CustomItemData.h"
#include "CustomizationPreset.generated.h"


USTRUCT(BlueprintType)
struct FPresetItemsindex
{
	GENERATED_BODY()

	UPROPERTY()
	int32 PresetIndex = 0;
	UPROPERTY()
	int32 HeadIndex = 0;
	UPROPERTY()
	int32 FaceIndex = 0;
	UPROPERTY()
	int32 ShoulderIndex = 0;
};

USTRUCT(BlueprintType)
struct FEquippedItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemSlot EquipSlot;
};

USTRUCT(BlueprintType)
struct FCustomizationPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PresetIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEquippedItemData> EquippedItems;
};

USTRUCT(BlueprintType)
struct FCharacterCustomizationPreset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCustomizationPreset> Presets;
};

class CCFF_API CustomizationPreset {};
