#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Character/Lobby/LobbyPreviewPawn.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

ALobbyPlayerState::ALobbyPlayerState()
{
	bReplicates = true;
	bIsReady = false;
	PlayerNickname = TEXT("Player");
}

void ALobbyPlayerState::SetReady(bool bNewReady)
{
	if (HasAuthority())
	{
		bIsReady = bNewReady;
		OnRep_ReadyState();
	}
}

void ALobbyPlayerState::SetPlayerNickname(const FString& InNickname)
{
	if (HasAuthority())
	{
		PlayerNickname = InNickname;
		OnRep_PlayerNickname();
	}
}

void ALobbyPlayerState::SetCharacterID(FName InID)
{
	CharacterID = InID;
	OnRep_CharacterID();
}

void ALobbyPlayerState::OnRep_ReadyState()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (IsValid(PlayerController) == true)
	{
		ALobbyPreviewPawn* PreviewPawn = Cast<ALobbyPreviewPawn>(PlayerController->GetPawn());
		if (IsValid(PreviewPawn) == true)
		{
			PreviewPawn->SetReadyState(bIsReady);
		}
	}
}

void ALobbyPlayerState::OnRep_PlayerNickname()
{
	UWorld* World = GetWorld();
	if (!World) return;

	for (TActorIterator<ALobbyPreviewPawn> It(World); It; ++It)
	{
		ALobbyPreviewPawn* PreviewPawn = *It;
		if (!IsValid(PreviewPawn)) continue;

		if (PreviewPawn->GetPlayerState() == this)
		{
			PreviewPawn->SetPlayerName(PlayerNickname);
		}
	}
}

void ALobbyPlayerState::OnRep_CharacterID()
{
	APawn* Pawn = GetPawn();
	if (IsValid(Pawn))
	{
		ABasePreviewPawn* PreviewPawn = Cast<ABasePreviewPawn>(Pawn);
		if (IsValid(PreviewPawn))
		{
			PreviewPawn->InitializePreview(CharacterID);
		}
	}
}

FCustomizationPreset ALobbyPlayerState::GetCustomizationPreset(FName RequestedCharacterID, int32 RequestedPresetIndex)
{
	for (const FCharacterCustomizationPreset& CharPreset : ClientCharacterCustomizationPresets)
	{
		if (CharPreset.CharacterID == RequestedCharacterID)
		{
			for (const FCustomizationPreset& Preset : CharPreset.Presets)
			{
				if (Preset.PresetIndex == RequestedPresetIndex)
				{
					return Preset;
				}
			}
		}
	}

	return FCustomizationPreset();
}

void ALobbyPlayerState::SetCharacterCustomizationPresets(const TArray<FCharacterCustomizationPreset>& Presets)
{
	ClientCharacterCustomizationPresets = Presets;
	for (const FCharacterCustomizationPreset& CharPreset : ClientCharacterCustomizationPresets)
	{
		UE_LOG(LogTemp, Log, TEXT("¢º Character ID: %s"), *CharPreset.CharacterID.ToString());
		for (const FCustomizationPreset& Preset : CharPreset.Presets)
		{
			UE_LOG(LogTemp, Log, TEXT("  - Preset Index: %d"), Preset.PresetIndex);
			for (const FEquippedItemData& Item : Preset.EquippedItems)
			{
				UE_LOG(LogTemp, Log, TEXT("    - Slot: %s, ItemID: %s"), *UEnum::GetValueAsString(Item.EquipSlot), *Item.ItemID.ToString());
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("=================================="));
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, PlayerNickname);
	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
	DOREPLIFETIME(ALobbyPlayerState, CharacterID);
	DOREPLIFETIME(ALobbyPlayerState, ClientCharacterCustomizationPresets);

}