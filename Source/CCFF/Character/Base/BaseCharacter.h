// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterStats.h"
#include "CharacterAnim.h"
#include "CharacterEnum.h"
#include "DamageAble.h"
#include "InputActionValue.h"
#include "Character/Base/DamageAble.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UBattleComponent;
class UItemInteractionComponent;
struct FInputActionValue;
struct FHitBoxData;

UCLASS()
class CCFF_API ABaseCharacter : public ACharacter, public IDamageAble
{
	GENERATED_BODY()

public:
	ABaseCharacter();

public:
#pragma region GetFunction
	FORCEINLINE float GetMaxHealth() const { return Stats.MaxHealth; }
	FORCEINLINE float GetHealth() const { return Stats.Health; }
#pragma endregion
	
#pragma region SetFunction
	FORCEINLINE void SetMaxHealth(const float& Value) { Stats.MaxHealth = Value; }
	FORCEINLINE void SetHealth(const float& Value) { Stats.Health = Value; }
#pragma endregion
protected:
#pragma region Override

	// === Character Override Functions ===
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	//Interface Override Functions
	virtual float TakeDamage_Implementation(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser,FHitBoxData& HitData) override;

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
	FTimerHandle HitlagTimerHandle;
	UPROPERTY()
	FTimerHandle BlockstunTimerHandle;
#pragma endregion

#pragma region Cached variables
	UPROPERTY()
	FVector2D CurrentMoveInput;
	UPROPERTY()
	FVector StoredVelocity;
	UPROPERTY()
	FVector StoredKnockbackAngle;
	UPROPERTY()
	float StoredKnockbackForce;
#pragma endregion

#pragma region DataPreLoad
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadCharacterStats();
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadCharacterBalanceStats();
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadAttackCollisions();
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadCharacterAnim();
	UFUNCTION(BlueprintCallable, Category = "DataLoad")
	void PreLoadBattleModifiers();
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

protected:
#pragma region AttackCollisionData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox/Collision")
	int32 CurrentActivatedCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox/Collision")
	TArray<UShapeComponent*> AttackCollisions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox/Data")
	TArray<FHitBoxData> HitBoxList;
#pragma endregion
	
protected:
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
	FCharacterBalanceStats BalanceStats;
	//Attack and Hitted Animation Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FCharacterAnim Anim;
#pragma endregion

protected:
#pragma region CombatEffect
	// === Damage & Reaction ===
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	float TakeNormalDamage(float Damage, float MinimumDamage);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeHitstun(int32 Hitstun);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void EndHitstun();
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeHitlag(int32 Hitlag);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void EndHitlag();
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeHitlagAndStoredKnockback(int32 Hitlag, FVector KnockbackAngle, float KnockbackForce);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void EndHitlagAndTakeKnocback();
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeBlockstun(int32 Blockstun);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void EndBlockstun();
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void TakeKnockback(FVector KnockbackAngle, float KnockbackForce);
	UFUNCTION(BlueprintCallable, Category = "Combat/Effect")
	void GuardCrush();

#pragma endregion

#pragma region CombatReaction
	// === Hit Reaction ===
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void OnAttackHit(float Damage);
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void OnAttackBlocked();
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ProcessHitReaction(ABaseCharacter* Attacker, FHitBoxData& HitData);
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ReceiveNormalHit(ABaseCharacter* Attacker, FHitBoxData& HitData);
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ReceiveArmorHit(ABaseCharacter* Attacker, FHitBoxData& HitData);
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ReceiveBlock(ABaseCharacter* Attacker, FHitBoxData& HitData);
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void ReceiveGrabbed();
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void Clash(ABaseCharacter* Attacker, FHitBoxData& HitData);
	UFUNCTION(BlueprintCallable, Category = "Combat/Reaction")
	void OnDeath() const;

#pragma endregion	

private:
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


