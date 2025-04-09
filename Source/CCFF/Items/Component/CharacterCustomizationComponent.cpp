#include "Items/Component/CharacterCustomizationComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

UCharacterCustomizationComponent::UCharacterCustomizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterCustomizationComponent::Server_EquipItem_Implementation(UCustomizationItemAsset* ItemData)
{

}

void UCharacterCustomizationComponent::EquipItem(UCustomizationItemAsset* ItemData)
{
    if (!ItemData || !ItemData->Mesh) return;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    USkeletalMeshComponent* BaseMesh = OwnerCharacter->GetMesh();
    if (!BaseMesh) return;

    // Remove Existing Item
    UnequipSlot(ItemData->EquipSlot);

    // Create New StaticMeshComponent and Attach
    UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(OwnerCharacter);
    if (!NewMeshComp) return;

    NewMeshComp->SetStaticMesh(ItemData->Mesh);
    NewMeshComp->AttachToComponent(BaseMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ItemData->AttachSocketName);
    NewMeshComp->RegisterComponent();

    EquippedMeshComponents.Add(ItemData->EquipSlot, NewMeshComp);
    EquippedItems.Add(ItemData->EquipSlot, ItemData);
}

void UCharacterCustomizationComponent::UnequipSlot(EEquipSlot Slot)
{
    if (UStaticMeshComponent* Comp = EquippedMeshComponents.FindRef(Slot))
    {
        Comp->DestroyComponent();
        EquippedMeshComponents.Remove(Slot);
    }

    EquippedItems.Remove(Slot);
}

