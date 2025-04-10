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
	void ServerSetNickname(const FString& InNickname);

	UFUNCTION(Server, Reliable)
	void ServerToggleReady();

	void SetLobbyCameraView();

protected:
	void ShowLobbyUI();
	
	void HandleLocalSetup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULobbyWidget> LobbyWidgetInstance;

};
