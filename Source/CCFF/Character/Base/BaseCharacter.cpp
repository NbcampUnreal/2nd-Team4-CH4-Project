// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "ClearReplacementShaders.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraActor.h"  
#include "Camera/CameraComponent.h"
#include "Character/Components/BattleComponent.h"
#include "Character/Components/HPWidgetComponent.h"
#include "Character/Components/StatusComponent.h"
#include "Character/DataStruct/AttackCollisionData.h"
#include "Character/Utilities/DamageHelper.h"
#include "Character/Utilities/DataLoaderSubSystem.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Controller/CharacterController.h"
#include "Framework/GameMode/ArenaGameMode.h"
#include "Framework/GameState/ArenaGameState.h"
#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/Character/UW_HPWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Component/ItemInteractionComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter():
	BufferThreshold(0.5f),
	CurrentActivatedCollision(-1),
	bCanAttack(true),
	LastAttackStartTime(0.f),
	ServerDelay(0.f),
	LastMoveInputTime(0.f),
	DoubleTapThreshold(0.3f),
	bIsDoubleTab(false)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 48.0f);
		
	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	JumpMaxCount=2;

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

	//Create Status Component and HPWidgetComp
	StatusComponent=CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	HPWidgetComponent=CreateDefaultSubobject<UHPWidgetComponent>(TEXT("HPWidgetComp"));
	HPWidgetComponent->SetupAttachment(GetRootComponent());
	HPWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 85.f));
	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HPWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//Create BattleComponent
	BattleComponent=CreateDefaultSubobject<UBattleComponent>(TEXT("BattleComponent"));
	
	CurrentCharacterState=ECharacterState::Normal;
	CurrentResistanceState=EResistanceState::Normal;
	
	ItemInteractionComponent = CreateDefaultSubobject<UItemInteractionComponent>(TEXT("ItemInteractionComponent"));
}

void ABaseCharacter::SetHPWidget(UUW_HPWidget* InHPWidget)
{
	UUW_HPWidget* HPWidget=Cast<UUW_HPWidget>(InHPWidget);
	if (IsValid(HPWidget)==true&&!HasAuthority())
	{
		HPWidget->InitializeHPWidget(StatusComponent);
		StatusComponent->OnCurrentHPChanged.AddUObject(HPWidget,&UUW_HPWidget::OnHPChange);
	}
}

void ABaseCharacter::SetHUDWidget(UUserWidget* HUDWidget)
{
	if (UBaseInGameWidget* MyHUD=Cast<UBaseInGameWidget>(HUDWidget))
	{
		MyHUD->InitializeHUDWidget(StatusComponent);
		StatusComponent->OnCurrentHPChanged.AddUObject(MyHUD,&UBaseInGameWidget::UpdateHealthBar);
		StatusComponent->OnSuperMeterChanged.AddUObject(MyHUD,&UBaseInGameWidget::UpdateSuperMeterBar);
		StatusComponent->OnBurstMeterChanged.AddUObject(MyHUD,&UBaseInGameWidget::UpdateBurstMeterBar);
		//UE_LOG(LogTemp,Display,TEXT("CharacterController SetHud Call"));
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Rotate Camera properly
	if (IsValid(CameraBoom))
	{
		const float CameraRotation=GetActorRotation().Yaw;
		CameraBoom->SetRelativeRotation((FRotator(-35, CameraRotation-180, 0)));
	}
	// Binding Event Notify and End
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&ABaseCharacter::AttackNotify);
		AnimInstance->OnMontageEnded.AddDynamic(this,&ABaseCharacter::OnAttackEnded);
	}
	if (StatusComponent)
	{
		StatusComponent->OnDeathState.AddUObject(this,&ABaseCharacter::OnDeath);
		StatusComponent->OnGuardCrush.AddUObject(this,&ABaseCharacter::GuardCrush);
	}
	//Initialize struct variables
	PreLoadCharacterStats();
	PreLoadAttackCollisions();
	PreLoadCharacterAnim();
	PreLoadCharacterBalanceStats();
	PreLoadBattleModifiers();
	//Set MaxWalkSpeed
	GetCharacterMovement()->MaxWalkSpeed = BalanceStats.MaxWalkSpeed;
	// FString NetModeString = UDamageHelper::GetRoleString(this);
	// FString CombinedString = FString::Printf(TEXT("%s::BeginPlay() %s [%s]"), *CharacterType , *UDamageHelper::GetNetModeString(this), *NetModeString);
	// UE_LOG(LogTemp,Warning,TEXT("bCanAttack: %d"),bCanAttack);
	// UDamageHelper::MyPrintString(this, CombinedString, 10.f);

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnPlayerOverlapRiver);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// // Client (Rotating Widget to focus camera)
	// if (IsValid(HPWidgetComponent)==true&&HasAuthority()==false)
	// {
	// 	FVector WidgetCompLocation=HPWidgetComponent->GetComponentLocation();
	// 	FVector LocalPlayerCameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
	// 	HPWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetCompLocation, LocalPlayerCameraLocation));
	// }
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// FString NetModeString = UDamageHelper::GetRoleString(this);
	// FString CombinedString = FString::Printf(TEXT("BaseCharacter::PossessedBy() %s [%s]"), *UDamageHelper::GetNetModeString(this), *NetModeString);
	// UDamageHelper::MyPrintString(this, CombinedString, 10.f);
}

