#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePreviewPawn.generated.h"

class USkeletalMeshComponent;
class UCharacterCustomizationComponent;

UCLASS()
class CCFF_API ABasePreviewPawn : public APawn
{
    GENERATED_BODY()

public:
    ABasePreviewPawn();


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USkeletalMeshComponent* PreviewMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UCharacterCustomizationComponent* CustomizationComponent;

    void InitializePreview(FName CharacterID, class AMainMenuPlayerState* PlayerState);
    FORCEINLINE FName GetCharacterID() const { return CurretCharacterID; };

protected:
	virtual void BeginPlay() override;

private:
	FName CurretCharacterID = FName("Cactus"); // Default character ID
};