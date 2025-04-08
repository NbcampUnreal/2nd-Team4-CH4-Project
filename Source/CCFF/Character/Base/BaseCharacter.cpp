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
#include "InputActionValue.h"
#include "Character/DataLoaderSubSystem.h"
#include "Character/Base/AttackCollisionData.h"
#include "Components/SphereComponent.h"
#include "Items/Component/ItemInteractionComponent.h"
#include "Character/DamageHelper.h"


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
	CameraBoom->SetWorldRotation((FRotator(-35, -180, 0)));
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritYaw=false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CurrentCharacterState=ECharacterState::Normal;
	CurrentResistanceState=EResistanceState::Normal;
	
	ItemInteractionComponent = CreateDefaultSubobject<UItemInteractionComponent>(TEXT("ItemInteractionComponent"));
	
	CurrentActivatedCollision=-1;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//애니메이션 Notify 이벤트 연결
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&ABaseCharacter::AttackNotify);
	}
	PreLoadCharacterStats();
	PreLoadAttackCollisions();
	PreLoadCharacterAnim();
}

float ABaseCharacter::TakeDamage_Implementation(float DamageAmount, const FDamageEvent& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser, FHitBoxData& HitData)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ABaseCharacter* Attacker = Cast<ABaseCharacter>(DamageCauser);
	ProcessHitReaction(Attacker, HitData);
	
	return ActualDamage;
}

void ABaseCharacter::AttackNotify(const FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (!NotifyName.IsValid()||AttackCollisions.IsEmpty()) return;
	// Determin the type of attack by name
	FString NotifyNameString=NotifyName.ToString();
	// Hitbox 
	if(NotifyNameString.Contains(TEXT("Hitbox")))
	{
		TCHAR LastChar = NotifyNameString[NotifyNameString.Len() - 1];
		int32 AttackNumber = FCString::Atoi(&LastChar)-1;
		//UE_LOG(LogTemp, Log, TEXT("Parsed Attack Number: %d"), AttackNumber);	
		// Activate Collision in proper location
		if (AttackCollisions[AttackNumber])
		{
			// Deactivate Collision
			if (NotifyNameString.Contains(TEXT("End")))
			{
				CurrentActivatedCollision=-1;
				DeactivateAttackCollision(AttackNumber);
				UE_LOG(LogTemp,Warning,TEXT("Deactivate Collision! (Index: %d)"),AttackNumber);
			}
			else // Activate Collision
			{
				CurrentActivatedCollision=AttackNumber;
				AttackCollisions[AttackNumber]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				UE_LOG(LogTemp,Warning,TEXT("Activate Collision! (Index: %d)"),AttackNumber);
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Activate Failed!"));
		}
		return;
	}
}

void ABaseCharacter::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Except self
	if (!OtherActor || OtherActor == this) return;
	
	float DamageAmount=Stats.DamageModifier*HitBoxList[CurrentActivatedCollision].Damage;
	UE_LOG(LogTemp,Warning,TEXT("Damage: %f, Overlapped Actor: %s"),DamageAmount,*OtherActor->GetName());
	UDamageHelper::ApplyDamage(
	OtherActor,                // 피해 대상
	DamageAmount,			   // 공격력 (Character Stats 기반)
	GetController(),           // 공격한 플레이어의 컨트롤러
	this,                      // 공격한 액터 (자기 자신)
	UDamageType::StaticClass(),// 기본 데미지 타입)
	HitBoxList[CurrentActivatedCollision]
	);
}


void ABaseCharacter::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
}