float ABaseCharacter::TakeDamage_Implementation(
	float DamageAmount,
	const FDamageEvent& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser,
	FHitBoxData& HitData)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ABaseCharacter* Attacker = Cast<ABaseCharacter>(DamageCauser);
	ProcessHitReaction(Attacker, HitData);
	
	return ActualDamage;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,CurrentCharacterState);
	DOREPLIFETIME(ThisClass,CurrentResistanceState);
	DOREPLIFETIME(ThisClass,bCanAttack);
}

void ABaseCharacter::AttackNotify(const FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (!HasAuthority())
	{
		//Client logic
		// Particle, Effect
		FString NotifyNameString = NotifyName.ToString();
		TCHAR LastChar = NotifyNameString[NotifyNameString.Len() - 1];
		int32 AttackNumber = FCString::Atoi(&LastChar)-1;
		DrawDebugBox(GetWorld(),AttackCollisions[AttackNumber]->GetComponentLocation(),AttackCollisions[AttackNumber]->GetScaledBoxExtent(),AttackCollisions[AttackNumber]->GetComponentQuat(),FColor::Red,false,2.0f);
		return;
	}
	// Server logic
	// Determin the type of attack by name
	FString NotifyNameString = NotifyName.ToString();

	if (!NotifyName.IsValid()||AttackCollisions.IsEmpty()) return;
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
	if (!HasAuthority()) return;
	// Server Logic
	// Except self
	if (!OtherActor || OtherActor == this) return;
	UE_LOG(LogTemp,Warning,TEXT("Overlapped Actor: %s"),*OtherActor->GetName());
	
	float DamageAmount=BalanceStats.DamageModifier*HitBoxList[CurrentActivatedCollision].Damage;
	UDamageHelper::ApplyDamage(
	OtherActor,                // 피해 대상
	DamageAmount,			   // 공격력 (Character Stats 기반)
	GetController(),           // 공격한 플레이어의 컨트롤러
	this,                      // 공격한 액터 (자기 자신)
	UDamageType::StaticClass(),// 기본 데미지 타입)
	HitBoxList[CurrentActivatedCollision]
	);

	// Notice :: 다혜 테스트 추가
	if (AController* PC = GetController())
	{
		if (AArenaPlayerState* ArenaPS = PC->GetPlayerState<AArenaPlayerState>())
		{
			ArenaPS->AddDamage(DamageAmount); // 누적!
		}
	}
}

