#include "CharacterCustomizationComponent.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
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

    TArray<FName> SocketNames = MeshComp->GetAllSocketNames();

    for (FName SocketName : SocketNames)
    {
        UE_LOG(LogTemp, Log, TEXT("Available Socket: %s"), *SocketName.ToString());

    }
    if (!MeshComp)
    {
        UE_LOG(LogTemp, Error, TEXT("SkeletalMeshComponent not found on Owner."));
        return;
    }

    if (MeshComp->SkeletalMesh)
    {
        UE_LOG(LogTemp, Log, TEXT("Character's SkeletalMesh: %s"), *MeshComp->SkeletalMesh->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Character's SkeletalMesh is NULL!"));
    }

    if (!MeshComp->DoesSocketExist(ItemData.SocketName))
    {
        UE_LOG(LogTemp, Error, TEXT("Socket %s does not exist on skeletal mesh: %s"),
            *ItemData.SocketName.ToString(),
            MeshComp->SkeletalMesh ? *MeshComp->SkeletalMesh->GetName() : TEXT("NULL"));
    }

    UStaticMeshComponent* NewItemComponent = NewObject<UStaticMeshComponent>(OwnerPawn);
    if (!NewItemComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create StaticMeshComponent for item."));
        return;
    }
        UStaticMesh* ItemMesh = ItemData.ItemMesh.LoadSynchronous();
    NewItemComponent->SetStaticMesh(ItemMesh);

    NewItemComponent->RegisterComponent();

    if (!ItemMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load StaticMesh for item ID: %d"), ItemData.ItemID);
        return;
    }

    if (MeshComp->SkeletalMesh)
    {
        UE_LOG(LogTemp, Log, TEXT("SkeletalMesh Name: %s"), *MeshComp->SkeletalMesh->GetName());
    }

    NewItemComponent->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform, ItemData.SocketName);

    UE_LOG(LogTemp, Log, TEXT("Attaching item '%s' to socket: %s on SkeletalMesh: %s"),
        *ItemMesh->GetName(),
        *ItemData.SocketName.ToString(),
        *MeshComp->SkeletalMesh->GetName());

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
