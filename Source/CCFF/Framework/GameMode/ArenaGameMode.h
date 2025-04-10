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

	// ���� ����/����, üũ �Լ� (���� ���ѿ��� �����)
	virtual void StartRound() override;
	virtual void EndRound() override;
	virtual void CheckGameConditions() override;

#pragma region Arena
	// ��Ƽ�÷��̿� ���� RPC �Լ� (Ŭ���̾�Ʈ�� ȣ���ϸ� �������� ����)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartRound();
	virtual void Server_StartRound_Implementation();
	virtual bool Server_StartRound_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EndRound();
	virtual void Server_EndRound_Implementation();
	virtual bool Server_EndRound_Validate();
	
	void InternalStartRound();

	// TODO :: ���� �ý���(��ý�Ʈ, ���)

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
