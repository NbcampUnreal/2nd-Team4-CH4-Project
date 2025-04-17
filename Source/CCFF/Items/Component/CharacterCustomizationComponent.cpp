#include "Items/Component/CharacterCustomizationComponent.h"
#include "Character/Base/BaseCharacter.h"
#include "Character/Base/BasePreviewPawn.h"
#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/Character.h"
#include "Items/DataTable/CustomItemData.h"
#include "Items/Manager/CustomizationManager.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Subsystems/WorldSubsystem.h"

UCharacterCustomizationComponent::UCharacterCustomizationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UCharacterCustomizationComponent::BeginPlay()
{
	Super::BeginPlay();

    if (GetOwner()->HasAuthority())
    {
		UE_LOG(LogTemp, Warning, TEXT("CharacterCustomizationComponent BeginPlay on server"));
    }
	else
	{   
		UE_LOG(LogTemp, Warning, TEXT("CharacterCustomizationComponent BeginPlay on client"));
	}

    UCustomizationManager* CustomizationManager = Cast<UGameInstance>(GetOwner()->GetGameInstance())->GetSubsystem<UCustomizationManager>();
	if (CustomizationManager)
	{
		CustomizationManager->InitiailizeDataTable();
	}
}

void UCharacterCustomizationComponent::EquipItemByID(int32 ItemID, EItemSlot Slot)
{
	UDataTable* CustomItemDataTable = nullptr;

	UCustomizationManager* CustomizationManager = Cast<UGameInstance>(GetOwner()->GetGameInstance())->GetSubsystem<UCustomizationManager>();
    if (!CustomizationManager)
    {
        UE_LOG(LogTemp, Error, TEXT("CustomizationManager is null."));
        return;
    }
    switch (Slot)
    {
    case EItemSlot::Head:
		CustomItemDataTable = CustomizationManager->GetHeadCustomItemDataTable();
        break;
    case EItemSlot::Face:
		CustomItemDataTable = CustomizationManager->GetFaceCustomItemDataTable();
        break;
    case EItemSlot::Shoulder:
		CustomItemDataTable = CustomizationManager->GetShoulderCustomItemDataTable();
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Invalid item slot specified."));
        return;
    }

	if (!CustomItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CustomItemDataTable is null."));
		return;
	}

    static const FString ContextString(TEXT("Item Lookup"));
    FCustomItemData* FoundItem = CustomItemDataTable->FindRow<FCustomItemData>(FName(*FString::FromInt(ItemID)), ContextString);

    if (!FoundItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Item ID %d not found in DataTable."), ItemID);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[EquipItemByID] Equipping item: %s"), *FoundItem->ItemName.ToString());

    EquipItem(*FoundItem);
}

void UCharacterCustomizationComponent::EquipItem(FCustomItemData ItemData)
{
    
    if (ItemData.Slot == EItemSlot::None)
    {
        UE_LOG(LogTemp, Warning, TEXT("Item slot is None. Cannot equip."));
        return;
    }

    UnequipItem(ItemData.Slot);

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is not a valid Pawn."));
        return;
    }

    USkeletalMeshComponent* MeshComp = nullptr;
    if (ACharacter* Character = Cast<ACharacter>(OwnerPawn))
    {
        MeshComp = Character->GetMesh();
    }
    else
    {
        MeshComp = OwnerPawn->FindComponentByClass<USkeletalMeshComponent>();
    }

    UStaticMeshComponent* NewItemComponent = NewObject<UStaticMeshComponent>(OwnerPawn);
    if (NewItemComponent)
    {
        NewItemComponent->SetIsReplicated(true);
        UStaticMesh* ItemMesh = ItemData.ItemMesh.LoadSynchronous();
        NewItemComponent->SetStaticMesh(ItemMesh);
        NewItemComponent->RegisterComponent();
        NewItemComponent->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform, ItemData.SocketName);
    }
    

    EquippedItems.Add(ItemData.Slot, NewItemComponent);
}

void UCharacterCustomizationComponent::UnequipItem(EItemSlot Slot)
{
    if (UStaticMeshComponent** FoundItem = EquippedItems.Find(Slot))
    {
        (*FoundItem)->DestroyComponent();
        EquippedItems.Remove(Slot);
    }
}

