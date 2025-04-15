#include "Character/Base/BasePreviewPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/PlayerState/MainMenuPlayerState.h"

ABasePreviewPawn::ABasePreviewPawn()
{
	PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	PreviewMesh->SetupAttachment(RootComponent);

    CustomizationComponent = CreateDefaultSubobject<UCharacterCustomizationComponent>(TEXT("CustomizationComponent"));
}

void ABasePreviewPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePreviewPawn::InitializePreview(FName CharacterID)
{
	CurrentCharacterID = CharacterID;

	if (CustomizationComponent)
	{
		if (USkeletalMesh* Mesh = CustomizationComponent->GetBaseMeshByCharacterID(CharacterID))
		{
			PreviewMesh->SetSkeletalMesh(Mesh);
			if (UClass* AnimBPClass = CustomizationComponent->GetAnimBlueprintByCharacterID(CharacterID))
			{
				PreviewMesh->SetAnimInstanceClass(AnimBPClass);
			}
			OnCharacterChanged.ExecuteIfBound();

			if (!IsLocallyControlled())
			{
				CustomizationComponent->UnequipAllItems();
			}
		}
	}
}
