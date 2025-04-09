#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNicknameUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyStateChanged, bool, bReady);

UCLASS()
class CCFF_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void SetReady(bool bNewReady);
	FORCEINLINE bool IsReady() const { return bIsReady; };

	void SetPlayerNickname(const FString& InNickname);
	FORCEINLINE FString GetPlayerNickname() const { return PlayerNickname; }

	FOnNicknameUpdated OnNicknameUpdated;
	FOnReadyStateChanged OnReadyStateChanged;

	UFUNCTION()
	void OnRep_ReadyState();

	UFUNCTION()
	void OnRep_PlayerNickname();

protected:
	UPROPERTY(Replicated = OnRep_ReadyStated)
	bool bIsReady = false;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerNickname)
	FString PlayerNickname;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
