#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/DataTable/CustomItemData.h"
#include "CharacterCustomizationComponent.generated.h"

class UDataTable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CCFF_API UCharacterCustomizationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterCustomizationComponent();

    UFUNCTION()
	void EquipItemByID(int32 ItemID);
    UFUNCTION(BlueprintCallable)
	void EquipItem(FCustomItemData ItemData);

    UFUNCTION(BlueprintCallable)
    void UnequipItem(EItemSlot Slot);

protected:
    virtual void BeginPlay() override;

    UDataTable* CustomItemDataTable;

    UPROPERTY()
    TMap<EItemSlot, UStaticMeshComponent*> EquippedItems;
};
