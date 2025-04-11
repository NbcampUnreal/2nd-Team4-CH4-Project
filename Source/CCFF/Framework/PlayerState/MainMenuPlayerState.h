#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Items/DataTable/CustomItemData.h"
#include "MainMenuPlayerState.generated.h"

class UCustomizationItemAsset;

UCLASS()
class CCFF_API AMainMenuPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
	void SavePreset(FName CharacterID, int32 PresetIndex);



private:
};
