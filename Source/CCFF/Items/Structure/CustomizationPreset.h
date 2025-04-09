#pragma once

#include "CoreMinimal.h"
#include "Items/DataAsset/CustomizationItemAsset.h"
#include "CustomizationPreset.generated.h"

USTRUCT(BlueprintType)
struct FCustomizationPreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UCustomizationItemAsset> Head;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UCustomizationItemAsset> Face;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UCustomizationItemAsset> Shoulder;

    // 기본 생성자: 모두 nullptr
    FCustomizationPreset()
        : Head(nullptr), Face(nullptr), Shoulder(nullptr)
    {
    }
};