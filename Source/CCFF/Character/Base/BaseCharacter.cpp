// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BattleComponent.h"
#include "CharacterAnim.h"
#include "InputActionValue.h"
#include "Character/DataLoaderSubSystem.h"
#include "Character/Base/AttackCollisionData.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 48.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f; // The camera follows at this distance behind the character
	CameraBoom->SetWorldRotation(FRotator(-35, 0, 0));
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritYaw=false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CurrentCharacterState=ECharacterState::Normal;
	CurrentResistanceState=EResistanceState::Normal;
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//애니메이션 Notify 이벤트 연결
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&ABaseCharacter::AttackNormalNotify);
	}
	PreLoadCharacterStats();
	PreLoadAttackCollisions();
	PreLoadCharacterAnim();
}

void ABaseCharacter::AttackNormalNotify(const FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName!="NormalAttack"||AttackCollisions.IsEmpty()) return;
	// Activate Collision in proper location
	if (AttackCollisions[0])
	{
		AttackCollisions[0]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Activate Failed!"));
	}
}

void ABaseCharacter::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Except self
	if (!OtherActor || OtherActor == this) return;

	// Apply Damage
	UGameplayStatics::ApplyDamage(
		OtherActor,                // 피해 대상
		Stats.AttackPower,         // 공격력 (Character Stats 기반)
		GetController(),           // 공격한 플레이어의 컨트롤러
		this,                      // 공격한 액터 (자기 자신)
		UDamageType::StaticClass() // 기본 데미지 타입
	);
	UE_LOG(LogTemp, Warning, TEXT("%s hit %s!"), *GetName(), *OtherActor->GetName());
}

void ABaseCharacter::OnAttackNormalEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AttackCollisions.IsEmpty()) return;
	if (AttackCollisions[0])
	{
		AttackCollisions[0]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseCharacter::PreLoadCharacterStats()
{
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			Stats=Loader->InitializeStat(FName(CharacterType));
		}
	}
}

void ABaseCharacter::PreLoadCharacterAnim()
{
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			Anim=Loader->InitializeCharacterAnim(FName(CharacterType));
		}
	}
}

