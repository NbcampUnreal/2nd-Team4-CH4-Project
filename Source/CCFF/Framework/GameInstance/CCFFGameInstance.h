#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Framework/Data/PlayerMetaData.h"
#include "Framework/Online/SessionDataStruct.h"
#include "CCFFGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionsFound, const TArray<FSessionInfo>&, Sessions);

UCLASS()
class CCFF_API UCCFFGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void SaveData();
	void LoadData();

	void StartFindSessions(APlayerController* OwnerPlayerController);

	FORCEINLINE FString GetNickname() const { return PlayerMeta.Nickname; }
	void SetNickname(const FString& NewNickname);

	FORCEINLINE const FPlayerMetaData& GetPlayerMeta() const { return PlayerMeta; }
	void SetPlayerMeta(const FPlayerMetaData& NewPlayerMeta);

	FORCEINLINE FString GetServerIP() const { return ServerIP; }
	void SetServerIP(const FString& NewServerIP);

	UPROPERTY(BlueprintAssignable)
	FOnSessionsFound OnSessionsFound;

private:
	static const int32 CurrentSaveVersion;

	UPROPERTY()
	FPlayerMetaData PlayerMeta;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString ServerIP = TEXT("127.0.0.1");

};
