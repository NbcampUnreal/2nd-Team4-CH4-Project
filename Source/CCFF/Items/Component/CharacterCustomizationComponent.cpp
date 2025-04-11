#include "Items/Component/CharacterCustomizationComponent.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/Base/BaseCharacter.h"
#include "Character/Base/BasePreviewPawn.h"
#include "Items/DataTable/CustomItemData.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Framework/PlayerState/MainMenuPlayerState.h"
#include "Engine/DataTable.h"

UCharacterCustomizationComponent::UCharacterCustomizationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterCustomizationComponent::BeginPlay()
{
	Super::BeginPlay();

	static const FString HeadItemDataTablePath = TEXT("/Game/CCFF/DataTables/DT_HeadCustomItemData.DT_HeadCustomItemData");
	HeadCustomItemDataTable = LoadObject<UDataTable>(nullptr, *HeadItemDataTablePath);
	static const FString FaceItemDataTablePath = TEXT("/Game/CCFF/DataTables/DT_FaceCustomItemData.DT_FaceCustomItemData");
	FaceCustomItemDataTable = LoadObject<UDataTable>(nullptr, *FaceItemDataTablePath);
	static const FString ShoulderItemDataTablePath = TEXT("/Game/CCFF/DataTables/DT_ShoulderCustomItemData.DT_ShoulderCustomItemData");
	ShoulderCustomItemDataTable = LoadObject<UDataTable>(nullptr, *ShoulderItemDataTablePath);
}

void UCharacterCustomizationComponent::EquipItemByID(int32 ItemID, EItemSlot Slot)
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
        UE_LOG(LogTemp, Warning, TEXT("Invalid item slot specified."));
        return;
    }

    static const FString ContextString(TEXT("Item Lookup"));
    FCustomItemData* FoundItem = CustomItemDataTable->FindRow<FCustomItemData>(FName(*FString::FromInt(ItemID)), ContextString);

    if (!FoundItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Item ID %d not found in DataTable."), ItemID);
        return;
    }

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

void UCharacterCustomizationComponent::SavePreset(FPresetItemsindex PresetIndexes)
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        ABasePreviewPawn* PreviewPawn = Cast<ABasePreviewPawn>(OwnerPawn);
        if (PreviewPawn)
        {
            PresetIndexes.HeadIndex = EquippedItems.Contains(EItemSlot::Head) ? EquippedItems[EItemSlot::Head]->GetUniqueID() : -1;
            PresetIndexes.FaceIndex = EquippedItems.Contains(EItemSlot::Face) ? EquippedItems[EItemSlot::Face]->GetUniqueID() : -1;
            PresetIndexes.ShoulderIndex = EquippedItems.Contains(EItemSlot::Shoulder) ? EquippedItems[EItemSlot::Shoulder]->GetUniqueID() : -1;
        }
    }

    FName CharacterID = GetCharacterID();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC || !PC->IsLocalController())
    {
        UE_LOG(LogTemp, Warning, TEXT("SavePreset failed: No valid local PlayerController found."));
        return;
    }

    Server_SavePreset(PC, CharacterID, PresetIndexes);

    UE_LOG(LogTemp, Log, TEXT("Preset saved: CharacterID: %s, PresetIndex: %d, Head: %d, Face: %d, Shoulder: %d"),
        *CharacterID.ToString(), PresetIndexes.PresetIndex, PresetIndexes.HeadIndex, PresetIndexes.FaceIndex, PresetIndexes.ShoulderIndex);
}


void UCharacterCustomizationComponent::Server_SavePreset_Implementation(APlayerController* PC, FName CharacterID, FPresetItemsindex PresetIndexes)
{
    UE_LOG(LogTemp, Warning, TEXT("Server_SavePreset RPC Called"));

    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Server_SavePreset failed: Invalid PlayerController."));
        return;
    }

    AMainMenuPlayerState* PS = Cast<AMainMenuPlayerState>(PC->PlayerState);

    if (!PS)
    {
        UE_LOG(LogTemp, Warning, TEXT("Server_SavePreset failed: PlayerState is NULL or wrong type. Actual PlayerState: %s"),
            *GetNameSafe(PC->PlayerState));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Server_SavePreset succeeded: Found PlayerState %s"), *PS->GetName());
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