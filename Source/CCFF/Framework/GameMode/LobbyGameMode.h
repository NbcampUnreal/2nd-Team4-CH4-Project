#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class CCFF_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();

	void NotifyPlayerReadyStatusChanged();

	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:
	virtual FString SelectMapForMatch() const;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	TArray<FString> AvailableMapPaths;

	UPROPERTY(EditAnywhere, Category = "Lobby")
	int32 SelectedMapIndex = -1;

};
