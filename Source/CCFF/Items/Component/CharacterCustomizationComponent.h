#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/DataTable/CustomItemData.h"
#include "Items/Structure/CustomizationPreset.h"
#include "CharacterCustomizationComponent.generated.h"

class UDataTable;
class AMainMenuPlayerState;
class UCustomizationManager;

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

	FPresetItemsIndex GetCurrentCustomItemsIndex() const;

	//UFUNCTION(BlueprintCallable)
	//UStaticMeshComponent* GetEquippedItem(EItemSlot Slot) const;
	//UFUNCTION(BlueprintCallable)
	//TMap<EItemSlot, UStaticMeshComponent*> GetEquippedItems() const { return EquippedItems; }

protected:
    virtual void BeginPlay() override;



    UPROPERTY()
    TMap<EItemSlot, UStaticMeshComponent*> EquippedItems;

	UFUNCTION(Server, Reliable)
	void Server_SavePreset(APlayerController* PC, FName CharacterID, FPresetItemsIndex PresetIndexes);
	void Server_SavePreset_Implementation(APlayerController* PC, FName CharacterID, FPresetItemsIndex PresetIndexes);

	FName GetCharacterID()const;

private:
	UDataTable* HeadCustomItemDataTable;
	UDataTable* FaceCustomItemDataTable;
	UDataTable* ShoulderCustomItemDataTable;
	void InitializeCustomizationSystem();
	UCustomizationManager* CustomizationManager;

	void EquipPreset(int32 PresetIndex);
};
