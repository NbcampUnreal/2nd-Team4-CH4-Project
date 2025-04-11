#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/DataTable/CustomItemData.h"
#include "Items/Structure/CustomizationPreset.h"
#include "CharacterCustomizationComponent.generated.h"

class UDataTable;
class AMainMenuPlayerState;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CCFF_API UCharacterCustomizationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterCustomizationComponent();
    UFUNCTION()
	void EquipItemByID(int32 ItemID, EItemSlot Slot);
    UFUNCTION(BlueprintCallable)
	void EquipItem(FCustomItemData ItemData);
    UFUNCTION(BlueprintCallable)
    void UnequipItem(EItemSlot Slot);
	UFUNCTION(BlueprintCallable)    
	void UnequipAllItems();
	UFUNCTION(BlueprintCallable)
	void SavePreset(FPresetItemsindex PresetIndexes);

	//UFUNCTION(BlueprintCallable)
	//UStaticMeshComponent* GetEquippedItem(EItemSlot Slot) const;
	//UFUNCTION(BlueprintCallable)
	//TMap<EItemSlot, UStaticMeshComponent*> GetEquippedItems() const { return EquippedItems; }

protected:
    virtual void BeginPlay() override;

    UDataTable* HeadCustomItemDataTable;
	UDataTable* FaceCustomItemDataTable;
	UDataTable* ShoulderCustomItemDataTable;

    UPROPERTY()
    TMap<EItemSlot, UStaticMeshComponent*> EquippedItems;

	UFUNCTION(Server, Reliable)
	void Server_SavePreset(APlayerController* PC, FName CharacterID, FPresetItemsindex PresetIndexes);
	void Server_SavePreset_Implementation(APlayerController* PC, FName CharacterID, FPresetItemsindex PresetIndexes);

	FName GetCharacterID()const;
};
