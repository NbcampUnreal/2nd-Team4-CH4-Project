// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CustomizationItemAsset.generated.h"

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
    Head     UMETA(DisplayName = "Head"),
	Face	 UMETA(DisplayName = "Face"),
    Shoulder UMETA(DisplayName = "Shoulder"),
};

UCLASS()
class CCFF_API UCustomizationItemAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Customization")
    FName ItemID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Customization")
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Customization")
    EEquipSlot EquipSlot;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Customization")
    UStaticMesh* Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Customization")
    FName AttachSocketName;
};
