#pragma once

#include "ArenaSubModeType.generated.h"

UENUM(BlueprintType)
enum class EArenaSubMode : uint8
{
	Elimination	UMETA(DisplayName = "Elimination"),
	DeathMatch	UMETA(DisplayName = "DeathMatch")
};
