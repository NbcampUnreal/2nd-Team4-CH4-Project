#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Framework/Data/ArenaSubModeType.h"
#include "LobbyWidget.generated.h"

UCLASS()
class CCFF_API ULobbyWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void UpdateArenaSubModeUI(EArenaSubMode NewMode);

protected:
    UFUNCTION()
    void OnReadyClicked();

    UFUNCTION()
    void OnBackClicked();

    UFUNCTION()
    void OnArenaSubModeSelected(EArenaSubMode Selected);

    UPROPERTY(meta = (BindWidget))
    class UButton* ReadyButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* BackButton;

    UPROPERTY(meta = (BindWidget))
    class UArenaSubModeSelectorWidget* ArenaSubModeSelectorWidget;

};
