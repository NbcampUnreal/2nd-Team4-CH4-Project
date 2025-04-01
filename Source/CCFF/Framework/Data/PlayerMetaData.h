#pragma once

#include "CoreMinimal.h"
#include "PlayerMetaData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 SaveVersion = 1;	// 저장 구조 변경 시 비교용. GameInstance에서 로딩 버전 판단에 사용.

	UPROPERTY(BlueprintReadWrite)
	FString PlayerUniqueID;

	UPROPERTY(BlueprintReadWrite)
	FString Nickname;

};
