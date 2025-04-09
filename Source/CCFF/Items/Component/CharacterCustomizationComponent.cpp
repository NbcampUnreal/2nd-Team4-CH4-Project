#include "Items/Component/CharacterCustomizationComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Net/UnrealNetwork.h"

UCharacterCustomizationComponent::UCharacterCustomizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    //SetIsReplicated(true);


}

void UCharacterCustomizationComponent::BeginPlay()
{
	Super::BeginPlay();
	
    Presets.SetNum(3);

	for (FCustomizationPreset& Preset : Presets)
	{
		Preset.Head = nullptr;
		Preset.Face = nullptr;
		Preset.Shoulder = nullptr;
	}
}
void UCharacterCustomizationComponent::Server_EquipItem_Implementation(UCustomizationItemAsset* ItemData)
{

}

//Equip the Item to the Character
void UCharacterCustomizationComponent::EquipItem(UCustomizationItemAsset* ItemData)
{
    if (!ItemData || !ItemData->Mesh) return;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    USkeletalMeshComponent* BaseMesh = OwnerCharacter->GetMesh();
    if (!BaseMesh) return;

    UnequipSlot(ItemData->EquipSlot);

    UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(OwnerCharacter);
    if (!NewMeshComp) return;

    NewMeshComp->SetStaticMesh(ItemData->Mesh);
    NewMeshComp->AttachToComponent(BaseMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ItemData->AttachSocketName);
    NewMeshComp->RegisterComponent();

    EquippedMeshComponents.Add(ItemData->EquipSlot, NewMeshComp);

    FEquippedItemEntry Entry(ItemData->EquipSlot, ItemData->ItemID);
    FEquippedItemEntry* Existing = EquippedItems.FindByPredicate([=](const FEquippedItemEntry& E) { return E.Slot == ItemData->EquipSlot; });
    if (Existing)
    {
        *Existing = Entry;
    }
    else
    {
        EquippedItems.Add(Entry);
    }
}


void UCharacterCustomizationComponent::UnequipSlot(EEquipSlot Slot)
{
    if (UStaticMeshComponent* Comp = EquippedMeshComponents.FindRef(Slot))
    {
        Comp->DestroyComponent();
        EquippedMeshComponents.Remove(Slot);
    }

    EquippedItems.RemoveAll([=](const FEquippedItemEntry& E) { return E.Slot == Slot; });
}

void UCharacterCustomizationComponent::UnequipAll()
{
    TArray<EEquipSlot> Slots;
    EquippedMeshComponents.GetKeys(Slots);

    for (EEquipSlot Slot : Slots)
    {
        UnequipSlot(Slot);
    }
}

//Apply the Preset to the Character
void UCharacterCustomizationComponent::ApplyPreset(int32 Index)
{
    if (!Presets.IsValidIndex(Index)) return;

    const FCustomizationPreset& Preset = Presets[Index];

    UnequipAll();
    if (Preset.Head) EquipItem(Preset.Head);
    if (Preset.Face) EquipItem(Preset.Face);
    if (Preset.Shoulder) EquipItem(Preset.Shoulder);
}

//Save Current Equipped Item Data to the Indexed Preset
void UCharacterCustomizationComponent::SaveCurrentToPreset(int32 Index)
{
    if (!Presets.IsValidIndex(Index)) return;

    FCustomizationPreset& Preset = Presets[Index];

    if (const FEquippedItemEntry* Entry = EquippedItems.FindByPredicate([](const FEquippedItemEntry& E) { return E.Slot == EEquipSlot::Head; }))
        Preset.Head = FindCustomizationAssetByID(Entry->ItemID);
    if (const FEquippedItemEntry* Entry = EquippedItems.FindByPredicate([](const FEquippedItemEntry& E) { return E.Slot == EEquipSlot::Face; }))
        Preset.Face = FindCustomizationAssetByID(Entry->ItemID);
    if (const FEquippedItemEntry* Entry = EquippedItems.FindByPredicate([](const FEquippedItemEntry& E) { return E.Slot == EEquipSlot::Shoulder; }))
        Preset.Shoulder = FindCustomizationAssetByID(Entry->ItemID);
}

void UCharacterCustomizationComponent::OnRep_EquippedItems()
{
    UnequipAll();

    for (const FEquippedItemEntry& Entry : EquippedItems)
    {
        if (UCustomizationItemAsset* Asset = FindCustomizationAssetByID(Entry.ItemID)) // 구현 필요
        {
            EquipItem(Asset);
        }
    }
}

UCustomizationItemAsset* UCharacterCustomizationComponent::FindCustomizationAssetByID(FName ItemID)
{
    // 예시: 어셋 레지스트리, DataTable, 혹은 Subsystem 등에서 찾는 로직
    return nullptr;
}

void UCharacterCustomizationComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCharacterCustomizationComponent, EquippedItems);
}