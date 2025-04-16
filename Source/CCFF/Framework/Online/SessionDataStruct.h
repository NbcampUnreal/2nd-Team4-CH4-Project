#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "SessionDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString SessionName;

	UPROPERTY(BlueprintReadWrite)
	FString IPAddress;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxPlayers;


	FSessionInfo()
		: SessionName(TEXT(""))
		, IPAddress(TEXT(""))
		, CurrentPlayers(0)
		, MaxPlayers(0)
	{
	}

};