void ABaseCharacter::PreLoadAttackCollisions()
{
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			if (UEnum* Type=StaticEnum<EAttackType>())
			{
				const int32 n=Type->NumEnums();
				AttackCollisions.SetNum(n-1);
				for (int32 i=0;i<n-1;i++)
				{
					FString TypeName=Type->GetNameStringByIndex(i);
					UE_LOG(LogTemp,Warning,TEXT("Current RowName: %s, Current Index: %d"),*TypeName,i);
					const FName RowName=FName(CharacterType+"_"+TypeName);
					FAttackCollisionData Data=Loader->InitializeAttackCollisionData(RowName);
					if (Data.Scale!=FVector::ZeroVector)
					{
						//Create Collision and Attacth to Mesh
						UShapeComponent* AttackCollision = NewObject<UShapeComponent>(this,USphereComponent::StaticClass());
						AttackCollision->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Root");
						AttackCollision->SetRelativeLocation(Data.Location);
						AttackCollision->SetRelativeScale3D(Data.Scale);
						AttackCollision->OnComponentBeginOverlap.AddDynamic(this,&ABaseCharacter::OnAttackOverlap);
						AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						AddInstanceComponent(AttackCollision);
						AttackCollision->RegisterComponent();
						AttackCollisions[i]=AttackCollision;
					}
				}
			}
		}
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (ACharacterController* MyController=Cast<ACharacterController>(GetController()))
		{
			// Moving
			EnhancedInputComponent->BindAction(MyController->MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		
			// Jumping
			EnhancedInputComponent->BindAction(MyController->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(MyController->JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			//Attack Normal
			EnhancedInputComponent->BindAction(MyController->AttackNormalAction,ETriggerEvent::Triggered,this,&ABaseCharacter::AttackNormal);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::StartJump(const FInputActionValue& Value)
{
	// Jump 함수는 Character가 기본 제공
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void ABaseCharacter::StopJump(const FInputActionValue& Value)
{
	// StopJumping 함수도 Character가 기본 제공
	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}

void ABaseCharacter::AttackNormal(const FInputActionValue& Value)
{
	//메시 유효 
	if (!GetMesh()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//둘다 유효
	if (AnimInstance&&Anim.AttackMontage1)
	{
		//몽타주 실행
		AnimInstance->Montage_Play(Anim.AttackMontage1);
		//몽타주 끝났을 때 이벤트 바인딩
		AnimInstance->OnMontageEnded.Clear();
		AnimInstance->OnMontageEnded.AddDynamic(this,&ABaseCharacter::OnAttackNormalEnded);
	}
}

void ABaseCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (ACharacterController* MyController = Cast<ACharacterController>(PlayerController))
			{
				Subsystem->AddMappingContext(MyController->DefaultMappingContext, 0);
			}
		}
	}
}



float ABaseCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stats.Health = FMath::Clamp(Stats.Health - DamageAmount, 0.0f, Stats.MaxHealth);
	UE_LOG(LogTemp,Warning,TEXT("Damage: %f"),ActualDamage);
	
	//ProcessHitReaction(DamageAmount, DamageEvent, EventInstigator, DamageCauser); Error Detected (Crash)
	
	return ActualDamage;
}

void ABaseCharacter::TakeNormalDamage(float Damage, float MinimumDamage)
{
	float ScaledDamage = BattleComponent->ComboStaleDamage(Damage, MinimumDamage);
	float NewHealth = FMath::Clamp(Stats.Health - ScaledDamage, 0.0f, Stats.MaxHealth);
	Stats.Health = NewHealth;

	ModifySuperMeter(BattleComponent->GetMeterGainFromDamageTaken(ScaledDamage));

	if (NewHealth <= 0.0f)
	{
		OnDeath();
	}
}

void ABaseCharacter::TakeHitstun(int32 Hitstun)
{
	if (Hitstun == 0)
	{
		return;
	}

	float ScaledHitstun = BattleComponent->ComboStaleHitstun(Hitstun);
	CurrentCharacterState = ECharacterState::Hitted;

	GetWorldTimerManager().ClearTimer(HitstunTimerHandle);

	GetWorldTimerManager().SetTimer(
		HitstunTimerHandle,
		this,
		&ABaseCharacter::EndHitstun,
		ScaledHitstun / 60.0f,
		false
	);
}

void ABaseCharacter::EndHitstun()
{
	CurrentCharacterState = ECharacterState::Normal;
	BattleComponent->ResetCombo();
}

void ABaseCharacter::TakeHitLag(int32 Hitlag)
{
	if (Hitlag == 0)
	{
		return;
	}
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		if (Movement->MovementMode != MOVE_None)
		{
			StoredVelocity = Movement->Velocity;
			Movement->DisableMovement();
		}
	}
	GetMesh()->bPauseAnims = true;

	GetWorldTimerManager().ClearTimer(HitLagTimerHandle);
	GetWorldTimerManager().SetTimer(HitLagTimerHandle, this, &ABaseCharacter::EndHitLag, Hitlag / 60.0f, false);
}

void ABaseCharacter::EndHitLag()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(MOVE_Walking);
		Movement->Velocity = StoredVelocity;
	}
	GetMesh()->bPauseAnims = false;
}

void ABaseCharacter::TakeBlockStun(int32 BlockStun)
{
	CurrentCharacterState = ECharacterState::BlockStun;
	GetWorldTimerManager().ClearTimer(BlockStunTimerHandle);
	GetWorldTimerManager().SetTimer(
		BlockStunTimerHandle,
		this,
		&ABaseCharacter::EndBlockStun,
		BlockStun / 60.0f,
		false
	);
}

void ABaseCharacter::EndBlockStun()
{
	if (CurrentCharacterState == ECharacterState::BlockStun)
	{
		CurrentCharacterState = ECharacterState::Normal;
	}
}

void ABaseCharacter::TakeKnockback(FVector KnockbackAngle, float KnockbackForce, FVector2D DiInput)
{
	FVector KnockbackVelocity = BattleComponent->KnockbackDir(KnockbackAngle, KnockbackForce, DiInput, Stats.DiModifier);
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->StopMovementImmediately();
	}

	LaunchCharacter(KnockbackVelocity, true, true);
}

void ABaseCharacter::GuardCrush()
{
	CurrentCharacterState = ECharacterState::Hitted;
}


void ABaseCharacter::OnAttackHit() const
{
	//TakeHitLag(Hitlag);
	//GainSuperMeter(Damage * Stats.AttackSuperGainMultiplier);
	//히트 이펙트
	//히트 사운드
}

