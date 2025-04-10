#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Items/Structure/CustomizationPresetTypes.h"
#include "MainMenuPlayerState.generated.h"

class UCustomizationItemAsset;

UCLASS()
class CCFF_API AMainMenuPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    // Customization Presets Saved Data
    UPROPERTY(BlueprintReadWrite)
    TMap<FName, FCharacterCustomizationData> CharacterCustomizations;

	// Currently Selected Character ID
    UPROPERTY(BlueprintReadWrite)
    FName CurrentCharacterID;

	// Item Equip and Unequip
    UFUNCTION(BlueprintCallable)
    void SetEquippedItem(FName CharacterID, EEquipSlot Slot, FName ItemID);
    UFUNCTION(BlueprintCallable)
    void RemoveEquippedItem(FName CharacterID, EEquipSlot Slot);

    UFUNCTION(BlueprintCallable)
    FName GetEquippedItemID(FName CharacterID, EEquipSlot Slot) const;

	// Saving Preset and Loading Preset of the Character
    UFUNCTION(BlueprintCallable)
    void SavePreset(FName CharacterID, int32 PresetIndex);
    UFUNCTION(BlueprintCallable)
    void LoadPreset(FName CharacterID, int32 PresetIndex);

    // Get Preset Data of the Character
    const FCustomizationPreset* GetCurrentPreset(FName CharacterID) const;
};
