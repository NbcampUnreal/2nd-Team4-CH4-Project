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

	static const FString DataTablePath = TEXT("/Game/CCFF/DataTables/CustomItemData.CustomItemData");
	CustomItemDataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
	if (!CustomItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load ItemSpawnTable."));
	}
}

void UCharacterCustomizationComponent::EquipItemByID(int32 ItemID)
{
    if (!CustomItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Item DataTable is not set."));
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

// 특정 아이템 장착
void UCharacterCustomizationComponent::EquipItem(FCustomItemData ItemData)
{
    if (ItemData.Slot == EItemSlot::None)
    {
        UE_LOG(LogTemp, Warning, TEXT("Item slot is None. Cannot equip."));
        return;
    }

    // 기존 장착 아이템 해제
    UnequipItem(ItemData.Slot);

    // 소유 캐릭터의 Skeletal Mesh 가져오기
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

    if (!MeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent not found on Owner."));
        return;
    }

    // 새로운 아이템 생성 및 장착
    UStaticMeshComponent* NewItemComponent = NewObject<UStaticMeshComponent>(OwnerPawn);
    if (!NewItemComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create StaticMeshComponent for item."));
        return;
    }

    NewItemComponent->RegisterComponent();
    NewItemComponent->SetStaticMesh(ItemData.ItemMesh.LoadSynchronous());
    NewItemComponent->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemData.SocketName);

    EquippedItems.Add(ItemData.Slot, NewItemComponent);
}

// 특정 슬롯의 아이템 해제
void UCharacterCustomizationComponent::UnequipItem(EItemSlot Slot)
{
    if (UStaticMeshComponent** FoundItem = EquippedItems.Find(Slot))
    {
        (*FoundItem)->DestroyComponent();
        EquippedItems.Remove(Slot);
    }
}
