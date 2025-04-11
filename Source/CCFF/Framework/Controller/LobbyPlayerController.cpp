#include "Framework/Controller/LobbyPlayerController.h"
#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Framework/GameMode/LobbyGameMode.h"
#include "Framework/GameState/LobbyGameState.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/UI/LobbyWidget.h"
#include "Framework/UI/CountdownWidget.h"
#include "Kismet/GameplayStatics.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	if (IsLocalController())
	{
		HandleLocalSetup();
		
		UCCFFGameInstance* GameInstance = GetGameInstance<UCCFFGameInstance>();
		if (GameInstance)
		{
			ServerSetNickname(GameInstance->GetNickname());
		}
	}
}

void ALobbyPlayerController::HandleLocalSetup()
{
	ShowLobbyUI();
}

void ALobbyPlayerController::SetLobbyCameraView()
{
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LobbyCam"), FoundCameras);

	if (FoundCameras.Num() > 0)
	{
		SetViewTargetWithBlend(FoundCameras[0], 0.5f);
	}
}

void ALobbyPlayerController::SetAllowSoloStart(int32 Allow)
{
	ServerSetAllowSoloStart(Allow);
}

void ALobbyPlayerController::ServerSetAllowSoloStart_Implementation(int32 Allow)
{
	if (ALobbyGameMode* GM = GetWorld()->GetAuthGameMode<ALobbyGameMode>())
	{
		GM->bAllowSoloStart = (Allow > 0);
		UE_LOG(LogTemp, Warning, TEXT("[Server] SetAllowSoloStart = %s"), Allow > 0 ? TEXT("true") : TEXT("false"));

		if (ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>())
		{
			GS->UpdateAllowStartGame();
		}
	}
}

void ALobbyPlayerController::ShowLobbyUI()
{
	if (LobbyWidgetClass && !LobbyWidgetInstance)
	{
		LobbyWidgetInstance = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		if (LobbyWidgetInstance)
		{
			LobbyWidgetInstance->AddToViewport();
		}
	}
}

void ALobbyPlayerController::ServerSetNickname_Implementation(const FString& InNickname)
{
	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState) == true)
	{
		LobbyPlayerState->SetPlayerNickname(InNickname);
	}
}

void ALobbyPlayerController::ServerToggleReady_Implementation()
{
	ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (IsValid(LobbyPlayerState) == true)
	{
		bool bNewState = !LobbyPlayerState->IsReady();
		LobbyPlayerState->SetReady(bNewState);

		ALobbyGameMode* LobbyGameMode = GetWorld()->GetAuthGameMode<ALobbyGameMode>();
		if (IsValid(LobbyGameMode) == true)
		{
			LobbyGameMode->NotifyPlayerReadyStatusChanged();
		}
	}
}

void ALobbyPlayerController::UpdateCountdownWidget_Implementation(int32 NewTime)
{
	if (!CountdownWidgetInstance && CountdownWidgetClass)
	{
		CountdownWidgetInstance = CreateWidget<UCountdownWidget>(this, CountdownWidgetClass);
		if (CountdownWidgetInstance)
		{
			CountdownWidgetInstance->AddToViewport();
		}
	}

	if (CountdownWidgetInstance)
	{
		if (NewTime >= 0)
		{
			CountdownWidgetInstance->SetCountdownText(FString::FromInt(NewTime));
		}
		else
		{
			CountdownWidgetInstance->RemoveFromParent();
			CountdownWidgetInstance = nullptr;
		}
	}
}

void ALobbyPlayerController::ClientTeardownCountdown_Implementation()
{
	if (CountdownWidgetInstance)
	{
		CountdownWidgetInstance->RemoveFromParent();
		CountdownWidgetInstance = nullptr;
	}
}
