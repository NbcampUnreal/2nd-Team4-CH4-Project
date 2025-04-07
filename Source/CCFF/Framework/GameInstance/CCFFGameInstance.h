#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Framework/Data/PlayerMetaData.h"
#include "CCFFGameInstance.generated.h"

UCLASS()
class CCFF_API UCCFFGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FORCEINLINE FString GetNickname() const { return PlayerMeta.Nickname; }
	void SetNickname(const FString& NewNickname);

	FORCEINLINE const FPlayerMetaData& GetPlayerMeta() const { return PlayerMeta; }
	void SetPlayerMeta(const FPlayerMetaData& NewPlayerMeta);

	void SaveData();
	void LoadData();

private:
	/* 현재 세이브 버전. 구조체 변경 시 증가 */
	static const int32 CurrentSaveVersion;

	UPROPERTY()
	FPlayerMetaData PlayerMeta;

};
