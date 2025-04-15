// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
class UUserWidget;
class ABaseCharacter;

UCLASS()
class CCFF_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterController();
	
#pragma region InputMapping
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInputAction>> AttackAction;
	
#pragma endregion

#pragma region Character
public:
	UFUNCTION(Server, Reliable, WithValidation, Category = "CCFF|Flow")
	void ServerReturnToLobby();

	UFUNCTION(Server, Reliable, WithValidation, Category = "CCFF|Character")
	void ServerSetNickname(const FString& InNickname);

	UFUNCTION(Server, Reliable, WithValidation, Category = "CCFF|Character")
	void ServerSetCharacterID(FName InID);


#pragma endregion

#pragma region DeathAndRespawn

public:
	void HandleRiverOverlap(ABaseCharacter* DeadPawn, AActor* OtherActor);
	void OnPawnDeath(ABaseCharacter* DeadPawn);


protected:
	UFUNCTION(Server, Reliable)
	void ServerRequestRespawn();

	void StartRespawnTimer(float Delay);
	void EnterSpectatorMode();

private:
	FTimerHandle RespawnTimerHandle;

#pragma endregion

	virtual void BeginPlay() override;


#pragma region GamePause
protected:
	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void TogglePause();

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
	UUserWidget* PauseWidget;

public:
	bool bIsPause;
#pragma endregion

};
