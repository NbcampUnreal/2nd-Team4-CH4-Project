#include "Framework/PlayerState/LobbyPlayerState.h"
#include "Character/Lobby/LobbyPreviewPawn.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

ALobbyPlayerState::ALobbyPlayerState()
{
	bReplicates = true;
	bIsReady = false;
	PlayerNickname = TEXT("Player");
}

void ALobbyPlayerState::SetReady(bool bNewReady)
{
	if (HasAuthority())
	{
		bIsReady = bNewReady;
		OnRep_ReadyState();
	}
}

void ALobbyPlayerState::SetPlayerNickname(const FString& InNickname)
{
	if (HasAuthority())
	{
		PlayerNickname = InNickname;
		OnRep_PlayerNickname();
	}
}

void ALobbyPlayerState::OnRep_ReadyState()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (IsValid(PlayerController) == true)
	{
		ALobbyPreviewPawn* PreviewPawn = Cast<ALobbyPreviewPawn>(PlayerController->GetPawn());
		if (IsValid(PreviewPawn) == true)
		{
			PreviewPawn->SetReadyState(bIsReady);
		}
	}
}

void ALobbyPlayerState::OnRep_PlayerNickname()
{
	UWorld* World = GetWorld();
	if (!World) return;

	for (TActorIterator<ALobbyPreviewPawn> It(World); It; ++It)
	{
		ALobbyPreviewPawn* PreviewPawn = *It;
		if (!IsValid(PreviewPawn)) continue;

		if (PreviewPawn->GetPlayerState() == this)
		{
			PreviewPawn->SetPlayerName(PlayerNickname);
		}
	}
}
void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, PlayerNickname);
	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}
