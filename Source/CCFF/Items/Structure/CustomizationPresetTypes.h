#pragma once

#include "CoreMinimal.h"
#include "Items/DataAsset/CustomizationItemAsset.h"
#include "CustomizationPresetTypes.generated.h"


USTRUCT(BlueprintType)
struct FEquippedItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipSlot EquipSlot;
};

USTRUCT(BlueprintType)
struct FCustomizationPreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EEquipSlot, FEquippedItemData> EquippedItems;
};

USTRUCT(BlueprintType)
struct FCharacterCustomizationData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCustomizationPreset CurrentPreset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FCustomizationPreset> Presets;
};

class CCFF_API CustomizationPresetTypes{};
