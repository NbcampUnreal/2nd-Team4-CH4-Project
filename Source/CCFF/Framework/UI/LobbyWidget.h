#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "LobbyWidget.generated.h"

UCLASS()
class CCFF_API ULobbyPlayerSlotWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPlayerInfo(const FString& Nickname, bool bReady);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* NicknameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ReadyText;
};

UCLASS()
class CCFF_API ULobbyWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;
    void RefreshPlayerSlots();

protected:
    UFUNCTION()
    void OnReadyButtonClicked();

    UFUNCTION()
    void OnAnyPlayerReadyStateChanged(bool bIsReady);

    void AddPlayerSlot(ALobbyPlayerState* LobbyPlayerState);
    void BindToPlayerStateEvents(ALobbyPlayerState* LobbyPlayerState);
    void UnbindAllPlayerStateEvents();

    UPROPERTY(meta = (BindWidget))
    class UButton* ReadyButton;

    UPROPERTY(EditDefaultsOnly, Category = "Lobby")
    TSubclassOf<class ULobbyPlayerSlotWidget> PlayerSlotWidgetClass;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* PlayerSlotBox;

    UPROPERTY()
    TMap<TObjectPtr<class ALobbyPlayerState>, TObjectPtr<class ULobbyPlayerSlotWidget>> PlayerSlotMap;

};
