#include "Character/Lobby/LobbyPreviewPawn.h"
#include "Components/WidgetComponent.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/UI/Lobby/PreviewNameTagWidget.h"
#include "Framework/UI/Lobby/PreviewReadyStatusWidget.h"


ALobbyPreviewPawn::ALobbyPreviewPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	NameTagWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTagWidget"));
	NameTagWidget->SetupAttachment(RootComponent);
	NameTagWidget->SetWidgetClass(UPreviewNameTagWidget::StaticClass());
	NameTagWidget->SetTickWhenOffscreen(true);

	ReadyStatusWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ReadyStatusWidget"));
	ReadyStatusWidget->SetupAttachment(RootComponent);
	ReadyStatusWidget->SetWidgetClass(UPreviewReadyStatusWidget::StaticClass());
	ReadyStatusWidget->SetTickWhenOffscreen(true);
}

void ALobbyPreviewPawn::BeginPlay()
{
	Super::BeginPlay();
	bNameSet = false;
}

void ALobbyPreviewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState) == false)
	{
		return;
	}


	FString CurrentName = LobbyPlayerState->GetPlayerNickname();
	if (CurrentName != LastNickname)
	{
		if (UPreviewNameTagWidget* NameWidget = Cast<UPreviewNameTagWidget>(NameTagWidget->GetUserWidgetObject()))
		{
			NameWidget->SetPlayerName(CurrentName);
			LastNickname = CurrentName;
		}
	}

	bool bCurrentReady = LobbyPlayerState->IsReady();
	if (bCurrentReady != bLastIsReady)
	{
		if (UPreviewReadyStatusWidget* StatusWidget = Cast<UPreviewReadyStatusWidget>(ReadyStatusWidget->GetUserWidgetObject()))
		{
			StatusWidget->SetReadyState(bCurrentReady);
			bLastIsReady = bCurrentReady;
		}
	}

	bool bCurrentHost = LobbyPlayerState->GetIsHost();
	if (bCurrentHost != bLastIsHost)
	{
		if (UPreviewNameTagWidget* NameWidget = Cast<UPreviewNameTagWidget>(NameTagWidget->GetUserWidgetObject()))
		{
			NameWidget->SetHostMarkVisible(bCurrentHost);
			bLastIsHost = bCurrentHost;
		}
	}
}

void ALobbyPreviewPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	bNameSet = false;
}

void ALobbyPreviewPawn::SetPlayerName(const FString& InName)
{
	if (UPreviewNameTagWidget* NameWidget = Cast<UPreviewNameTagWidget>(NameTagWidget->GetUserWidgetObject()))
	{
		NameWidget->SetPlayerName(InName);
		LastNickname = InName;
	}
}

void ALobbyPreviewPawn::SetReadyState(bool bIsReady)
{
	if (UPreviewReadyStatusWidget* StatusWidget = Cast<UPreviewReadyStatusWidget>(ReadyStatusWidget->GetUserWidgetObject()))
	{
		StatusWidget->SetReadyState(bIsReady);
		bLastIsReady = bIsReady;
	}
}

void ALobbyPreviewPawn::SetHostTagVisibility(bool bVisible)
{
	if (UPreviewNameTagWidget* NameWidget = Cast<UPreviewNameTagWidget>(NameTagWidget->GetUserWidgetObject()))
	{
		NameWidget->SetHostMarkVisible(bVisible);
		bLastIsHost = bVisible;
	}
}
