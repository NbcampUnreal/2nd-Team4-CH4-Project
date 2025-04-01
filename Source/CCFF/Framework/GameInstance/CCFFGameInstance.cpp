#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SaveGame/CCFFSaveGame.h"

const int32 UCCFFGameInstance::CurrentSaveVersion = 1;

void UCCFFGameInstance::SetNickname(const FString& NewNickname)
{
	PlayerMeta.Nickname = NewNickname;
}

void UCCFFGameInstance::SetPlayerMeta(const FPlayerMetaData& NewPlayerMeta)
{
	PlayerMeta = NewPlayerMeta;
}

void UCCFFGameInstance::SaveData()
{
	if (PlayerMeta.Nickname.IsEmpty())
	{
		return;
	}

	UCCFFSaveGame* SaveGameObject = Cast<UCCFFSaveGame>(UGameplayStatics::CreateSaveGameObject(UCCFFSaveGame::StaticClass()));
	if (SaveGameObject)
	{
		PlayerMeta.SaveVersion = CurrentSaveVersion;
		SaveGameObject->SavedMetaData = PlayerMeta;

		const FString SlotName = FString::Printf(TEXT("PlayerSaveSlot_%s"), *PlayerMeta.Nickname);
		UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);
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
		UCCFFSaveGame* LoadedGame = Cast<UCCFFSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (LoadedGame)
		{
			int32 LoadedGameVersion = LoadedGame->SavedMetaData.SaveVersion;
			if (LoadedGameVersion <= CurrentSaveVersion)
			{
				PlayerMeta = LoadedGame->SavedMetaData;
			}
		}
	}
}
