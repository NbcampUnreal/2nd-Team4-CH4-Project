#include "CharacterCustomizationComponent.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/Base/BaseCharacter.h"
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

void UCharacterCustomizationComponent::SaveCurrentPreset(int32 PresetIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Saving current preset..."));
}

void UCharacterCustomizationComponent::Server_SaveCurrentPreset_Implementation(FName CharacterID, int32 PresetIndex)
{
}

FName UCharacterCustomizationComponent::GetCharacterType() const
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn)
    {
        return TEXT("Unknown");
    }

    // ACharacter에서 CharacterType 가져오기
    ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OwnerCharacter);
    if (BaseCharacter)
    {
 
    }

    return OwnerPawn->GetFName();
}