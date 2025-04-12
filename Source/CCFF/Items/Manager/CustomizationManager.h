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

	FORCEINLINE int32 GetTotalHeadItems() const { return TotalHeadItemNums; }
	FORCEINLINE int32 GetTotalFaceItems() const { return TotalFaceItemNums; }
	FORCEINLINE int32 GetTotalShoulderItems() const { return TotalShoulderItemNums; }

	FORCEINLINE FPresetItemsIndex GetAppliedPresetIndexes() const { return AppliedPresetIndexes; }

	TArray<FCharacterCustomizationPreset> GetCharacterCustomizationPresets() const { return CharacterCustomizationPresets; }

	FName GetItemName(int32 ItemID, EItemSlot Slot) const;
	FName GetCharacterID() const;
	TSoftObjectPtr<UStaticMesh> GetItemMesh(int32 ItemID, EItemSlot Slot) const;

	FPresetItemsIndex GetPresetItemsIndex(FName CharacterID, int32 PresetIndex) const;
	
	void SavePreset(FPresetItemsIndex PresetIndexes);
	void LoadPreset(FName CharacterID, int32 PresetIndex);
	/*void LoadAllPresets();*/

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UDataTable* HeadCustomItemDataTable;
	UDataTable* FaceCustomItemDataTable;
	UDataTable* ShoulderCustomItemDataTable;

	int32 TotalHeadItemNums = 0;
	int32 TotalFaceItemNums = 0;
	int32 TotalShoulderItemNums = 0;

	FPresetItemsIndex AppliedPresetIndexes;
	
	TArray <FCharacterCustomizationPreset> CharacterCustomizationPresets;

	void CountTotalItemNums();
};
