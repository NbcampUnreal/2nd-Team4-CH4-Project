#include "Items/Component/CharacterCustomizationComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Structure/CustomizationPresetTypes.h"
#include "Items/DataAsset/CustomizationItemAsset.h"

UCharacterCustomizationComponent::UCharacterCustomizationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterCustomizationComponent::BeginPlay()
{
    Super::BeginPlay();
}

// clear all customization items
void UCharacterCustomizationComponent::ClearCustomization()
{
    for (auto& Elem : AttachedMeshes)
    {
        if (Elem.Value)
        {
            Elem.Value->DestroyComponent();
        }
    }
    AttachedMeshes.Empty();
}

void UCharacterCustomizationComponent::ApplyCustomization(const FCustomizationPreset& Preset)
{
    ClearCustomization();

    for (const auto& Pair : Preset.EquippedItems)
    {
        const FEquippedItemData& ItemData = Pair.Value;

        const FStringAssetReference AssetRef(TEXT("/Game/Customization/Items/") + ItemData.ItemID.ToString() + TEXT(".") + ItemData.ItemID.ToString());
        UCustomizationItemAsset* ItemAsset = Cast<UCustomizationItemAsset>(AssetRef.TryLoad());
        if (!ItemAsset || !ItemAsset->Mesh) continue;

        AttachItemToSlot(ItemAsset->EquipSlot, ItemAsset->Mesh, ItemAsset->AttachSocketName);
    }
}

void UCharacterCustomizationComponent::AttachItemToSlot(EEquipSlot Slot, UStaticMesh* Mesh, FName SocketName)
{
    if (!Mesh || !GetOwner()) return;

    USkeletalMeshComponent* Skeletal = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
    if (!Skeletal || !Skeletal->DoesSocketExist(SocketName)) return;

    UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(GetOwner());
    MeshComp->RegisterComponent();
    MeshComp->SetStaticMesh(Mesh);
    MeshComp->AttachToComponent(Skeletal, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
    MeshComp->SetRelativeLocation(FVector::ZeroVector);

    AttachedMeshes.Add(Slot, MeshComp);
}