void ABaseCharacter::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentCharacterState = ECharacterState::Normal;
}

void ABaseCharacter::DeactivateAttackCollision(const int32 Index) const
{
	AttackCollisions[Index]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ABaseCharacter::ServerRPCAttack_Implementation(const int32 Num, float InStartAttackTime)
{
	UE_LOG(LogTemp,Warning,TEXT("ServerRPC Called with %d !!"),Num);
	ServerDelay=GetWorld()->GetTimeSeconds()-InStartAttackTime;
	const float MontagePlayTime=Anim.AttackMontage[Num]->GetPlayLength();
	// 0<=ServerDelay<=MontagePlayTime
	ServerDelay=FMath::Clamp(ServerDelay,0.f,MontagePlayTime);
	PrevMontagePlayTime=MontagePlayTime;
	// Consider ServerDelay Timer (Can Attack)
	if (KINDA_SMALL_NUMBER<MontagePlayTime-ServerDelay)
	{
		bCanAttack=false;
		OnRep_CanAttack();
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle,FTimerDelegate::CreateLambda([&]()
		{
			bCanAttack=true;
			OnRep_CanAttack();
		}),
		MontagePlayTime-ServerDelay,false,-1.f);
	}
	LastAttackStartTime=InStartAttackTime;
	PlayAttackMontage(Num);
	
	for (APlayerController* PC:TActorRange<APlayerController>(GetWorld()))
	{
		if (IsValid(PC)==true&&GetController()!=PC) // Find Other Client
		{
			if (ABaseCharacter* OtherClientCharacter=Cast<ABaseCharacter>(PC->GetPawn()))
			{
				OtherClientCharacter->ClientRPCPlayAttackMontage(Num,this);
			}
		}
	}
}

bool ABaseCharacter::ServerRPCAttack_Validate(const int32 Num, float InStartAttackTime)
{
	return true;
	// First Attack input
	// if (LastAttackStartTime==0.f)
	// {
	// 	return true;
	// }
	// const bool bIsValid=(PrevMontagePlayTime-0.1f)<(InStartAttackTime-LastAttackStartTime);
	// if (!bIsValid)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("ServerRPCAttack_Validate failed. InCheckTime: %f, LastTime: %f, MontagePlayTime : %f"), InStartAttackTime, LastAttackStartTime, PrevMontagePlayTime);
	// }
	// return bIsValid;
}
void ABaseCharacter::ClientRPCPlayAttackMontage_Implementation(const int32 Num, ABaseCharacter* InTargetCharacter)
{
	if (IsValid(InTargetCharacter)==true)
	{
		InTargetCharacter->PlayAttackMontage(Num);
	}
}

void ABaseCharacter::ExecuteBufferedAction()
{
	const float CurrentTime=GetWorld()->GetTimeSeconds();
	const int32 InputAction=static_cast<int32>(InputBuffer.InputAttack);
	//Execute Buffered Input Action
	if (InputAction>=0&&InputAction<8&&(CurrentTime-InputBuffer.BufferedTime<=BufferThreshold))
	{
		ExecuteAttackByIndex(InputAction);
		UE_LOG(LogTemp,Warning,TEXT("Execute Buffered Input(Index: %d)"),InputAction);
	}
	// Clear Buffer
	InputBuffer=FBufferedInput();
}

void ABaseCharacter::OnRep_CanAttack()
{
	if (bCanAttack==true)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		ExecuteBufferedAction();
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}

