#include "Character/Lobby/LobbyPreviewPawn.h"
#include "Components/WidgetComponent.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/UI/Lobby/PreviewNameTagWidget.h"
#include "Framework/UI/Lobby/PreviewReadyStatusWidget.h"

ALobbyPreviewPawn::ALobbyPreviewPawn()
{
	PrimaryActorTick.bCanEverTick = true;

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

	if (!bNameSet)
	{
		if (UPreviewNameTagWidget* NameWidget = Cast<UPreviewNameTagWidget>(NameTagWidget->GetUserWidgetObject()))
		{
			if (ALobbyPlayerState* LPS = Cast<ALobbyPlayerState>(GetPlayerState()))
			{
				NameWidget->SetPlayerName(LPS->GetPlayerNickname());
				bNameSet = true;
			}
		}
	}

	if (UPreviewReadyStatusWidget* StatusWidget = Cast<UPreviewReadyStatusWidget>(ReadyStatusWidget->GetUserWidgetObject()))
	{
		if (ALobbyPlayerState* LPS = Cast<ALobbyPlayerState>(GetPlayerState()))
		{
			StatusWidget->SetReadyState(LPS->IsReady());
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
	}
}

void ALobbyPreviewPawn::SetReadyState(bool bIsReady)
{
	if (UPreviewReadyStatusWidget* StatusWidget = Cast<UPreviewReadyStatusWidget>(ReadyStatusWidget->GetUserWidgetObject()))
	{
		StatusWidget->SetReadyState(bIsReady);
	}
}