void UCharacterCustomizationComponent::UnequipAllItems()
{
	for (auto& Item : EquippedItems)
	{
		if (Item.Value)
		{
			Item.Value->DestroyComponent();
		}
	}
	EquippedItems.Empty();
}

FName UCharacterCustomizationComponent::GetCharacterID() const
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn))
    {
        ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OwnerCharacter);
        if (BaseCharacter)
        {
            return FName(BaseCharacter->GetCharacterType());
        }
    }

    if (ABasePreviewPawn* PreviewPawn = Cast<ABasePreviewPawn>(OwnerPawn))
    {
        return PreviewPawn->GetCharacterID();
    }

    return TEXT("Unknown");
}

void UCharacterCustomizationComponent::EquipPreset(FCustomizationPreset Preset)
{
    UE_LOG(LogTemp, Warning, TEXT("[EquipPreset] EquippedItems Count: %d"), Preset.EquippedItems.Num());

    for (const FEquippedItemData& Item : Preset.EquippedItems)
	{
		FName ItemID = Item.ItemID;
		int32 ItemIDInt = FCString::Atoi(*ItemID.ToString());
		EquipItemByID(ItemIDInt, Item.EquipSlot);

        UE_LOG(LogTemp, Warning, TEXT("[EquipPreset] ItemID: %s, Slot: %s"), *Item.ItemID.ToString(), *UEnum::GetValueAsString(Item.EquipSlot));

	}
}

USkeletalMesh* UCharacterCustomizationComponent::GetBaseMeshByCharacterID(FName CharacterID) const
{
    if (USkeletalMesh* const* FoundMesh = CharacterMeshMap.Find(CharacterID))
    {
        return *FoundMesh;
    }

    UE_LOG(LogTemp, Warning, TEXT("[CustomizationComponent] No mesh found for CharacterID: %s"), *CharacterID.ToString());
    return nullptr;
}

UClass* UCharacterCustomizationComponent::GetAnimBlueprintByCharacterID(FName CharacterID) const
{
	if (UClass* const* FoundAnimBP = CharacterAnimBPMap.Find(CharacterID))
	{
		return *FoundAnimBP;
	}
	UE_LOG(LogTemp, Warning, TEXT("[CustomizationComponent] No AnimBP found for CharacterID: %s"), *CharacterID.ToString());
	return nullptr;
}

FPresetItemsIndex UCharacterCustomizationComponent::GetCurrentCustomItemsIndex() const
{
	FPresetItemsIndex PresetItemsIndex;
	for (const auto& Item : EquippedItems)
	{
		switch (Item.Key)
		{
		case EItemSlot::Head:
			PresetItemsIndex.HeadIndex = FCString::Atoi(*Item.Value->GetName());
			break;
		case EItemSlot::Face:
			PresetItemsIndex.FaceIndex = FCString::Atoi(*Item.Value->GetName());
			break;
		case EItemSlot::Shoulder:
			PresetItemsIndex.ShoulderIndex = FCString::Atoi(*Item.Value->GetName());
			break;
		default:
			break;
		}
	}
	return PresetItemsIndex;
}

void UCharacterCustomizationComponent::EquipPresetByIndex(int32 PresetIndex)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EquipPresetByIndex] Owner is not a character"));
		return;
	}

	APlayerState* PS = OwnerCharacter->GetPlayerState();
	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EquipPresetByIndex] PlayerState is null"));
		return;
	}

	AArenaPlayerState* ArenaPS = Cast<AArenaPlayerState>(PS);
	if (!ArenaPS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EquipPresetByIndex] PlayerState is not AArenaPlayerState"));
		return;
	}

	FName CharacterID = ArenaPS->GetSelectedCharacterID();
	const TArray<FCharacterCustomizationPreset>& Presets = ArenaPS->GetAllPresets();

	for (const auto& CharPreset : Presets)
	{
		if (CharPreset.CharacterID == CharacterID)
		{
			for (const auto& Preset : CharPreset.Presets)
			{
				if (Preset.PresetIndex == PresetIndex)
				{
					UE_LOG(LogTemp, Log, TEXT("[EquipPresetByIndex] Found Preset for ID: %s, Index: %d, Items: %d"),
						*CharacterID.ToString(), PresetIndex, Preset.EquippedItems.Num());

					EquipPreset(Preset);
					return;
				}
			}
		}
	}
}