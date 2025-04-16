#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/SaveGame/CCFFSaveGame.h"
#include "Framework/HUD/MainMenuHUD.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Components/AudioComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

const int32 UCCFFGameInstance::CurrentSaveVersion = 1;

void UCCFFGameInstance::Init()
{
	Super::Init();

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UCCFFGameInstance::HandleNetworkFailure);
	}

	if (!UGameplayStatics::DoesSaveGameExist(TEXT("LocalUserDatabase"), 0))
	{
		ImportAccountsFromJSON();
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
		SaveInstance->LastLoggedInNickname = PlayerMeta.PlayerUniqueID;

		UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("LocalUserDatabase"), 0);
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

void UCCFFGameInstance::PlayBGMForCurrentMap()
{
	if (IsRunningDedicatedServer()) return;

	StopCurrentBGM();

	const FString CleanMapName = GetCleanMapName();
	const FName MapKey = FName(*CleanMapName);

	if (USoundBase** FoundBGM = MapNameToBGMMap.Find(MapKey))
	{
		if (*FoundBGM)
		{
			CurrentBGMComponent = UGameplayStatics::SpawnSound2D(this, *FoundBGM);
		}
	}
}


void UCCFFGameInstance::StopCurrentBGM()
{
	if (CurrentBGMComponent)
	{
		CurrentBGMComponent->Stop();
		CurrentBGMComponent = nullptr;
	}
}

FString UCCFFGameInstance::GetCleanMapName() const
{
	FString FullMapName = GetWorld() ? GetWorld()->GetMapName() : TEXT("NULL");

	if (FullMapName.StartsWith(TEXT("UEDPIE")))
	{
		int32 LastUnderscore = FullMapName.Find(TEXT("_"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (LastUnderscore != INDEX_NONE)
		{
			return FullMapName.RightChop(LastUnderscore + 1);
		}
	}

	return FullMapName;
}

void UCCFFGameInstance::ImportAccountsFromJSON()
{
	// 1. Load File Path
	const FString FilePath = FPaths::ProjectSavedDir() + TEXT("Accounts/sample_accounts.json");
	FString FileContent;

	if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("[AccountImport] Failed to load file: %s"), *FilePath);
		return;
	}

	// 2. Parsing Json File (Cast to TJsonObject)
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
	TSharedPtr<FJsonObject> RootObject;

	if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[AccountImport] Failed to parse JSON"));
		return;
	}

	// Iterate through each account entry in the JSON root
	TArray<FPlayerMetaData> AccountList;

	for (const auto& Elem : RootObject->Values)
	{
		const FString& ID = Elem.Key;
		const TSharedPtr<FJsonObject> AccountObj = Elem.Value->AsObject();
		if (!AccountObj.IsValid()) continue;

		// Create and fill FPlayerMetaData for each account
		FPlayerMetaData Meta;
		Meta.PlayerUniqueID = ID;

		// Hash the password using MD5 for security
		Meta.Password = FMD5::HashAnsiString(*AccountObj->GetStringField("Password"));
		Meta.Nickname = AccountObj->GetStringField("Nickname");

		// Parse Audio settings
		if (TSharedPtr<FJsonObject> AudioObj = AccountObj->GetObjectField("AudioSettings"))
		{
			Meta.AudioSettings.Master.Volume = AudioObj->GetNumberField("Volume");
		}

		// Parse Screen Resolution
		const TArray<TSharedPtr<FJsonValue>>* ResArray;
		if (AccountObj->TryGetArrayField("Resolution", ResArray) && ResArray->Num() == 2)
		{
			Meta.Resolution.X = static_cast<int32>((*ResArray)[0]->AsNumber());
			Meta.Resolution.Y = static_cast<int32>((*ResArray)[1]->AsNumber());
		}

		// Parse WindowMode from String
		FString ModeStr = AccountObj->GetStringField("WindowMode").ToLower();
		if (ModeStr == "fullscreen")
			Meta.WindowMode = EWindowMode::WindowedFullscreen;
		else if (ModeStr == "windowed")
			Meta.WindowMode = EWindowMode::Windowed;
		else if (ModeStr == "borderless")
			Meta.WindowMode = EWindowMode::Fullscreen;

		AccountList.Add(Meta);
	}

	// Save all parsed accounts SaveGame object
	UCCFFSaveGame* SaveGame = Cast<UCCFFSaveGame>(UGameplayStatics::CreateSaveGameObject(UCCFFSaveGame::StaticClass()));
	if (SaveGame)
	{
		SaveGame->AllAccounts = AccountList;

		const bool bSaved = UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("LocalUserDatabase"), 0);
		UE_LOG(LogTemp, Log, TEXT("[AccountImport] Imported %d accounts, SaveGame result: %s"),
			AccountList.Num(), bSaved ? TEXT("Success") : TEXT("Failed"));
	}
}

bool UCCFFGameInstance::UpdateAccountInSaveGame(const FPlayerMetaData& UpdatedMeta)
{
	const FString SlotName = TEXT("LocalUserDatabase");
	UCCFFSaveGame* SaveGame = Cast<UCCFFSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!SaveGame) return false;

	for (FPlayerMetaData& Account : SaveGame->AllAccounts)
	{
		if (Account.PlayerUniqueID == UpdatedMeta.PlayerUniqueID)
		{
			Account = UpdatedMeta;
			return UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
		}
	}
	return false;
}

bool UCCFFGameInstance::TryLogin(const FString& ID, const FString& InputPassword)
{
	const FString SlotName = TEXT("LocalUserDatabase");
	UCCFFSaveGame* SaveGame = Cast<UCCFFSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!SaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("[TryLogin] SaveGame not found"));
		return false;
	}

	// 입력된 비밀번호를 해시 처리
	const FString InputPasswordHash = FMD5::HashAnsiString(*InputPassword);

	for (const FPlayerMetaData& Meta : SaveGame->AllAccounts)
	{
		if (Meta.PlayerUniqueID == ID)
		{
			// 비밀번호 해시 비교
			if (Meta.Password == InputPasswordHash)
			{
				PlayerMeta = Meta;
				ApplyUserSettings(Meta);

				UE_LOG(LogTemp, Log, TEXT("[TryLogin] Login success: %s (%s)"), *ID, *Meta.Nickname);
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[TryLogin] Password mismatch for ID: %s"), *ID);
				return false;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[TryLogin] ID not found: %s"), *ID);
	return false;
}

void UCCFFGameInstance::ApplyUserSettings(const FPlayerMetaData& Meta)
{
	PlayerMeta = Meta;

	if (!MasterSoundMix || !MasterSoundClass || !BGMSoundClass || !SFXSoundClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SoundClass or Mix is NULL!"));
		return;
	}

	auto ApplyAudio = [&](const FAudioCategorySetting& Category, USoundClass* SoundClass)
		{
			const float Volume = Category.bMuted ? 0.0f : Category.Volume;
			UGameplayStatics::SetSoundMixClassOverride(this, MasterSoundMix, SoundClass, Volume, 1.0f, 0.0f, true);
		};

	ApplyAudio(Meta.AudioSettings.Master, MasterSoundClass);
	ApplyAudio(Meta.AudioSettings.BGM, BGMSoundClass);
	ApplyAudio(Meta.AudioSettings.SFX, SFXSoundClass);

	UGameplayStatics::PushSoundMixModifier(this, MasterSoundMix);
}