void ABaseCharacter::ExecuteAttackByIndex(const int32 Index)
{
	if (bCanAttack&&GetCharacterMovement()->IsFalling()==false)
	{
		//UE_LOG(LogTemp,Warning,TEXT("Attack1 Called !!"));
		ServerRPCAttack(Index,GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
		// Play Montage in Owning Client
		if (HasAuthority()==false&&IsLocallyControlled()==true)
		{
			//bIsCancelable = Index<=2;
			bCanAttack=false;
			OnRep_CanAttack();
			PlayAttackMontage(Index);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Buffered Input Index: %d"),Index);
		InputBuffer=FBufferedInput(static_cast<EAttackType>(Index),GetWorld()->GetTimeSeconds());
	}
}

void ABaseCharacter::Attack1(const FInputActionValue& Value)
{
	ExecuteAttackByIndex(0);
}
void ABaseCharacter::Attack2(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->MaxWalkSpeed==BalanceStats.MaxRunSpeed)
	{
		ExecuteAttackByIndex(2);		
	}
	else
	{
		ExecuteAttackByIndex(1);
	}
}


void ABaseCharacter::Attack4(const FInputActionValue& Value)
{
	ExecuteAttackByIndex(3);
}

void ABaseCharacter::Attack5(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->MaxWalkSpeed==BalanceStats.MaxRunSpeed)
	{
		ExecuteAttackByIndex(5);
	}
	else
	{
		ExecuteAttackByIndex(4);
	}
}

void ABaseCharacter::Attack7(const FInputActionValue& Value)
{
	ExecuteAttackByIndex(6);
}

void ABaseCharacter::Attack8(const FInputActionValue& Value)
{
	ExecuteAttackByIndex(7);
}

void ABaseCharacter::PlayAttackMontage(const int32& Num)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//둘다 유효
	if (AnimInstance&&Anim.AttackMontage[Num])
	{
		//몽타주 실행
		AnimInstance->Montage_Play(Anim.AttackMontage[Num]);
		CurrentCharacterState = ECharacterState::Attack;
	}
}

void ABaseCharacter::ServerRPCSetMaxWalkSpeed_Implementation(const float Value)
{
	GetCharacterMovement()->MaxWalkSpeed=Value;
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	//Detect Double Tap(Sprint)
	//Get input direction vector
	FVector2D CurrentInputDirection=MovementVector.GetSafeNormal();
	// Compare direction (95% match)
	if (FVector2D::DotProduct(LastMoveInputDirection, CurrentInputDirection)>0.95f&&bIsDoubleTab)
	{
		GetCharacterMovement()->MaxWalkSpeed=BalanceStats.MaxRunSpeed;
		ServerRPCSetMaxWalkSpeed(BalanceStats.MaxRunSpeed);
	}
	float CurrentTime=GetWorld()->GetTimeSeconds();	
	LastMoveInputTime=CurrentTime;
	LastMoveInputDirection = CurrentInputDirection;
	
	//Move Logic
	if (Controller != nullptr && CurrentCharacterState == ECharacterState::Normal)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		FVector MoveDirection = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
		// 이동 처리
		AddMovementInput(MoveDirection);
	}
}
void ABaseCharacter::StartSprint(const FInputActionValue& Value)
{
	float CurrentTime=GetWorld()->GetTimeSeconds();
	if (CurrentTime-LastMoveInputTime<=DoubleTapThreshold)
	{
		bIsDoubleTab=true;
	}
}

void ABaseCharacter::StopSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed=BalanceStats.MaxWalkSpeed;
	ServerRPCSetMaxWalkSpeed(BalanceStats.MaxWalkSpeed);
	bIsDoubleTab=false;
}


