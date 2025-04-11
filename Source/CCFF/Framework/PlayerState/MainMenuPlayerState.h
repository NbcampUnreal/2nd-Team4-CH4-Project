#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Items/Structure/CustomizationPreset.h"
#include "MainMenuPlayerState.generated.h"

class UCustomizationItemAsset;

UCLASS()
class CCFF_API AMainMenuPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
	void SavePreset(FName CharacterID, int32 PresetIndex);

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_CharacterCustomizationPresets)
	TArray <FCharacterCustomizationPreset> CharacterCustomizationPresets;

	UFUNCTION()
	void OnRep_CharacterCustomizationPresets();


};
