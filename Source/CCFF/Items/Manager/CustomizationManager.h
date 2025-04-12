#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Items/Structure/CustomizationPreset.h"
#include "CustomizationManager.generated.h"


class UDataTable;

UCLASS()
class CCFF_API UCustomizationManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	FORCEINLINE UDataTable* GetHeadCustomItemDataTable() const { return HeadCustomItemDataTable; }
	FORCEINLINE UDataTable* GetFaceCustomItemDataTable() const { return FaceCustomItemDataTable; }
	FORCEINLINE UDataTable* GetShoulderCustomItemDataTable() const { return ShoulderCustomItemDataTable; }
	FORCEINLINE FPresetItemsIndex GetAppliedPresetIndexes() const { return AppliedPresetIndexes; }

	FName GetItemName(int32 ItemID, EItemSlot Slot) const;
	FName GetCharacterID() const;
	FPresetItemsIndex GetPresetItemsIndex(FName CharacterID, int32 PresetIndex) const;
	void SavePreset(FPresetItemsIndex PresetIndexes);

	void LoadPreset(FName CharacterID, int32 PresetIndex);
	void LoadAllPresets();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UDataTable* HeadCustomItemDataTable;
	UDataTable* FaceCustomItemDataTable;
	UDataTable* ShoulderCustomItemDataTable;

	FPresetItemsIndex AppliedPresetIndexes;

	TArray <FCharacterCustomizationPreset> CharacterCustomizationPresets;
};
