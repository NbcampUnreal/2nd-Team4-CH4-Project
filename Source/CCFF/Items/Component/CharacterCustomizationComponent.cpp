#include "Items/Component/CharacterCustomizationComponent.h"
#include "Character/Base/BaseCharacter.h"
#include "Character/Base/BasePreviewPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "Framework/PlayerState/MainMenuPlayerState.h"
#include "GameFramework/Character.h"
#include "Items/DataTable/CustomItemData.h"
#include "Items/Manager/CustomizationManager.h"
#include "Items/Structure/CustomizationPreset.h"

UCharacterCustomizationComponent::UCharacterCustomizationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterCustomizationComponent::BeginPlay()
{
	Super::BeginPlay();

    InitializeCustomizationSystem();
}

void UCharacterCustomizationComponent::InitializeCustomizationSystem()
{
    CustomizationManager = Cast<UGameInstance>(GetOwner()->GetGameInstance())->GetSubsystem<UCustomizationManager>();
    HeadCustomItemDataTable = CustomizationManager->GetHeadCustomItemDataTable();
    FaceCustomItemDataTable = CustomizationManager->GetFaceCustomItemDataTable();
    ShoulderCustomItemDataTable = CustomizationManager->GetShoulderCustomItemDataTable();
    if (!HeadCustomItemDataTable || !FaceCustomItemDataTable || !ShoulderCustomItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load DataTables."));
        return;
    }
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
	for (const FEquippedItemData& Item : Preset.EquippedItems)
	{
		FName ItemID = Item.ItemID;
		int32 ItemIDInt = FCString::Atoi(*ItemID.ToString());
		EquipItemByID(ItemIDInt, Item.EquipSlot);
	}
}