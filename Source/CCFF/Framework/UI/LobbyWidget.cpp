#include "Framework/UI/LobbyWidget.h"
#include "Framework/Controller/LobbyPlayerController.h"
#include "Components/Button.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ReadyButton)
    {
        ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnReadyClicked);
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