void ABaseCharacter::DeactivateAttackCollision(const int32 Index) const
{
	AttackCollisions[Index]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ABaseCharacter::PreLoadCharacterMovementStats()
{
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			MovementStats=Loader->InitializeMovementStat(FName(CharacterType));
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
				HitBoxList.SetNum(n-1);
				for (int32 i=0;i<n-1;i++)
				{
					FString TypeName=Type->GetNameStringByIndex(i);
					UE_LOG(LogTemp,Warning,TEXT("Current RowName: %s, Current Index: %d"),*TypeName,i);
					const FName RowName=FName(CharacterType+"_"+TypeName);
					FAttackCollisionData Data=Loader->InitializeAttackCollisionData(RowName);
					FHitBoxData HitBoxData=Loader->InitializeHitBoxData(RowName);
					if (Data.Scale!=FVector::ZeroVector)
					{
						//Create Collision and Attacth to mesh
						UShapeComponent* AttackCollision = NewObject<UShapeComponent>(this,USphereComponent::StaticClass());
						AttackCollision->SetupAttachment(GetMesh());
						AttackCollision->SetRelativeLocation(Data.Location);
						AttackCollision->SetRelativeScale3D(Data.Scale);
						AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						//Bind Overlap Event
						AttackCollision->OnComponentBeginOverlap.AddDynamic(this,&ABaseCharacter::OnAttackOverlap);
						//Activate Components
						AddInstanceComponent(AttackCollision);
						AttackCollision->RegisterComponent();
						AttackCollisions[i]=AttackCollision;
						HitBoxList[i]=HitBoxData;
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

			//Attack Actions
			EnhancedInputComponent->BindAction(MyController->AttackAction1,ETriggerEvent::Triggered,this,&ABaseCharacter::Attack1);
			EnhancedInputComponent->BindAction(MyController->AttackAction2,ETriggerEvent::Triggered,this,&ABaseCharacter::Attack2);
			EnhancedInputComponent->BindAction(MyController->AttackAction3,ETriggerEvent::Triggered,this,&ABaseCharacter::Attack3);
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

	CurrentMoveInput = MovementVector;

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

void ABaseCharacter::Attack1(const FInputActionValue& Value)
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
		AnimInstance->OnMontageEnded.AddDynamic(this,&ABaseCharacter::OnAttackEnded);
	}
}
void ABaseCharacter::Attack2(const FInputActionValue& Value)
{
	//메시 유효 
	if (!GetMesh()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//둘다 유효
	if (AnimInstance&&Anim.AttackMontage2)
	{
		//몽타주 실행
		AnimInstance->Montage_Play(Anim.AttackMontage2);
		//몽타주 끝났을 때 이벤트 바인딩
		AnimInstance->OnMontageEnded.Clear();
		AnimInstance->OnMontageEnded.AddDynamic(this,&ABaseCharacter::OnAttackEnded);
	}
}
void ABaseCharacter::Attack3(const FInputActionValue& Value)
{
	//메시 유효 
	if (!GetMesh()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//둘다 유효
	if (AnimInstance&&Anim.AttackMontage3)
	{
		//몽타주 실행
		AnimInstance->Montage_Play(Anim.AttackMontage3);
		//몽타주 끝났을 때 이벤트 바인딩
		AnimInstance->OnMontageEnded.Clear();
		AnimInstance->OnMontageEnded.AddDynamic(this,&ABaseCharacter::OnAttackEnded);
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

float ABaseCharacter::TakeNormalDamage(float Damage, float MinimumDamage)
{
	float ScaledDamage = BattleComponent->ComboStaleDamage(Damage, MinimumDamage);
	float NewHealth = FMath::Clamp(Stats.Health - ScaledDamage, 0.0f, Stats.MaxHealth);
	Stats.Health = NewHealth;

	ModifySuperMeter(BattleComponent->GetMeterGainFromDamageTaken(ScaledDamage));

	if (NewHealth <= 0.0f)
	{
		OnDeath();
	}
	return ScaledDamage;
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

void ABaseCharacter::TakeHitlag(int32 Hitlag)
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

	GetWorldTimerManager().ClearTimer(HitlagTimerHandle);
	GetWorldTimerManager().SetTimer(HitlagTimerHandle, this, &ABaseCharacter::EndHitlag, Hitlag / 60.0f, false);
}

void ABaseCharacter::EndHitlag()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(MOVE_Walking);
		Movement->Velocity = StoredVelocity;
	}
	GetMesh()->bPauseAnims = false;
}

void ABaseCharacter::TakeBlockstun(int32 Blockstun)
{
	CurrentCharacterState = ECharacterState::Blockstun;
	GetWorldTimerManager().ClearTimer(BlockstunTimerHandle);
	GetWorldTimerManager().SetTimer(
		BlockstunTimerHandle,
		this,
		&ABaseCharacter::EndBlockstun,
		Blockstun / 60.0f,
		false
	);
}

void ABaseCharacter::EndBlockstun()
{
	if (CurrentCharacterState == ECharacterState::Blockstun)
	{
		CurrentCharacterState = ECharacterState::Normal;
	}
}

void ABaseCharacter::TakeKnockback(FVector KnockbackAngle, float KnockbackForce)
{
	FVector KnockbackVelocity = BattleComponent->KnockbackDir(KnockbackAngle, KnockbackForce, CurrentMoveInput, Stats.DiModifier);
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


void ABaseCharacter::OnAttackHit(float Damage)
{
	TakeHitlag(HitBoxList[CurrentActivatedCollision].Hitlag);
	ModifySuperMeter(Damage * Stats.AttackSuperGainMultiplier);
	//히트 이펙트
	//히트 사운드
}

void ABaseCharacter::OnAttackBlocked()
{
	TakeHitlag(HitBoxList[CurrentActivatedCollision].Hitlag);
	ModifySuperMeter(HitBoxList[CurrentActivatedCollision].GuardDamage * Stats.AttackSuperGainMultiplier);
}

void ABaseCharacter::ProcessHitReaction(ABaseCharacter* Attacker, FHitBoxData& HitData)
{
	if (HitData.Type == EHitBoxType::Throw)
	{
		ReceiveGrabbed();
		return;
	}
	if (CurrentCharacterState == ECharacterState::Block || CurrentCharacterState == ECharacterState::Blockstun)
	{
		ReceiveBlock(Attacker, HitData);
		return;
	}
	if (HitData.Type == EHitBoxType::Burst)
	{
		ReceiveNormalHit(Attacker, HitData);
		return;
	}
	if (HitData.Type == EHitBoxType::Projectile)
	{
		if (CurrentResistanceState == EResistanceState::ProjectileInvulnerable)
		{
			return;
		}
		if (CurrentResistanceState == EResistanceState::ProjectileArmor)
		{
			ReceiveArmorHit(Attacker, HitData);
			return;
		}
	}
	if (CurrentResistanceState == EResistanceState::Invulnerable)
	{
		return;
	}
	if (CurrentResistanceState == EResistanceState::HyperArmor)
	{
		ReceiveArmorHit(Attacker, HitData);
		return;
	}
	ReceiveNormalHit(Attacker, HitData);
	return;
}

void ABaseCharacter::ReceiveNormalHit(ABaseCharacter* Attacker, FHitBoxData& HitData)
{
	int32 VictimHitlag = HitData.VictimHitlag;
	int32 Hitstun = HitData.Hitstun;
	float Damage = HitData.Damage;

	if (CurrentCharacterState == ECharacterState::Attack || CurrentCharacterState == ECharacterState::Grab)
	{
		VictimHitlag = FMath::RoundToInt(VictimHitlag * CurrentBattleModifiers.CounterHitlagModifier);
		Hitstun = FMath::RoundToInt(Hitstun * CurrentBattleModifiers.CounterHitlagModifier);
		Damage *= CurrentBattleModifiers.CounterDamageModifier;
		//카운터 사운드
	}

	CurrentResistanceState = EResistanceState::Normal;
	TakeHitlag(VictimHitlag);
	TakeHitstun(Hitstun);
	TakeNormalDamage(Damage, HitData.MinimumDamage);
	TakeKnockback(HitData.KnockbackAngle, HitData.KnockbackForce);
	BattleComponent->IncreaseCombo();

	Attacker->OnAttackHit(TakeNormalDamage(Damage, HitData.MinimumDamage));
	return;
}

void ABaseCharacter::ReceiveArmorHit(ABaseCharacter* Attacker, FHitBoxData& HitData)
{
	float ArmorDamage = HitData.Damage * CurrentBattleModifiers.ArmorDamageModifier;
	TakeHitlag(CurrentBattleModifiers.ArmorHitlag);
	TakeNormalDamage(ArmorDamage, 0.0f);
	//아머 이펙트
	//아머 사운드
	Attacker->OnAttackHit(ArmorDamage);
	return;
}

void ABaseCharacter::ReceiveBlock(ABaseCharacter* Attacker, FHitBoxData& HitData)
{
	TakeBlockstun(HitData.Blockstun);
	TakeNormalDamage(HitData.GuardDamage, 0.0f);
	ModifyGuardMeter(-HitData.GuardMeterDamage);
	TakeKnockback(HitData.KnockbackAngle, HitData.GuardPushback);
	//가드 이펙트
	//가드 사운드
	Attacker->OnAttackBlocked();
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

void ABaseCharacter::Clash(FHitBoxData& HitData)
{
	TakeHitlag(HitBoxList[CurrentActivatedCollision].Hitlag);
	TakeKnockback(HitData.KnockbackAngle, HitData.KnockbackForce);
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