#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

UCLASS()
class CCFF_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerToggleReady();

	UFUNCTION(Server, Reliable)
	void ServerRequestStartGame();
	
	UFUNCTION(Server, Reliable)
	void ServerSetNickname(const FString& InNickname);

};
