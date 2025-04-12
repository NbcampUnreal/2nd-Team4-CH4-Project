#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/DataTable/CustomItemData.h"
#include "Items/Structure/CustomizationPreset.h"
#include "CharacterCustomizationComponent.generated.h"

class UDataTable;
class AMainMenuPlayerState;
class UCustomizationManager;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	// Character base mesh accessor
	UFUNCTION(BlueprintCallable)
	USkeletalMesh* GetBaseMeshByCharacterID(FName CharacterID) const;

protected:
    virtual void BeginPlay() override;



    UPROPERTY()
    TMap<EItemSlot, UStaticMeshComponent*> EquippedItems;
	FName GetCharacterID()const;

private:
	UDataTable* HeadCustomItemDataTable;
	UDataTable* FaceCustomItemDataTable;
	UDataTable* ShoulderCustomItemDataTable;
	void InitializeCustomizationSystem();
	UCustomizationManager* CustomizationManager;

	void EquipPreset(FCustomizationPreset Preset);

	// Character ID �� SkeletalMesh map
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TMap<FName, USkeletalMesh*> CharacterMeshMap;
};
