#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Framework/Data/ArenaSubModeType.h"
#include "LobbyPlayerController.generated.h"

UCLASS()
class CCFF_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Server, Reliable)
	void ServerSetNickname(const FString& InNickname);

	UFUNCTION(Server, Reliable)
	void ServerToggleReady();

	void SetLobbyCameraView();

	UFUNCTION(Client, Reliable)
	void UpdateCountdownWidget(int32 NewTime);

	UFUNCTION(Client, Reliable)
	void ClientTeardownCountdown();

#pragma region CHARACTER_SELECT
	UFUNCTION()
	void HandleCharacterSelectedFromUI(FName CharacterID);

	UFUNCTION()
	void HandleHorizontalInput(const FInputActionValue& Value);

	UFUNCTION()
	void HandleVerticalInput(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerSetCharacterID(FName CharacterID);

	void SetupEnhancedInput();
	void CacheCharacterIDList();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* CharacterSelectInputContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_NavigateHorizontal;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_NavigateVertical;

	int32 CurrentCharacterIndex = 0;
	TArray<FName> CharacterIDList;

#pragma endregion


#pragma region FORBID_SOLO_PLAYING
	UFUNCTION(Exec)
	void SetAllowSoloStart(int32 Allow);

	UFUNCTION(Server, Reliable)
	void ServerSetAllowSoloStart(int32 Allow);

#pragma endregion

public:
	FORCEINLINE TObjectPtr<class ULobbyWidget> GetLobbyWidget() const { return LobbyWidgetInstance; }

protected:
	void ShowLobbyUI();
	
	void HandleLocalSetup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULobbyWidget> LobbyWidgetInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCountdownWidget> CountdownWidgetClass;

	UPROPERTY()
	UCountdownWidget* CountdownWidgetInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCharacterSelectWidget> CharacterSelectWidgetClass;

	UPROPERTY()
	UCharacterSelectWidget* CharacterSelectWidgetInstance;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region CHARACTER_CUSTOMIZATION
public:
	FORCEINLINE int32 GetCurrentPresetIndex() const { return CurrentPresetIndex; }

protected:
	UFUNCTION(Server, Reliable)
	void Server_SetPresetsToPlayerState(const TArray<FCharacterCustomizationPreset>& ClientPresets);
	void Server_SetPresetsToPlayerState_Implementation(const TArray<FCharacterCustomizationPreset>& ClientPresets);

private:
	UFUNCTION(Server, Reliable)
	void Server_SwitchPresetIndex(int32 IndexDirection);
	void Server_SwitchPresetIndex_Implementation(int32 IndexDirection);

	UPROPERTY(Replicated)
	int32 CurrentPresetIndex = -1;
	
	virtual void OnRep_PlayerState() override;
	void SetCustomizationPresets();
	void RequestEquipPreset(FCustomizationPreset Preset);

#pragma endregion

#pragma region ArenaSubMode
public:
	UFUNCTION(Server, Reliable)
	void ServerRequestChangeArenaSubMode(EArenaSubMode NewMode);

#pragma endregion

#pragma region LOADING_WIDGET
public:
	UFUNCTION(Client, Reliable)
	void ShowLoadingWidget();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	class UUserWidget* LoadingWidget;

#pragma endregion

};
