#include "Character/Base/BasePreviewPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Engine/TextureRenderTarget2D.h"


ABasePreviewPawn::ABasePreviewPawn()
{
	PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMesh->SetupAttachment(RootComponent);


}

void ABasePreviewPawn::BeginPlay()
{
	Super::BeginPlay();
    CustomizationComponent = CreateDefaultSubobject<UCharacterCustomizationComponent>(TEXT("CharacterCustomizationComponent"));
}

void ABasePreviewPawn::SetPreviewSkeletalMesh(USkeletalMesh* NewMesh)
{
	if (NewMesh)
	{
        SkeletalMesh->SetSkeletalMesh(NewMesh);
	}
}

void ABasePreviewPawn::EquipPreviewItem(UCustomizationItemAsset* ItemData)
{
    if (!ItemData || !ItemData->Mesh) return;

    // ���� ���� ����
    UnequipSlot(ItemData->EquipSlot);

    // �� StaticMeshComponent ���� �� ����
    UStaticMeshComponent* NewComp = NewObject<UStaticMeshComponent>(this);
    if (!NewComp) return;

    NewComp->SetStaticMesh(ItemData->Mesh);
    NewComp->RegisterComponent();
    NewComp->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ItemData->AttachSocketName);

    EquippedMeshComponents.Add(ItemData->EquipSlot, NewComp);
    EquippedItems.Add(ItemData->EquipSlot, ItemData);
}

void ABasePreviewPawn::UnequipSlot(EEquipSlot Slot)
{
    if (UStaticMeshComponent* Comp = EquippedMeshComponents.FindRef(Slot))
    {
        Comp->DestroyComponent();
        EquippedMeshComponents.Remove(Slot);
    }

    EquippedItems.Remove(Slot);
}

void ABasePreviewPawn::UnequipAll()
{
    for (auto& Pair : EquippedMeshComponents)
    {
        if (Pair.Value)
        {
            Pair.Value->DestroyComponent();
        }
    }

    EquippedMeshComponents.Empty();
    EquippedItems.Empty();
}
