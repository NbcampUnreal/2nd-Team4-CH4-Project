// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterStats.h"
#include "CharacterAnim.h"
#include "CharacterEnum.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UBattleComponent;
class UItemInteractionComponent;
struct FInputActionValue;

UCLASS()
class CCFF_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

#pragma region Override

	// === Character Override Functions ===
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

#pragma endregion

#pragma region AttackAnimation
	UFUNCTION()
	void OnAttackOverlap(
		UPrimitiveComponent* OverlappedComponent,  // 오버랩된 콜리전 컴포넌트
		AActor* OtherActor,                        // 충돌한 다른 액터
		UPrimitiveComponent* OtherComp,            // 충돌한 액터의 컴포넌트
		int32 OtherBodyIndex,                      // 충돌한 바디의 인덱스
		bool bFromSweep,                           // Sweep에 의한 오버랩 여부
		const FHitResult& SweepResult              // 충돌 결과 정보
	);
	UFUNCTION()
	void AttackNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	UFUNCTION()
	void OnAttackEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void DeactivateAttackCollision(const int32 Index) const;
#pragma endregion 
	
protected:

#pragma region Timer
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
protected:
#pragma region DataPreLoad
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadCharacterStats();
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadCharacterMovementStats();
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadAttackCollisions();
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadCharacterAnim();
#pragma endregion
	
#pragma region MoveFunction
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);

#pragma endregion
#pragma region AttackFunctions
	void Attack1(const FInputActionValue& Value);
	void Attack2(const FInputActionValue& Value);
	void Attack3(const FInputActionValue& Value);
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FCharacterMovementStats MovementStats;
	//Attack and Hitted Animation Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FCharacterAnim Anim;
#pragma endregion
protected:
#pragma region Components
	// === Components ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBattleComponent* BattleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UItemInteractionComponent* ItemInteractionComponent;
#pragma endregion
	
private:

#pragma region Meter
	// === Meter Handling ===
	UFUNCTION(BlueprintCallable, Category = "Combat/Meter")
	void ModifyGuardMeter(float GuardMeterDamage);
	UFUNCTION(BlueprintCallable, Category = "Combat/Meter")
	void ModifySuperMeter(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Combat/Meter")
	void GainBurstMeter(float Amount);
#pragma endregion
	
};


