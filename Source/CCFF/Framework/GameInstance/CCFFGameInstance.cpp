#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SaveGame/CCFFSaveGame.h"
#include "Framework/HUD/MainMenuHUD.h"

const int32 UCCFFGameInstance::CurrentSaveVersion = 1;

void UCCFFGameInstance::Init()
{
	Super::Init();
	LoadData();

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UCCFFGameInstance::HandleNetworkFailure);
	}
}

void UCCFFGameInstance::Shutdown()
{
	Super::Shutdown();

	if (GEngine)
	{
		GEngine->OnNetworkFailure().RemoveAll(this);
	}
}

void UCCFFGameInstance::SaveData()
{
	if (PlayerMeta.Nickname.IsEmpty())
	{
		return;
	}

	UCCFFSaveGame* SaveInstance = Cast<UCCFFSaveGame>(UGameplayStatics::CreateSaveGameObject(UCCFFSaveGame::StaticClass()));
	if (SaveInstance)
	{
		SaveInstance->SaveVersion = CurrentSaveVersion;
		SaveInstance->PlayerMeta = PlayerMeta;
		SaveInstance->ServerIP = ServerIP;

		const FString SlotName = FString::Printf(TEXT("PlayerSaveSlot_%s"), *PlayerMeta.Nickname);
		UGameplayStatics::SaveGameToSlot(SaveInstance , SlotName, 0);
	}
}

void UCCFFGameInstance::LoadData()
{
	if (PlayerMeta.Nickname.IsEmpty())
	{
		return;
	}

	const FString SlotName = FString::Printf(TEXT("PlayerSaveSlot_%s"), *PlayerMeta.Nickname);
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UCCFFSaveGame* SaveInstance = Cast<UCCFFSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (SaveInstance)
		{
			int32 LoadedGameVersion = SaveInstance->PlayerMeta.SaveVersion;
			if (LoadedGameVersion <= CurrentSaveVersion)
			{
				PlayerMeta = SaveInstance->PlayerMeta;
				ServerIP = SaveInstance->ServerIP;
			}
		}
	}
}

void UCCFFGameInstance::StartFindSessions(APlayerController* OwnerPlayerController)
{
	TArray<FSessionInfo> DummySessions;

	FSessionInfo Session;
	Session.SessionName = TEXT("Test Room");
	Session.CurrentPlayers = 1;
	Session.MaxPlayers = 4;
	Session.IPAddress = TEXT("127.0.0.1:7777");

	DummySessions.Add(Session);

	OnSessionsFound.Broadcast(DummySessions);
}

void UCCFFGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Error, TEXT("[NetworkFailure] Type: %d, Reason: %s"), static_cast<int32>(FailureType), *ErrorString);

	FText MessageToShow = FText::FromString(ErrorString);
	if (ErrorString.Contains(TEXT("in-game")))
	{
		MessageToShow = FText::FromString(TEXT("서버가 게임 중입니다. 잠시 후 다시 시도해주세요."));
	}
	else if (ErrorString.Contains(TEXT("full")))
	{
		MessageToShow = FText::FromString(TEXT("로비가 가득 찼습니다. 잠시 후 다시 시도해주세요."));
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (PlayerController)
	{
		AMainMenuHUD* HUD = Cast<AMainMenuHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			HUD->ShowErrorPopup(MessageToShow);
		}
	}
}

void UCCFFGameInstance::SetNickname(const FString& NewNickname)
{
	PlayerMeta.Nickname = NewNickname;
}

void UCCFFGameInstance::SetSelectedCharacterID(FName NewID)
{
	PlayerMeta.SelectedCharacterID = NewID;
}

void UCCFFGameInstance::SetPlayerMeta(const FPlayerMetaData& NewPlayerMeta)
{
	PlayerMeta = NewPlayerMeta;
}

void UCCFFGameInstance::SetServerIP(const FString& NewServerIP)
{
	ServerIP = NewServerIP;
	SaveData();
}