void ABaseCharacter::StartJump(const FInputActionValue& Value)
{
	// Jump 함수는 Character가 기본 제공
	if (Value.Get<bool>() && CurrentCharacterState == ECharacterState::Normal)
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

float ABaseCharacter::TakeNormalDamage(float Damage, float MinimumDamage)
{
	float ScaledDamage = BattleComponent->ComboStaleDamage(Damage, MinimumDamage);
	float NewHealth = FMath::Clamp(StatusComponent->GetCurrentHP() - ScaledDamage * BalanceStats.DamageTakenModifier, 0.0f, StatusComponent->GetMaxHP());
	StatusComponent->SetCurrentHP(NewHealth);
	UE_LOG(LogTemp,Warning,TEXT("TakeDamage: %.1f"),ScaledDamage);
	ModifySuperMeter(BattleComponent->GetMeterGainFromDamageTaken(ScaledDamage));

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
	BattleComponent->IncreaseCombo();

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

void ABaseCharacter::TakeHitlagAndStoredKnockback(int32 Hitlag, FVector KnockbackAngle, float KnockbackForce)
{
	if (Hitlag == 0)
	{
		TakeKnockback(KnockbackAngle, KnockbackForce);
		return;
	}
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		if (Movement->MovementMode != MOVE_None)
		{
			Movement->DisableMovement();
		}
	}
	StoredKnockbackAngle = KnockbackAngle;
	StoredKnockbackForce = KnockbackForce;

	GetMesh()->bPauseAnims = true;

	GetWorldTimerManager().ClearTimer(HitlagTimerHandle);
	GetWorldTimerManager().SetTimer(HitlagTimerHandle, this, &ABaseCharacter::EndHitlagAndTakeKnocback, Hitlag / 60.0f, false);
}

void ABaseCharacter::EndHitlagAndTakeKnocback()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(MOVE_Walking);
	}
	TakeKnockback(StoredKnockbackAngle, StoredKnockbackForce);
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
	
	FVector KnockbackVelocity = BattleComponent->KnockbackDir(KnockbackAngle, KnockbackForce, CurrentMoveInput, BalanceStats.DiModifier);
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
		VictimHitlag = FMath::RoundToInt(VictimHitlag * BattleComponent->Modifiers.CounterHitlagModifier);
		Hitstun = FMath::RoundToInt(Hitstun * BattleComponent->Modifiers.CounterHitlagModifier);
		Damage *= BattleComponent->Modifiers.CounterDamageModifier;
		//카운터 사운드
	}

	CurrentResistanceState = EResistanceState::Normal;
	TakeHitstun(Hitstun);
	TakeHitlagAndStoredKnockback(VictimHitlag, HitData.GetWorldKnockbackDirection(Attacker), HitData.KnockbackForce);
	float DealtDamage = TakeNormalDamage(Damage, HitData.MinimumDamage);

	Attacker->OnAttackHit(DealtDamage);
	return;
}

void ABaseCharacter::ReceiveArmorHit(ABaseCharacter* Attacker, FHitBoxData& HitData)
{
	float ArmorDamage = HitData.Damage * BattleComponent->Modifiers.ArmorDamageModifier;
	TakeHitlag(BattleComponent->Modifiers.ArmorHitlag);
	TakeNormalDamage(ArmorDamage, 0.0f);
	//아머 이펙트
	//아머 사운드
	Attacker->OnAttackHit(ArmorDamage);
	return;
}

void ABaseCharacter::ReceiveBlock(ABaseCharacter* Attacker, FHitBoxData& HitData)
{
	TakeKnockback(HitData.GetWorldKnockbackDirection(Attacker), HitData.GuardPushback);
	TakeBlockstun(HitData.Blockstun);
	TakeNormalDamage(HitData.GuardDamage, 0.0f);
	ModifyGuardMeter(-HitData.GuardMeterDamage);
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

void ABaseCharacter::Clash(ABaseCharacter* Attacker, FHitBoxData& HitData)
{
	TakeHitlagAndStoredKnockback(HitBoxList[CurrentActivatedCollision].Hitlag, HitData.GetWorldKnockbackDirection(Attacker), HitData.KnockbackForce);
}

void ABaseCharacter::OnDeath()
{
	// 사망 애니메이션 재생, 입력 차단, 리스폰 타이머 등
	
	if (!HasAuthority()) { return; }

	AController* MyController = GetController();
	HandlePlayerStateOnDeath();
	bool bRespawn = CanRespawn();
	HandleControllerOnDeath(bRespawn);

	AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GetWorld()->GetAuthGameMode());
	if (bRespawn && ArenaGameMode && MyController)
	{
		FTimerHandle RespawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, [ArenaGameMode, MyController]()
			{
				if (ArenaGameMode && MyController)
				{
					if (ACharacterController* CC = Cast<ACharacterController>(MyController))
					{
						ArenaGameMode->RespawnPlayer(CC);
					}
				}
			}, 1.5f, false);
	}

	Destroy();
}

