#pragma once

#include "CoreMinimal.h"
#include "Framework/GameMode/BaseInGameMode.h"
#include "ArenaGameMode.generated.h"

class ABaseCharacter;

UCLASS()
class CCFF_API AArenaGameMode : public ABaseInGameMode
{
	GENERATED_BODY()
	
public:
	AArenaGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	// 라운드 시작/종료, 체크 함수 (서버 권한에서 실행됨)
	virtual void StartRound() override;
	virtual void EndRound() override;
	virtual void CheckGameConditions() override;

#pragma region Arena
	// 멀티플레이용 서버 RPC 함수 (클라이언트가 호출하면 서버에서 실행)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartRound();
	virtual void Server_StartRound_Implementation();
	virtual bool Server_StartRound_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EndRound();
	virtual void Server_EndRound_Implementation();
	virtual bool Server_EndRound_Validate();
	
	void InternalStartRound();

	// TODO :: 업적 시스템(어시스트, 등등)

public:
	void UpdateArenaStats();
	void UpdatePlayerRating();
	void UpdateCountdown();

	float CountdownTime;

#pragma endregion

private:
	FTimerHandle ArenaTimerHandle;
	FTimerHandle CountdownTimerHandle;

};
