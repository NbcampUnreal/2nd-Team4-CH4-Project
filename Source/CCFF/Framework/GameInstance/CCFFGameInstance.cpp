#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SaveGame/CCFFSaveGame.h"

const int32 UCCFFGameInstance::CurrentSaveVersion = 1;

void UCCFFGameInstance::Init()
{
	Super::Init();
	LoadData();
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

void UCCFFGameInstance::SetNickname(const FString& NewNickname)
{
	PlayerMeta.Nickname = NewNickname;
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
