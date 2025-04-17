#include "Items/Manager/CustomizationManager.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Items/DataTable/CustomItemData.h"
#include "Character/Base/BasePreviewPawn.h"
#include "Character/Base/BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void UCustomizationManager::Initialize(FSubsystemCollectionBase& Collection)
{
	InitiailizeDataTable();
}

void UCustomizationManager::InitiailizeDataTable()
{
	if (HeadCustomItemDataTable && FaceCustomItemDataTable && ShoulderCustomItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Data tables are already initialized."));
		return;
	}

	static const FString HeadItemDataTablePath = TEXT("/Game/CCFF/DataTables/DT_HeadCustomItemData.DT_HeadCustomItemData");
	HeadCustomItemDataTable = LoadObject<UDataTable>(nullptr, *HeadItemDataTablePath);
	static const FString FaceItemDataTablePath = TEXT("/Game/CCFF/DataTables/DT_FaceCustomItemData.DT_FaceCustomItemData");
	FaceCustomItemDataTable = LoadObject<UDataTable>(nullptr, *FaceItemDataTablePath);
	static const FString ShoulderItemDataTablePath = TEXT("/Game/CCFF/DataTables/DT_ShoulderCustomItemData.DT_ShoulderCustomItemData");
	ShoulderCustomItemDataTable = LoadObject<UDataTable>(nullptr, *ShoulderItemDataTablePath);

	if (HeadCustomItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeadCustomItemDataTable loaded with %d rows"), HeadCustomItemDataTable->GetRowMap().Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load HeadCustomItemDataTable."));
	}

	if (FaceCustomItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("FaceCustomItemDataTable loaded with %d rows"), FaceCustomItemDataTable->GetRowMap().Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load FaceCustomItemDataTable."));
	}

	if (ShoulderCustomItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShoulderCustomItemDataTable loaded with %d rows"), ShoulderCustomItemDataTable->GetRowMap().Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load ShoulderCustomItemDataTable."));
	}


	CountTotalItemNums();
}

void UCustomizationManager::CountTotalItemNums()
{
	if (HeadCustomItemDataTable)
	{
		TotalHeadItemNums = HeadCustomItemDataTable->GetRowNames().Num();
	}
	if (FaceCustomItemDataTable)
	{
		TotalFaceItemNums = FaceCustomItemDataTable->GetRowNames().Num();
	}
	if (ShoulderCustomItemDataTable)
	{
		TotalShoulderItemNums = ShoulderCustomItemDataTable->GetRowNames().Num();
	}
}

// Saving a preset in StandAlone LockerRoom
void UCustomizationManager::SavePreset(FPresetItemsIndex PresetIndexes)
{
	
	if (PresetIndexes.PresetIndex < 0 || PresetIndexes.PresetIndex > 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("SavePreset failed: PresetIndex is out of range."));
		return;
	}

	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (!LocalPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayerController is null."));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("PlayerController found: %s"), *LocalPlayerController->GetName());

	APawn* ControlledPawn = LocalPlayerController->GetPawn();
	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControlledPawn is null."));
		return;
	}

	ABasePreviewPawn* PreviewPawn = Cast<ABasePreviewPawn>(ControlledPawn);
	if (!PreviewPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControlledPawn is not ABasePreviewPawn."));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Controlled Pawn found: %s"), *PreviewPawn->GetName());

	UCharacterCustomizationComponent* CharacterCustomizationComponent = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (!CharacterCustomizationComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterCustomizationComponent not found."));
		return;
	}

	FName CharacterID = PreviewPawn->GetCharacterID();
	UE_LOG(LogTemp, Log, TEXT("Character ID: %s"), *CharacterID.ToString());

	// Create a new preset
	FCustomizationPreset NewPreset;
	NewPreset.PresetIndex = PresetIndexes.PresetIndex;

	FEquippedItemData HeadItem;
	HeadItem.ItemID = FName(*FString::FromInt(PresetIndexes.HeadIndex));
	HeadItem.EquipSlot = EItemSlot::Head;

	FEquippedItemData FaceItem;
	FaceItem.ItemID = FName(*FString::FromInt(PresetIndexes.FaceIndex));
	FaceItem.EquipSlot = EItemSlot::Face;

	FEquippedItemData ShoulderItem;
	ShoulderItem.ItemID = FName(*FString::FromInt(PresetIndexes.ShoulderIndex));
	ShoulderItem.EquipSlot = EItemSlot::Shoulder;

	NewPreset.EquippedItems.Add(HeadItem);
	NewPreset.EquippedItems.Add(FaceItem);
	NewPreset.EquippedItems.Add(ShoulderItem);

	// Find the character preset by ID
	FCharacterCustomizationPreset* FoundCharacterPreset = CharacterCustomizationPresets.FindByPredicate(
		[&](const FCharacterCustomizationPreset& Preset) {return Preset.CharacterID == CharacterID;});

	// Check if the character already has a preset
	if (FoundCharacterPreset)
	{
		// Check if the indexed preset already exists for this character
		FCustomizationPreset* ExistingPreset = FoundCharacterPreset->Presets.FindByPredicate(
			[&](const FCustomizationPreset& Preset) {return Preset.PresetIndex == NewPreset.PresetIndex;});

		// overwrite the existing preset or add a new one
		if (ExistingPreset)
		{
			*ExistingPreset = NewPreset;
			UE_LOG(LogTemp, Log, TEXT("Updated existing preset %d for character %s."), NewPreset.PresetIndex, *CharacterID.ToString());
		}
		else
		{
			FoundCharacterPreset->Presets.Add(NewPreset);
			UE_LOG(LogTemp, Log, TEXT("Added new preset %d for character %s."), NewPreset.PresetIndex, *CharacterID.ToString());
		}
	}

	// If the character doesn't exist, create a new one
	else
	{
		FCharacterCustomizationPreset NewCharacterPreset;
		NewCharacterPreset.CharacterID = CharacterID;
		NewCharacterPreset.Presets.Add(NewPreset);
		CharacterCustomizationPresets.Add(NewCharacterPreset);
		UE_LOG(LogTemp, Log, TEXT("Created new character entry and added preset %d for character %s."), NewPreset.PresetIndex, *CharacterID.ToString());
	}
	

	// Just For Logging Delete Later
	UE_LOG(LogTemp, Log, TEXT("===== All Character Presets ====="));

	const UEnum* EnumPtr = StaticEnum<EItemSlot>();
	for (const FCharacterCustomizationPreset& CharPreset : CharacterCustomizationPresets)
	{
		UE_LOG(LogTemp, Log, TEXT("¢º Character ID: %s"), *CharPreset.CharacterID.ToString());

		for (const FCustomizationPreset& Preset : CharPreset.Presets)
		{
			UE_LOG(LogTemp, Log, TEXT("  - Preset Index: %d"), Preset.PresetIndex);

			for (const FEquippedItemData& Item : Preset.EquippedItems)
			{
				FString SlotName = EnumPtr
					? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Item.EquipSlot)).ToString()
					: TEXT("Unknown");

				UE_LOG(LogTemp, Log, TEXT("    - Slot: %s, ItemID: %s"), *SlotName, *Item.ItemID.ToString());
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("=================================="));
}