void ABaseCharacter::OnAttackBlocked() const
{
	//TakeHitLag(Hitlag);
	//GainSuperMeter(GuardDamage * Stats.AttackSuperGainMultiplier);
}

void ABaseCharacter::ProcessHitReaction(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//if (/*잡기면*/)
	//{
	//	ReceiveGrabbed();
	//	return;
	//}
	if (CurrentCharacterState == ECharacterState::Block || CurrentCharacterState == ECharacterState::BlockStun)
	{
		ReceiveBlock(DamageCauser);
		return;
	}
	//if (/*버스트면*/)
	//{
	//	ReceiveNormalHit();
	//	return;
	//}
	//if (/*투사체 확인*/)
	//{
	//	if (/*투사체 무적이면*/)
	//	{
	//		return;
	//	}
	//	if (/*투사체 아머면*/)
	//	{
	//		ReceiveArmorHit();
	//		return;
	//	}
	//}
	if (CurrentResistanceState == EResistanceState::Invulnerable)
	{
		return;
	}
	if (CurrentResistanceState == EResistanceState::HyperArmor)
	{
		ReceiveArmorHit(DamageAmount);
		return;
	}
	ReceiveNormalHit(DamageCauser);
	return;
}

void ABaseCharacter::ReceiveNormalHit(AActor* DamageCauser)
{
	if (CurrentCharacterState == ECharacterState::Attack || CurrentCharacterState == ECharacterState::Grab)
	{
		/*Hitlag *= BattleComponent->CounterHitStunModifier;
		VictimHitlag *= BattleComponent->CounterHitStunModifier;
		Hitstun *= BattleComponent->CounterHitStunModifier;
		Damage *= BattleComponent->CounterDamageModifier;*/
		//카운터 사운드
	}
	CurrentResistanceState = EResistanceState::Normal;
	/*ApplyHitLag(VictimHitlag);
	TakeHitstun(Hitstun);
	TakeDamage(Damage, MinimumDamage);
	TakeKnockback(KnockbackAngle, KnockbackForce, DiInput);*/
	BattleComponent->IncreaseCombo();
	return;
}

void ABaseCharacter::ReceiveArmorHit(float Damage) const
{
	/*TakeHitLag(ArmorHitlag);
	TakeDamage(Damage * ArmorDamageModifier);*/
	//아머 이펙트
	//아머 사운드
	return;
}

void ABaseCharacter::ReceiveBlock(AActor* DamageCauser) const
{
	/*ApplyBlockStun(BlockStun);
	ApplyDamage(GuardDamage, 0.0f);
	ModifyGuardMeter(-GuardMeterDamage);
	ApplyKnockback(Direction, GuardPushback, FVector2D(0.0f, 0.0f), 0.0f);*/
	//가드 이펙트
	//가드 사운드
	return;
}

void ABaseCharacter::ReceiveGrabbed()
{
	if (CurrentResistanceState == EResistanceState::ThrowInvulnerable)
	{
		return;
	}
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->DisableMovement();
	}
	CurrentResistanceState = EResistanceState::Normal;
	CurrentCharacterState = ECharacterState::Grabbed;
}

void ABaseCharacter::Clash(AActor* DamageCauser) const
{
	/*ApplyKnockback(KnockbackAngle, KnockbackForce, FVector2D(0.0f, 0.0f), 0.0f);*/
}

void ABaseCharacter::OnDeath() const
{
	// 사망 애니메이션 재생, 입력 차단, 리스폰 타이머 등
}

void ABaseCharacter::ModifyGuardMeter(float Amount)
{
	Stats.BlockMeter = FMath::Clamp(Stats.BlockMeter + Amount, 0.0f, Stats.MaxBlockMeter);

	if (Stats.BlockMeter <= 0.0f)
	{
		GuardCrush();
	}
}

void ABaseCharacter::ModifySuperMeter(float Amount)
{
	float NewSuperMeter = FMath::Clamp(Stats.SuperMeter + Amount, 0.0f, Stats.MaxSuperMeter);
	Stats.SuperMeter = NewSuperMeter;
}

void ABaseCharacter::GainBurstMeter(float Amount)
{
	float NewBurstMeter = FMath::Clamp(Stats.BurstMeter + Amount, 0.0f, Stats.MaxBurstMeter);
	Stats.BurstMeter = NewBurstMeter;
}