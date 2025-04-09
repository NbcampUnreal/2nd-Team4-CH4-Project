#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

UCLASS()
class CCFF_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	bool AreAllPlayersReady() const;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
	bool bAllowStartGame = false;

	void UpdateAllowStartGame();

};
