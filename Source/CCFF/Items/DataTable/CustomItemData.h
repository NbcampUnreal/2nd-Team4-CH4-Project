#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CustomItemData.generated.h"

UENUM(BlueprintType)
enum class EItemSlot : uint8
{
    None     UMETA(DisplayName = "None"),
    Head     UMETA(DisplayName = "Head"),
    Face     UMETA(DisplayName = "Face"),
    Shoulder UMETA(DisplayName = "Shoulder")
};

USTRUCT(BlueprintType)
struct FCustomItemData : public FTableRowBase
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemSlot Slot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UStaticMesh> ItemMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SocketName;

    FCustomItemData()
        : ItemID(0)
        , ItemName(NAME_None)
        , Slot(EItemSlot::None)
        , ItemMesh(nullptr)
        , SocketName(NAME_None)
    {
    }
};
