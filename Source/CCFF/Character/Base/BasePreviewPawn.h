#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePreviewPawn.generated.h"

class USkeletalMeshComponent;
class UCharacterCustomizationComponent;

DECLARE_DELEGATE(FOnCharacterChanged);

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

	FOnCharacterChanged OnCharacterChanged;

    void InitializePreview(FName CharacterID);
    void InitializePreview(FName CharacterID, UObject* PresetSource);
    FORCEINLINE FName GetCharacterID() const { return CurrentCharacterID; };

protected:
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
    FName CurrentCharacterID = "Cactus"; // Default character ID

};