void ABaseCharacter::SwitchToSpectatorCamera()
{
	if (ACharacterController* CC = Cast<ACharacterController>(GetController()))
	{
		AArenaGameMode* GM = Cast<AArenaGameMode>(GetWorld()->GetAuthGameMode());
		if (GM && GM->SpectatorCamera)
		{
			CC->SetViewTargetWithBlend(GM->SpectatorCamera, 0.f);
			CC->ChangeState(NAME_Spectating);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SwitchToSpectatorCamera: SpectatorCamera is null"));
		}
	}
}

void ABaseCharacter::ModifyGuardMeter(float Amount)
{
	float NewBlockMeter=StatusComponent->GetBlockMeter() + Amount;
	StatusComponent->SetBlockMeter(NewBlockMeter);
}

void ABaseCharacter::ModifySuperMeter(float Amount)
{
	float NewSuperMeter = StatusComponent->GetSuperMeter()+Amount;
	StatusComponent->SetSuperMeter(NewSuperMeter);
}

void ABaseCharacter::GainBurstMeter(float Amount)
{
	float NewBurstMeter = StatusComponent->GetBurstMeter()+Amount;
	StatusComponent->SetBurstMeter(NewBurstMeter);
}

void ABaseCharacter::PreLoadCharacterStats()
{
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			Stats=Loader->InitializeStat(FName("0"));
		}
	}
}

