#pragma once

#include "CoreMinimal.h"
#include "Items/DataTable/CustomItemData.h"
#include "CustomizationPreset.generated.h"

USTRUCT(BlueprintType)
struct FPresetItemsIndex
{
	GENERATED_BODY()

	UPROPERTY()
	int32 PresetIndex;

	UPROPERTY()
	int32 HeadIndex;

	UPROPERTY()
	int32 FaceIndex;

	UPROPERTY()
	int32 ShoulderIndex;

	FPresetItemsIndex()
		: PresetIndex(0), HeadIndex(0), FaceIndex(0), ShoulderIndex(0) {
	}
};

USTRUCT(BlueprintType)
struct FEquippedItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemSlot EquipSlot;

	FEquippedItemData()
		: ItemID(NAME_None), EquipSlot(EItemSlot::None) {
	}
};

USTRUCT(BlueprintType)
struct FCustomizationPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PresetIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEquippedItemData> EquippedItems;

	FCustomizationPreset()
		: PresetIndex(-1), EquippedItems() {
	}
};

USTRUCT(BlueprintType)
struct FCharacterCustomizationPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCustomizationPreset> Presets;

	FCharacterCustomizationPreset()
		: CharacterID(NAME_None), Presets() {
	}
};

class CCFF_API CustomizationPreset {};
