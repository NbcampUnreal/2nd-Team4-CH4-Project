#include "Framework/UI/LobbyWidget.h"
#include "Framework/Controller/LobbyPlayerController.h"
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
