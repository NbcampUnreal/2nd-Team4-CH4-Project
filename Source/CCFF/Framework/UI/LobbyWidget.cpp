#include "Framework/UI/LobbyWidget.h"
#include "Framework/Controller/LobbyPlayerController.h"
#include "Framework/UI/Lobby/ArenaSubModeSelectorWidget.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Components/Button.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ReadyButton && !ReadyButton->OnClicked.IsAlreadyBound(this, &ULobbyWidget::OnReadyClicked))
    {
        ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnReadyClicked);
    }

    if (BackButton && !BackButton->OnClicked.IsAlreadyBound(this, &ULobbyWidget::OnBackClicked))
    {
        BackButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnBackClicked);
    }

    if (ArenaSubModeSelectorWidget)
    {
        ArenaSubModeSelectorWidget->OnSubModeSelected.AddDynamic(this, &ULobbyWidget::OnArenaSubModeSelected);
    }
}

void ULobbyWidget::UpdateArenaSubModeUI(EArenaSubMode NewMode)
{
    if (ArenaSubModeSelectorWidget)
    {
        ArenaSubModeSelectorWidget->SetCurrentSelectedMode(NewMode);
    }
}

void ULobbyWidget::OnReadyClicked()
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
        if (LobbyPlayerController)
        {
            LobbyPlayerController->ServerToggleReady();
            UCCFFGameInstance* CCFFGameInstance = GetGameInstance<UCCFFGameInstance>();
            if (CCFFGameInstance)
            {
				CCFFGameInstance->SetLobbyPresetIndex(LobbyPlayerController->GetCurrentPresetIndex());
				UE_LOG(LogTemp, Warning, TEXT("[Client] SetLobbyPresetIndex = %d"), LobbyPlayerController->GetCurrentPresetIndex());
            }
        }
    }

    
}

void ULobbyWidget::OnBackClicked()
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
        if (LobbyPlayerController)
        {
            LobbyPlayerController->ClientTravel(TEXT("/Game/CCFF/Maps/MainMenuMap"), ETravelType::TRAVEL_Absolute);
        }
    }
}

void ULobbyWidget::OnArenaSubModeSelected(EArenaSubMode Selected)
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
        if (LobbyPlayerController)
        {
            LobbyPlayerController->ServerRequestChangeArenaSubMode(Selected);
        }
    }
}
