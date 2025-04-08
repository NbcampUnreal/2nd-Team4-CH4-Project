#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

UCLASS()
class CCFF_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALobbyPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void SetReady(bool bNewReady);

	UFUNCTION()
	void OnRep_ReadyState();

	FORCEINLINE bool IsReady() const { return bIsReady; };

	UPROPERTY(Replicated)
	FString PlayerNickname;

	UPROPERTY(Replicated = OnRep_RedayStated)
	bool bIsReady = false;

};
