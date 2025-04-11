#include "Framework/PlayerState/MainMenuPlayerState.h"
#include "Items/DataAsset/CustomizationItemAsset.h"
#include "Net/UnrealNetwork.h"

void AMainMenuPlayerState::SavePreset(FName CharacterID, int32 PresetIndex)
{
	// Find the character customization preset for the given character ID
	FCharacterCustomizationPreset* FoundPreset = CharacterCustomizationPresets.FindByPredicate([CharacterID](const FCharacterCustomizationPreset& Preset)
		{
			return Preset.CharacterID == CharacterID;
		});
	if (FoundPreset)
	{
		// Save the current preset index
		FoundPreset->Presets[0].PresetIndex = PresetIndex;
	}
}

void AMainMenuPlayerState::OnRep_CharacterCustomizationPresets()
{
	// Handle the replication of character customization presets here
}

void AMainMenuPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainMenuPlayerState, CharacterCustomizationPresets);
}