#include "Character/Base/BasePreviewPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Component/CharacterCustomizationComponent.h"
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

void ABasePreviewPawn::InitializePreview(FName CharacterID, AMainMenuPlayerState* InPlayerState)
{

}
