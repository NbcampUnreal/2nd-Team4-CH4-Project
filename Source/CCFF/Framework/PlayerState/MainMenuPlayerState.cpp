#include "Framework/PlayerState/MainMenuPlayerState.h"
#include "Items/DataAsset/CustomizationItemAsset.h"

void AMainMenuPlayerState::SetEquippedItem(FName CharacterID, EEquipSlot Slot, FName ItemID)
{
    // Assign an item to a specific equip slot for a character
    if (Slot == EEquipSlot::None) return;

    FEquippedItemData NewItemData;
    NewItemData.ItemID = ItemID;
    NewItemData.EquipSlot = Slot;

    FCharacterCustomizationData& CustomData = CharacterCustomizations.FindOrAdd(CharacterID);
    CustomData.CurrentPreset.EquippedItems.Add(Slot, NewItemData);
}

void AMainMenuPlayerState::RemoveEquippedItem(FName CharacterID, EEquipSlot Slot)
{
    // Remove the equipped item from the specified slot for a character
    if (Slot == EEquipSlot::None) return;

    if (FCharacterCustomizationData* CustomData = CharacterCustomizations.Find(CharacterID))
    {
        CustomData->CurrentPreset.EquippedItems.Remove(Slot);
    }
}

FName AMainMenuPlayerState::GetEquippedItemID(FName CharacterID, EEquipSlot Slot) const
{
    // Return the ItemID currently equipped in a given slot for a character
    if (Slot == EEquipSlot::None) return NAME_None;

    const FCharacterCustomizationData* CustomData = CharacterCustomizations.Find(CharacterID);
    if (CustomData)
    {
        const FEquippedItemData* ItemData = CustomData->CurrentPreset.EquippedItems.Find(Slot);
        if (ItemData)
        {
            return ItemData->ItemID;
        }
    }

    return NAME_None;
}

void AMainMenuPlayerState::SavePreset(FName CharacterID, int32 PresetIndex)
{
    // Save the current preset to a specific index for a character
    if (!CharacterCustomizations.Contains(CharacterID)) return;

    FCharacterCustomizationData& CustomData = CharacterCustomizations[CharacterID];
    if (CustomData.Presets.Num() <= PresetIndex)
    {
        CustomData.Presets.SetNum(PresetIndex + 1);
    }

    CustomData.Presets[PresetIndex] = CustomData.CurrentPreset;
}

void AMainMenuPlayerState::LoadPreset(FName CharacterID, int32 PresetIndex)
{
    // Load a saved preset from a specific index and set it as current for a character
    if (!CharacterCustomizations.Contains(CharacterID)) return;

    FCharacterCustomizationData& CustomData = CharacterCustomizations[CharacterID];
    if (CustomData.Presets.IsValidIndex(PresetIndex))
    {
        CustomData.CurrentPreset = CustomData.Presets[PresetIndex];
    }
}

const FCustomizationPreset* AMainMenuPlayerState::GetCurrentPreset(FName CharacterID) const
{
    // Return the pointer to the current preset for a given character
    const FCharacterCustomizationData* CustomData = CharacterCustomizations.Find(CharacterID);
    return CustomData ? &CustomData->CurrentPreset : nullptr;
}
