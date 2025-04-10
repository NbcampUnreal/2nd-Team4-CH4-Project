#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Items/DataAsset/CustomizationItemAsset.h"
#include "BasePreviewPawn.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class UCustomizationItemAsset;
class USpringArmComponent;

/**
 * Base Pawn for Previewing Customization Items
 */
UCLASS()
class CCFF_API ABasePreviewPawn : public APawn
{
    GENERATED_BODY()

public:
    ABasePreviewPawn();

    /** Change Skeletal Mesh */
    UFUNCTION(BlueprintCallable, Category = "Preview")
    void SetPreviewSkeletalMesh(USkeletalMesh* NewMesh);

    UFUNCTION(BlueprintCallable, Category = "Preview")
    void EquipPreviewItem(UCustomizationItemAsset* ItemData);

    UFUNCTION(BlueprintCallable, Category = "Preview")
    void UnequipSlot(EEquipSlot Slot);

    UFUNCTION(BlueprintCallable, Category = "Preview")
    void UnequipAll();

    /** Returns Render Target for UI */
    UFUNCTION(BlueprintCallable, Category = "Preview")
    UTextureRenderTarget2D* GetRenderTarget() const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

    /** Scene Capture */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneCaptureComponent2D* SceneCapture;

    /** Render Target for Scene Capture */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTextureRenderTarget2D* RenderTarget;

    /** Equipped MeshComponents */
    UPROPERTY(Transient)
    TMap<EEquipSlot, UStaticMeshComponent*> EquippedMeshComponents;

    /** Equipped Item Data */
    UPROPERTY(Transient)
    TMap<EEquipSlot, UCustomizationItemAsset*> EquippedItems;
};