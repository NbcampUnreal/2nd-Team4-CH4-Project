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

	UFUNCTION(Client, Reliable)
	void UpdateCountdownWidget(int32 NewTime);

	UFUNCTION(Client, Reliable)
	void ClientTeardownCountdown();

#pragma region FORBID_SOLO_PLAYING
	UFUNCTION(Exec)
	void SetAllowSoloStart(int32 Allow);

	UFUNCTION(Server, Reliable)
	void ServerSetAllowSoloStart(int32 Allow);

#pragma endregion

protected:
	void ShowLobbyUI();
	
	void HandleLocalSetup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULobbyWidget> LobbyWidgetInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCountdownWidget> CountdownWidgetClass;

	UPROPERTY()
	UCountdownWidget* CountdownWidgetInstance;

};
