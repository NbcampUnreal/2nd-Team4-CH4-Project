// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/DataTable/CustomItemData.h"
#include "CustomizationPreset.generated.h"

USTRUCT(BlueprintType)
struct FEquippedItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemSlot EquipSlot;
};

class CCFF_API CustomizationPreset {};