void ABaseCharacter::PreLoadCharacterBalanceStats()
{
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			BalanceStats=Loader->InitializeBalanceStat(FName(CharacterType));
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

void ABaseCharacter::PreLoadBattleModifiers()
{
	if (UGameInstance* GameInstance=GetGameInstance())
	{
		if (UDataLoaderSubSystem* Loader=GameInstance->GetSubsystem<UDataLoaderSubSystem>())
		{
			BattleComponent->Modifiers=Loader->InitializeBattleModifiers(FName("0"));
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
				AttackCollisions.SetNum(n-2);
				HitBoxList.SetNum(n-2);
				for (int32 i=0;i<n-1;i++)
				{
					FString TypeName=Type->GetNameStringByIndex(i);
					//UE_LOG(LogTemp,Warning,TEXT("Current RowName: %s, Current Index: %d"),*TypeName,i);
					const FName RowName=FName(CharacterType+"_"+TypeName);
					FAttackCollisionData Data=Loader->InitializeAttackCollisionData(RowName);
					FHitBoxData HitBoxData=Loader->InitializeHitBoxData(RowName);
					if (Data.Scale!=FVector::ZeroVector)
					{
						//Create Collision and Attacth to mesh
						UBoxComponent* AttackCollision = NewObject<UBoxComponent>(this,UBoxComponent::StaticClass());
						AttackCollision->SetupAttachment(GetMesh());
						AttackCollision->SetRelativeLocation(Data.Location);
						AttackCollision->SetRelativeRotation(Data.Rotation);
						AttackCollision->SetBoxExtent(Data.Scale*50.f,false);
						
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
			EnhancedInputComponent->BindAction(MyController->MoveAction, ETriggerEvent::Started, this, &ABaseCharacter::StartSprint);
			EnhancedInputComponent->BindAction(MyController->MoveAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprint);
		
			// Jumping
			EnhancedInputComponent->BindAction(MyController->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(MyController->JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			//Attack Actions
			EnhancedInputComponent->BindAction(MyController->AttackAction[0],ETriggerEvent::Triggered,this,&ABaseCharacter::Attack1);
			EnhancedInputComponent->BindAction(MyController->AttackAction[1],ETriggerEvent::Triggered,this,&ABaseCharacter::Attack2);
			EnhancedInputComponent->BindAction(MyController->AttackAction[3],ETriggerEvent::Triggered,this,&ABaseCharacter::Attack4);
			EnhancedInputComponent->BindAction(MyController->AttackAction[4],ETriggerEvent::Triggered,this,&ABaseCharacter::Attack5);
			EnhancedInputComponent->BindAction(MyController->AttackAction[6],ETriggerEvent::Triggered,this,&ABaseCharacter::Attack7);
			EnhancedInputComponent->BindAction(MyController->AttackAction[7],ETriggerEvent::Triggered,this,&ABaseCharacter::Attack8);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
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

void ABaseCharacter::HandlePlayerStateOnDeath()
{
	AArenaPlayerState* ArenaPlayerState = GetPlayerState<AArenaPlayerState>();
	AArenaGameState* ArenaGameState = Cast<AArenaGameState>(GetWorld()->GetGameState());

	if (ArenaPlayerState && ArenaGameState)
	{
		if (ArenaPlayerState->MaxLives <= 0)
		{
			float SurvivalTime = ArenaGameState->GetRoundStartTime() - ArenaGameState->GetRemainingTime();
			ArenaPlayerState->SetSurvivalTime(SurvivalTime);
			UE_LOG(LogTemp, Log, TEXT("Player Die.. SurvivalTime si %.2f"), ArenaGameState->GetRoundStartTime());
		}
		else
		{
			ArenaPlayerState->MaxLives -= 1;
			UE_LOG(LogTemp, Log, TEXT("Player Die.. Lives %d"), ArenaPlayerState->MaxLives);
		}
	}
}

bool ABaseCharacter::CanRespawn() const
{
	const AArenaPlayerState* ArenaPlayerState = GetPlayerState<AArenaPlayerState>();
	return (ArenaPlayerState && ArenaPlayerState->MaxLives > 0);
}

void ABaseCharacter::DeactivatePawnCamera()
{
	if (FollowCamera)
	{
		FollowCamera->Deactivate();
		FollowCamera->SetActive(false);
	}
	if (CameraBoom)
	{
		CameraBoom->Deactivate();
	}
}

void ABaseCharacter::TransitionToSpectator(ACharacterController* CC)
{
	AArenaGameMode* GM = Cast<AArenaGameMode>(GetWorld()->GetAuthGameMode());
	if (GM && GM->SpectatorCamera)
	{
		CC->SetViewTargetWithBlend(GM->SpectatorCamera, 0.f);
		CC->ChangeState(NAME_Spectating);

		FTimerHandle ForceSwitchTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ForceSwitchTimerHandle, [this, CC]()
			{
				AArenaGameMode* LocalGM = Cast<AArenaGameMode>(GetWorld()->GetAuthGameMode());
				if (LocalGM && LocalGM->SpectatorCamera)
				{
					CC->SetViewTargetWithBlend(LocalGM->SpectatorCamera, 0.f);
					CC->ChangeState(NAME_Spectating);
				}
			}, 0.1f, false);
	}
}

void ABaseCharacter::HandleControllerOnDeath(bool bRespawn)
{
	if (ACharacterController* CC = Cast<ACharacterController>(GetController()))
	{
		DisableInput(CC);
		CC->UnPossess();
		DeactivatePawnCamera();

		if (!bRespawn)
		{
			TransitionToSpectator(CC);
		}
	}
}

void ABaseCharacter::OnPlayerOverlapRiver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsDying) { return; }

	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("River")))
		{
			UE_LOG(LogTemp, Log, TEXT("River collision (River overlap) detected with: %s"), *OtherActor->GetName());
			bIsDying = true;
			OnDeath();
		}
	}
}
