#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Framework/Data/PlayerMetaData.h"
#include "Framework/Data/ArenaSubModeType.h"
#include "Framework/Online/SessionDataStruct.h"
#include "CCFFGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionsFound, const TArray<FSessionInfo>&, Sessions);

UCLASS()
class CCFF_API UCCFFGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	void SaveData();

	void StartFindSessions(APlayerController* OwnerPlayerController);

	UFUNCTION()
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	FORCEINLINE FString GetNickname() const { return PlayerMeta.Nickname; }
	void SetNickname(const FString& NewNickname);

	FORCEINLINE FName GetSelectedCharacterID() const { return PlayerMeta.SelectedCharacterID; }
	void SetSelectedCharacterID(FName NewID);

	FORCEINLINE const FPlayerMetaData& GetPlayerMeta() const { return PlayerMeta; }
	void SetPlayerMeta(const FPlayerMetaData& NewPlayerMeta);

	FORCEINLINE FString GetServerIP() const { return ServerIP; }
	void SetServerIP(const FString& NewServerIP);

	FORCEINLINE EArenaSubMode GetArenaSubMode() const { return ArenaSubMode; }
	FORCEINLINE void SetArenaSubMode(EArenaSubMode NewMode) { ArenaSubMode = NewMode; }

	UPROPERTY(BlueprintAssignable)
	FOnSessionsFound OnSessionsFound;

private:
	static const int32 CurrentSaveVersion;

	UPROPERTY()
	FPlayerMetaData PlayerMeta;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString ServerIP = TEXT("127.0.0.1:7777");

	UPROPERTY()
	EArenaSubMode ArenaSubMode = EArenaSubMode::Elimination;


#pragma region BGM
public:
	UFUNCTION()
	void PlayBGMForCurrentMap();

	UFUNCTION()
	void StopCurrentBGM();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundMix* MasterSoundMix;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundClass* MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundClass* BGMSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundClass* SFXSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TMap<FName, USoundBase*> MapNameToBGMMap;

private:
	UPROPERTY()
	UAudioComponent* CurrentBGMComponent;

	FString GetCleanMapName() const;

#pragma endregion


#pragma region LOCAL_LOGIN_SYSTEM
public:
	void ApplyUserSettings(const FPlayerMetaData& Meta);
	bool TryLogin(const FString& ID, const FString& InputPassword);

private:
	void ImportAccountsFromJSON();
	bool UpdateAccountInSaveGame(const FPlayerMetaData& UpdatedMeta);

#pragma endregion

#pragma region CUSTOMIZATION
public:
	FORCEINLINE void SetLobbyPresetIndex(int32 NewIndex) { LobbyPresetIndex = NewIndex; }
	FORCEINLINE int32 GetLobbyPresetIndex() const { return LobbyPresetIndex; }

private:
	int32 LobbyPresetIndex = -1;

#pragma endregion
};
