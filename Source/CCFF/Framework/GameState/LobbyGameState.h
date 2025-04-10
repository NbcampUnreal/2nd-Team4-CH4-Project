#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllowStartGameChanged, bool);

UCLASS()
class CCFF_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	bool EvaluateStartCondition() const;

	void UpdateAllowStartGame();

	UFUNCTION()
	void OnRep_bAllowStartGame();

	FOnAllowStartGameChanged OnAllowStartGameChanged;

	UPROPERTY(ReplicatedUsing = OnRep_bAllowStartGame, BlueprintReadOnly, Category = "Lobby")
	bool bAllowStartGame = false;

};
