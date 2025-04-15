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

UCLASS()
class CCFF_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterController();
	
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
	/** Guard Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GuardAction;
	/** Dodge Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;
	/** Burst Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BurstAction;

	UFUNCTION(Server, Reliable, WithValidation, Category = "CCFF|Flow")
	void ServerReturnToLobby();

	UFUNCTION(Server, Reliable, WithValidation, Category = "Arena|Flow")
	void ServerSetNickname(const FString& InNickname);

	bool bIsPause;

protected:
	virtual void BeginPlay() override;

protected:
#pragma region GamePause
	UFUNCTION(BlueprintCallable, Category = "CCFF|UI")
	void TogglePause();

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CCFF|UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
	UUserWidget* PauseWidget;
#pragma endregion

};