void UCustomizationManager::LoadPreset(FName CharacterID, int32 PresetIndex)
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABasePreviewPawn::StaticClass());
	if (!FoundActor) return;
	ABasePreviewPawn* PreviewPawn = Cast<ABasePreviewPawn>(FoundActor);
	if (!PreviewPawn) return;
	UCharacterCustomizationComponent* CharacterCustomizationComponent = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (!CharacterCustomizationComponent) return;

	UE_LOG(LogTemp, Log, TEXT("Loading preset %d for character %s."), PresetIndex, *CharacterID.ToString());

	const FCharacterCustomizationPreset* FoundCharacterPreset = CharacterCustomizationPresets.FindByPredicate(
		[&](const FCharacterCustomizationPreset& CharPreset) {
			return CharPreset.CharacterID == CharacterID;
		});

	if (!FoundCharacterPreset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character with ID %s not found."), *CharacterID.ToString());
		
		CharacterCustomizationComponent->UnequipAllItems();
		AppliedPresetIndexes.HeadIndex = 0;
		AppliedPresetIndexes.FaceIndex = 0;
		AppliedPresetIndexes.ShoulderIndex = 0;
		return;
	}

	const FCustomizationPreset* FoundPreset = FoundCharacterPreset->Presets.FindByPredicate(
		[&](const FCustomizationPreset& Preset) {
			return Preset.PresetIndex == PresetIndex;
		});

	if (!FoundPreset)
	{
		CharacterCustomizationComponent->UnequipAllItems();
		AppliedPresetIndexes.HeadIndex = 0;
		AppliedPresetIndexes.FaceIndex = 0;
		AppliedPresetIndexes.ShoulderIndex = 0;
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Loaded preset %d for character %s."), PresetIndex, *CharacterID.ToString());

	for (const FEquippedItemData& Item : FoundPreset->EquippedItems)
	{
		FString NameString = Item.ItemID.ToString();
		int32 ItemID = FCString::Atoi(*NameString);
		EItemSlot ItemSlot = Item.EquipSlot;

		CharacterCustomizationComponent->EquipItemByID(ItemID, Item.EquipSlot);
	
		UE_LOG(LogTemp, Log, TEXT("  - Slot: %s, ItemID: %s"), *UEnum::GetValueAsString(Item.EquipSlot), *Item.ItemID.ToString());
		switch (Item.EquipSlot)
		{
		case EItemSlot::Head:
			AppliedPresetIndexes.HeadIndex = ItemID;
			break;
		case EItemSlot::Face:
			AppliedPresetIndexes.FaceIndex = ItemID;
			break;
		case EItemSlot::Shoulder:
			AppliedPresetIndexes.ShoulderIndex = ItemID;
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid item slot specified."));
			break;
		}
	}
}

