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

class UBoxComponent;
class UUW_HPWidget;
class UStatusComponent;
class UHPWidgetComponent;
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
#pragma region Widget
	void SetHPWidget(UUW_HPWidget* InHPWidget);
	void SetHUDWidget(UUserWidget* HUDWidget);
#pragma endregion
	
#pragma region GetFunction
	FORCEINLINE FString GetCharacterType() const { return CharacterType; };
#pragma endregion
	
#pragma region Override
protected:
	// === Character Override Functions ===
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

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
	
#pragma region Timer
protected:
	// === Timers ===
	UPROPERTY()
	FTimerHandle HitstunTimerHandle;
	UPROPERTY()
	FTimerHandle HitlagTimerHandle;
	UPROPERTY()
	FTimerHandle BlockstunTimerHandle;
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
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);
	UFUNCTION(Server,Reliable)
	void ServerRPCSetMaxWalkSpeed(const float Value);
	
#pragma endregion

#pragma region AttackFunctions
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerRPCAttack(const int32 Num, float InStartAttackTime);
	UFUNCTION(Client,Unreliable)
	void ClientRPCPlayAttackMontage(const int32 Num, ABaseCharacter* InTargetCharacter);
	UFUNCTION()
	void PlayAttackMontage(const int32& Num);
	UFUNCTION()
	void Attack1(const FInputActionValue& Value);
	UFUNCTION()
	void Attack2(const FInputActionValue& Value);
	UFUNCTION()
	void Attack3(const FInputActionValue& Value);
	UFUNCTION()
	void Attack4(const FInputActionValue& Value);
	UFUNCTION()
	void OnRep_CanAttack();
	UFUNCTION()
	void ExecuteAttackByIndex(const int32 Index);
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
	void OnDeath();

	UFUNCTION()
	void SwitchToSpectatorCamera();
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
	
protected:
#pragma region AttackCollisionData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox/Collision")
	int32 CurrentActivatedCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox/Collision")
	TArray<UBoxComponent*> AttackCollisions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox/Data")
	TArray<FHitBoxData> HitBoxList;
#pragma endregion
	
protected:
#pragma region Components
	// === Components ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBattleComponent* BattleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UItemInteractionComponent* ItemInteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHPWidgetComponent> HPWidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStatusComponent> StatusComponent;
#pragma endregion

protected:
#pragma region Character Status
	//Character Type
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	FString CharacterType;
	//Character State
	UPROPERTY(Replicated)
	ECharacterState CurrentCharacterState;
	//Character Resistance State
	UPROPERTY(Replicated)
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
	
#pragma region Cached variables
	UPROPERTY()
	FVector2D CurrentMoveInput;
	UPROPERTY(ReplicatedUsing=OnRep_CanAttack)
	uint8 bCanAttack : 1;
	UPROPERTY()
	FVector StoredVelocity;
	UPROPERTY()
	FVector StoredKnockbackAngle;
	UPROPERTY()
	float StoredKnockbackForce;
	UPROPERTY()
	float LastAttackStartTime;
	UPROPERTY()
	float ServerDelay;
	UPROPERTY()
	float PrevMontagePlayTime;

	FVector2D LastMoveInputDirection;
	float LastMoveInputTime;
	float DoubleTapThreshold;
	uint8 bIsDoubleTab;
	
#pragma endregion
	
	
};







