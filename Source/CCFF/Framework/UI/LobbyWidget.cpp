#include "Framework/UI/LobbyWidget.h"
#include "Framework/Controller/LobbyPlayerController.h"
#include "Framework/GameState/LobbyGameState.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void ULobbyPlayerSlotWidget::SetPlayerInfo(const FString& Nickname, bool bReady)
{
    if (NicknameText)
    {
        NicknameText->SetText(FText::FromString(Nickname));
    }

    if (ReadyText)
    {
        ReadyText->SetText(bReady
            ? FText::FromString(TEXT("Ready"))
            : FText::FromString(TEXT("Not Ready")));
    }
}

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    RefreshPlayerSlots();

    if (ReadyButton)
    {
        ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnReadyButtonClicked);
    }
}

void ULobbyWidget::RefreshPlayerSlots()
{
    if (!PlayerSlotBox || !PlayerSlotWidgetClass) return;

    UnbindAllPlayerStateEvents();
    PlayerSlotBox->ClearChildren();
    PlayerSlotMap.Empty();

    ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
    if (IsValid(LobbyGameState) == false)
    {
        return;
    }

    for (APlayerState* PlayerState : LobbyGameState->PlayerArray)
    {
        ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
        if (IsValid(LobbyPlayerState))
        {
            AddPlayerSlot(LobbyPlayerState);
            BindToPlayerStateEvents(LobbyPlayerState);
        }
    }
}

void ULobbyWidget::AddPlayerSlot(ALobbyPlayerState* LobbyPlayerState)
{
    ULobbyPlayerSlotWidget* SlotWidget = CreateWidget<ULobbyPlayerSlotWidget>(this, PlayerSlotWidgetClass);
    if (IsValid(SlotWidget))
    {
        SlotWidget->SetPlayerInfo(LobbyPlayerState->GetPlayerNickname(), LobbyPlayerState->IsReady());
        PlayerSlotBox->AddChild(SlotWidget);
        PlayerSlotMap.Add(LobbyPlayerState, SlotWidget);
    }
}

void ULobbyWidget::BindToPlayerStateEvents(ALobbyPlayerState* LobbyPlayerState)
{
    if (IsValid(LobbyPlayerState) == false)
    {
        return;
    }

    LobbyPlayerState->OnReadyStateChanged.AddDynamic(this, &ULobbyWidget::OnAnyPlayerReadyStateChanged);
}

void ULobbyWidget::UnbindAllPlayerStateEvents()
{
    for (auto& Pair : PlayerSlotMap)
    {
        if (IsValid(Pair.Key))
        {
            Pair.Key->OnReadyStateChanged.RemoveAll(this);
        }
    }

    PlayerSlotMap.Empty();
}

void ULobbyWidget::OnAnyPlayerReadyStateChanged(bool bIsReady)
{
    RefreshPlayerSlots();
}

void ULobbyWidget::OnReadyButtonClicked()
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
