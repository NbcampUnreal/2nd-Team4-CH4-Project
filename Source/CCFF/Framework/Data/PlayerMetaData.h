#pragma once

#include "CoreMinimal.h"
#include "PlayerMetaData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 SaveVersion = 1;	// ���� ���� ���� �� �񱳿�. GameInstance���� �ε� ���� �Ǵܿ� ���.

	UPROPERTY(BlueprintReadWrite)
	FString PlayerUniqueID;

	UPROPERTY(BlueprintReadWrite)
	FString Nickname;

};
