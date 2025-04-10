#include "Character/Lobby/LobbyPreviewPawn.h"
#include "Components/WidgetComponent.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/UI/Lobby/PreviewNameTagWidget.h"
#include "Framework/UI/Lobby/PreviewReadyStatusWidget.h"

ALobbyPreviewPawn::ALobbyPreviewPawn()
{
	NameTagWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTagWidget"));
	NameTagWidget->SetupAttachment(RootComponent);

	ReadyStatusWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ReadyStatusWidget"));
	ReadyStatusWidget->SetupAttachment(RootComponent);
}

void ALobbyPreviewPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(GetPlayerState());
	if (LobbyPlayerState)
	{
		SetPlayerName(LobbyPlayerState->GetPlayerNickname());
		SetReadyState(LobbyPlayerState->IsReady());
	}
}

void ALobbyPreviewPawn::SetPlayerName(const FString& InName)
{
	UPreviewNameTagWidget* Widget = Cast<UPreviewNameTagWidget>(NameTagWidget->GetUserWidgetObject());
	if (Widget)
	{
		Widget->SetPlayerName(InName);
	}
}

void ALobbyPreviewPawn::SetReadyState(bool bReady)
{
	UPreviewReadyStatusWidget* Widget = Cast<UPreviewReadyStatusWidget>(NameTagWidget->GetUserWidgetObject());
	if (Widget)
	{
		Widget->SetReadyState(bReady);
	}
}
