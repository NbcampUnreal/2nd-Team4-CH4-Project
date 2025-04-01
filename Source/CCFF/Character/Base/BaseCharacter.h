// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterStats.h"
#include "CharacterEnum.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class CCFF_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	ABaseCharacter();

protected:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	//Character State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ECharacterState CurrentCharacterState;
	//Character Resistance State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EResistanceState CurrentResistanceState;
	//Character Stats struct
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FCharacterStats Stats;
	
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
};
