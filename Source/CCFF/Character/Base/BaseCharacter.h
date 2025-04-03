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
class UBattleComponent;
struct FInputActionValue;

UCLASS()
class CCFF_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

#pragma region CharacterOverride

	// === Character Override Functions ===
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion

#pragma region Components

protected:
	// === Components ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBattleComponent* BattleComponent;

#pragma endregion

#pragma region StateAndStats

protected:
	// === Stats ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FCharacterStats Stats;

	// === State ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ECharacterState CurrentCharacterState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EResistanceState CurrentResistanceState;

	// === Data Table ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* DT;

	// === Timers ===
	UPROPERTY()
	FTimerHandle HitstunTimerHandle;
	UPROPERTY()
	FTimerHandle HitLagTimerHandle;
	UPROPERTY()
	FTimerHandle BlockStunTimerHandle;
	UPROPERTY()
	FVector StoredVelocity;

#pragma endregion

#pragma region Input

protected:
#pragma region DataPreLoad
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadCharacterStats();
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadAttackCollisions();
#pragma endregion
	
#pragma region MoveFunction
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);

#pragma endregion

#pragma region CombatEffect

private:
	// === Damage & Reaction ===
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeNormalDamage(float Damage, float MinimumDamage);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeHitstun(int32 Hitstun);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void EndHitstun();
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeHitLag(int32 Hitlag);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void EndHitLag();
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeBlockStun(int32 BlockStun);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void EndBlockStun();
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeKnockback(FVector KnockbackAngle, float KnockbackForce, FVector2D DIInput);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void GuardCrush();

#pragma endregion

#pragma region CombatReaction

private:
	// === Hit Reaction ===
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void OnAttackHit() const;
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void OnAttackBlocked() const;
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ProcessHitReaction(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ReceiveNormalHit(AActor* DamageCauser);
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ReceiveArmorHit(float Damage) const;
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ReceiveBlock(AActor* DamageCauser) const;
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ReceiveGrabbed();
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void Clash(AActor* DamageCauser) const;
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void OnDeath() const;

#pragma endregion

#pragma region Meter

#pragma endregion

#pragma region Override
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
#pragma endregion
	
protected:
	
#pragma region AttackCollision
	TArray<UShapeComponent*> AttackCollisions;
#pragma endregion
	
#pragma region Character Status
	//Character Type
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	FString CharacterType;
	//Character State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ECharacterState CurrentCharacterState;
	//Character Resistance State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EResistanceState CurrentResistanceState;
	//Character Stats struct
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FCharacterStats Stats;
#pragma endregion
	
private:
	// === Meter Handling ===
	UFUNCTION(BlueprintCallable, Category = "Combat/Meter")
	void ModifyGuardMeter(float GuardMeterDamage);
	UFUNCTION(BlueprintCallable, Category = "Combat/Meter")
	void ModifySuperMeter(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Combat/Meter")
	void GainBurstMeter(float Amount);

#pragma endregion
};
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
};

