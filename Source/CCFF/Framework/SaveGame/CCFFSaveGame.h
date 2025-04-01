#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Framework/Data/PlayerMetaData.h"
#include "CCFFSaveGame.generated.h"

UCLASS()
class CCFF_API UCCFFSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FPlayerMetaData SavedMetaData;
	
};
