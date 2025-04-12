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

	void SetPlayerNickname(const FString& InNickname);
	FORCEINLINE FString GetPlayerNickname() const { return PlayerNickname; }
	FORCEINLINE void SetCharacterCustomizationPresets(const TArray<FCharacterCustomizationPreset>& Presets) { ClientCharacterCustomizationPresets = Presets; };

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

private:
	TArray <FCharacterCustomizationPreset> ClientCharacterCustomizationPresets;
};
