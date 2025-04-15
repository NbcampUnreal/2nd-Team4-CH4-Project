#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Items/Structure/CustomizationPreset.h"
#include "LobbyPlayerState.generated.h"


UCLASS()
class CCFF_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALobbyPlayerState();

	void SetReady(bool bNewReady);
	FORCEINLINE bool IsReady() const { return bIsReady; };
	FORCEINLINE FString GetPlayerNickname() const { return PlayerNickname; }
	void SetPlayerNickname(const FString& InNickname);

	UFUNCTION()
	void OnRep_ReadyState();

	UFUNCTION()
	void OnRep_PlayerNickname();

protected:
	UPROPERTY(Replicated = OnRep_ReadyState)
	bool bIsReady = false;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerNickname)
	FString PlayerNickname;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region CHARACTER_SELECT
public:
	void SetCharacterID(FName InID);

	UFUNCTION()
	FName GetCharacterID() const { return CharacterID; }

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CharacterID)
	FName CharacterID;

	UFUNCTION()
	void OnRep_CharacterID();

#pragma endregion

#pragma region CHARACTER_CUSTOMIZATION
public:

	void SetCharacterCustomizationPresets(const TArray<FCharacterCustomizationPreset>& Presets);
	FCustomizationPreset GetCustomizationPreset(FName RequestedCharacterID, int32 RequestedPresetIndex);

private:
	UPROPERTY(Replicated)
	TArray<FCharacterCustomizationPreset> ClientCharacterCustomizationPresets;
	FCharacterCustomizationPreset CurrentCharacterCustomizationPresets;
	FCustomizationPreset CurrentCustomizationPreset;

#pragma endregion

#pragma region HostPlayer
public:
	FORCEINLINE bool GetIsHost() const { return bIsHost; }
	FORCEINLINE void SetIsHost(bool bNewHost) { bIsHost = bNewHost; }

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsHost = false;

#pragma endregion

};