FPresetItemsIndex UCustomizationManager::GetPresetItemsIndex(FName CharacterID, int32 PresetIndex) const
{
	FPresetItemsIndex PresetItemsIndex;
	PresetItemsIndex.PresetIndex = PresetIndex;
	const FCharacterCustomizationPreset* FoundCharacterPreset = CharacterCustomizationPresets.FindByPredicate(
		[&](const FCharacterCustomizationPreset& CharPreset) {return CharPreset.CharacterID == CharacterID; });
	if (FoundCharacterPreset)
	{
		const FCustomizationPreset* FoundPreset = FoundCharacterPreset->Presets.FindByPredicate(
			[&](const FCustomizationPreset& Preset) {return Preset.PresetIndex == PresetIndex; });
		if (FoundPreset)
		{
			for (const FEquippedItemData& Item : FoundPreset->EquippedItems)
			{
				FString NameString = Item.ItemID.ToString();
				int32 ItemID = FCString::Atoi(*NameString);
				switch (Item.EquipSlot)
				{
				case EItemSlot::Head:
					PresetItemsIndex.HeadIndex = ItemID;
					break;
				case EItemSlot::Face:
					PresetItemsIndex.FaceIndex = ItemID;
					break;
				case EItemSlot::Shoulder:
					PresetItemsIndex.ShoulderIndex = ItemID;
					break;
				default:
					break;
				}
			}
		}
	}
	return PresetItemsIndex;
}


FName UCustomizationManager::GetCharacterID() const
{
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (!LocalPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayerController is null."));
		return NAME_None;
	}
	APawn* ControlledPawn = LocalPlayerController->GetPawn();
	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControlledPawn is null."));
		return NAME_None;
	}

	ABasePreviewPawn* PreviewPawn = Cast<ABasePreviewPawn>(ControlledPawn);
	if (!PreviewPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControlledPawn is not ABasePreviewPawn."));
		return NAME_None;
	}
	return PreviewPawn->GetCharacterID();
}


FCustomizationPreset UCustomizationManager::GetCustomizationPreset(FName CharacterID, int32 PresetIndex) const
{
	const FCharacterCustomizationPreset* FoundCharacterPreset = CharacterCustomizationPresets.FindByPredicate(
		[&](const FCharacterCustomizationPreset& CharPreset) {return CharPreset.CharacterID == CharacterID; });
	if (FoundCharacterPreset)
	{
		const FCustomizationPreset* FoundPreset = FoundCharacterPreset->Presets.FindByPredicate(
			[&](const FCustomizationPreset& Preset) {return Preset.PresetIndex == PresetIndex; });
		if (FoundPreset)
		{
			return *FoundPreset;
		}
	}

	return FCustomizationPreset();	
}

FName UCustomizationManager::GetItemName(int32 ItemID, EItemSlot Slot) const
{
	FString ItemName;
	UDataTable* CustomItemDataTable = nullptr;
	switch (Slot)
	{
	case EItemSlot::Head:
		CustomItemDataTable = HeadCustomItemDataTable;
		break;
	case EItemSlot::Face:
		CustomItemDataTable = FaceCustomItemDataTable;
		break;
	case EItemSlot::Shoulder:
		CustomItemDataTable = ShoulderCustomItemDataTable;
		break;
	default:
		return FName("Invalid");
	}

	static const FString ContextString(TEXT("Item Lookup"));
	FCustomItemData* FoundItem = CustomItemDataTable->FindRow<FCustomItemData>(FName(*FString::FromInt(ItemID)), ContextString);
	if (FoundItem)
	{
		ItemName = FoundItem->ItemName.ToString();
	}
	else
	{
		ItemName = "Not Found";
	}
	return FName(*ItemName);
}

TSoftObjectPtr<UStaticMesh> UCustomizationManager::GetItemMesh(int32 ItemID, EItemSlot Slot) const
{
	UDataTable* CustomItemDataTable = nullptr;
	switch (Slot)
	{
	case EItemSlot::Head:
		CustomItemDataTable = HeadCustomItemDataTable;
		break;
	case EItemSlot::Face:
		CustomItemDataTable = FaceCustomItemDataTable;
		break;
	case EItemSlot::Shoulder:
		CustomItemDataTable = ShoulderCustomItemDataTable;
		break;
	default:
		return TSoftObjectPtr<UStaticMesh>(nullptr);
	}
	static const FString ContextString(TEXT("Item Lookup"));
	FCustomItemData* FoundItem = CustomItemDataTable->FindRow<FCustomItemData>(FName(*FString::FromInt(ItemID)), ContextString);
	if (FoundItem)
	{
		return FoundItem->ItemMesh;
	}
	else
	{
		return TSoftObjectPtr<UStaticMesh>(nullptr);
	}
}