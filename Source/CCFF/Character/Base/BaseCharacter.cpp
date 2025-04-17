// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "ClearReplacementShaders.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MaterialHLSLTree.h"
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
#include "Framework/HUD/ArenaModeHUD.h"
#include "Framework/PlayerState/ArenaPlayerState.h"
#include "Framework/UI/BaseInGameWidget.h"
#include "Framework/UI/Character/UW_HPWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Component/ItemInteractionComponent.h"
#include "Net/UnrealNetwork.h"

class AArenaModeHUD;
// Sets default values
ABaseCharacter::ABaseCharacter():
	CurrentActivatedCollision(-1),
	BufferThreshold(0.5f),
	//bCanAttack(true),
	LastAttackStartTime(0.f),
	ServerDelay(0.f),
	LastMoveInputTime(0.f),
	DoubleTapThreshold(0.3f),
	bIsDoubleTab(false),
	LastDamageCauser(nullptr)
{
	//Set Tick
	PrimaryActorTick.bCanEverTick = true;
	
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
	GetCharacterMovement()->JumpZVelocity = 500.f;
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
		StatusComponent->OnCurrentHPChanged.AddUObject(MyHUD,&UBaseInGameWidget::UpdateHealthBar);
		StatusComponent->OnSuperMeterChanged.AddUObject(MyHUD,&UBaseInGameWidget::UpdateSuperMeterBar);
		StatusComponent->OnBurstMeterChanged.AddUObject(MyHUD,&UBaseInGameWidget::UpdateBurstMeterBar);
		StatusComponent->OnStockCountChanged.AddUObject(MyHUD,&UBaseInGameWidget::UpdateStockCount);
		UE_LOG(LogTemp,Display,TEXT("CharacterController SetHud Call"));
		MyHUD->InitializeHUDWidget(StatusComponent);
		UE_LOG(LogTemp,Display,TEXT("[SetHud] UpdateStockCount Called"));
		UpdateStockCount();
	}
}

void ABaseCharacter::UpdateStockCount()
{
	if (AArenaPlayerState* ArenaPS=Cast<AArenaPlayerState>(GetPlayerState()))
	{
		UE_LOG(LogTemp,Warning,TEXT("[UpdateStockCount] SetCurrentStockCount UpdateStockCount Call (LocallyControlled: %d, HasAuthority: %d"),IsLocallyControlled(),HasAuthority());
		StatusComponent->SetCurrentStockCount(ArenaPS->MaxLives);
	}
	else
	{
		UE_LOG(LogTemp,Display,TEXT("[UpdateStockCount] PlayerState is NULL"));
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
	//Bind Widget Update function to Delegate
	if (ACharacterController* CC = Cast<ACharacterController>(GetController()))
	{
		//UE_LOG(LogTemp,Error,TEXT("ABaseCharacter::BeginPlay Called"));
		if (CC->IsLocalController())
		{
			//UE_LOG(LogTemp,Error,TEXT("ABaseCharacter::BeginPlay Controller Is Valid"));
			if (AArenaModeHUD* AM = Cast<AArenaModeHUD>(CC->GetHUD()))
			{
				//UE_LOG(LogTemp,Error,TEXT("ABaseCharacter::SetHUD Called"));
				SetHUDWidget(AM->GetBaseInGameWidget());
			}
		}
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Burst Guage charge
	if (HasAuthority())
	{
		const float ChargePerSecond=500.f;
		const float ChargeAmount=ChargePerSecond*DeltaTime;
		StatusComponent->AddBlockMeter(ChargeAmount);
		StatusComponent->AddBurstMeter(ChargeAmount);
	}
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
	LastDamageCauser=DamageCauser;
	ABaseCharacter* Attacker = Cast<ABaseCharacter>(DamageCauser);
	ProcessHitReaction(Attacker, HitData);
	
	return ActualDamage;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,CurrentCharacterState);
	DOREPLIFETIME(ThisClass,CurrentResistanceState);
	//DOREPLIFETIME(ThisClass,bCanAttack);
}

void ABaseCharacter::AttackNotify(const FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	// Determin the type of attack by name
	FString NotifyNameString = NotifyName.ToString();
	// Parsing number
	int32 Index = NotifyNameString.Len() - 1;
	FString NumberStr;
	// 끝에서부터 숫자 추출
	while (Index >= 0 && FChar::IsDigit(NotifyNameString[Index]))
	{
		NumberStr.InsertAt(0, NotifyNameString[Index]);
		Index--;
	}
	int32 AttackNumber = FCString::Atoi(*NumberStr) - 1;
	if (!AttackCollisions.IsValidIndex(AttackNumber)||!AttackCollisions[AttackNumber]) return;
	if (!HasAuthority())
	{
		//Client logic
		// Particle, Effect
		if (AttackNumber>=0&&AttackNumber<16)
		{
			DrawDebugBox(GetWorld(),AttackCollisions[AttackNumber]->GetComponentLocation(),AttackCollisions[AttackNumber]->GetScaledBoxExtent(),AttackCollisions[AttackNumber]->GetComponentQuat(),FColor::Red,false,2.0f);
		}
		return;
	}

	if (!NotifyName.IsValid()||AttackCollisions.IsEmpty()) return;
	// Hitbox 
	if(NotifyNameString.Contains(TEXT("Hitbox")))
	{
		// Activate Collision in proper location
		if (AttackNumber>=0&&AttackNumber<16)
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
				CurrentActivatedCollision = AttackNumber;
				AttackCollisions[AttackNumber]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				TArray<AActor*> OverlappingActors;
				AttackCollisions[AttackNumber]->GetOverlappingActors(OverlappingActors);
				for (AActor* OtherActor : OverlappingActors)
				{
					if (OtherActor != this)
					{
						OnAttackOverlap(AttackCollisions[AttackNumber], OtherActor, nullptr, 0, false, FHitResult());
					}
				}
				UE_LOG(LogTemp,Warning,TEXT("Activate Collision! (Index: %d)"),AttackNumber);
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Activate Failed!"));
		}
		return;
	}
	if (NotifyNameString.Contains(TEXT("Invulnerable")))
	{
		if (NotifyNameString.Contains(TEXT("End")))
		{
			CurrentResistanceState=EResistanceState::Normal;
		}
		else
		{
			CurrentResistanceState=EResistanceState::Invulnerable;
		}
	}
	if (NotifyNameString.Contains(TEXT("HyperArmor")))
	{
		if (NotifyNameString.Contains(TEXT("End")))
		{
			CurrentResistanceState=EResistanceState::Normal;
		}
		else
		{
			CurrentResistanceState=EResistanceState::HyperArmor;
		}
	}
	if (NotifyNameString.Contains(TEXT("ProjectileInvulnerable")))
	{
		if (NotifyNameString.Contains(TEXT("End")))
		{
			CurrentResistanceState=EResistanceState::Normal;
		}
		else
		{
			CurrentResistanceState=EResistanceState::ProjectileInvulnerable;
		}
	}
}

void ABaseCharacter::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	// Server Logic
	// Except self
	if (!OtherActor || OtherActor == this || CurrentActivatedCollision==-1) return;
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
	//UE_LOG(LogTemp, Warning, TEXT("Montage %s Ended. Interrupted: %d"), *Montage->GetName(), bInterrupted);
	//CurrentCharacterState = ECharacterState::Normal;
}

void ABaseCharacter::DeactivateAttackCollision(const int32 Index) const
{
	if (Index>=0&&Index<14) return;
	AttackCollisions[Index]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ABaseCharacter::ServerRPCAction_Implementation(ECharacterState InState, float InStartAttackTime, const int32 Num)
{
	UE_LOG(LogTemp,Warning,TEXT("ServerRPCAction Called with %d !!"),Num);
	float MontagePlayTime=0.f;
	ECharacterState TempState=ECharacterState::Normal;
	
	switch (InState)
	{
	case ECharacterState::Attack:
		MontagePlayTime=Anim.AttackMontage[Num]->GetPlayLength();
		TempState=ECharacterState::Attack;
		break;
	case ECharacterState::Dodge:
		MontagePlayTime=Anim.DodgeMontage->GetPlayLength();
		TempState=ECharacterState::Dodge;
		break;
	case ECharacterState::Block:
		TempState=ECharacterState::Block;
	default:
		break;
	}
	if (Num==15) //Burst
	{
		StatusComponent->AddBurstMeter(-2500.f);
	}
	ServerDelay=GetWorld()->GetTimeSeconds()-InStartAttackTime;
	// 0<=ServerDelay<=MontagePlayTime
	ServerDelay=FMath::Clamp(ServerDelay,0.f,MontagePlayTime);
	PrevMontagePlayTime=MontagePlayTime;
	// Consider ServerDelay Timer (Can Attack)
	CurrentCharacterState=TempState;
	OnRep_CurrentCharacterState();
	if (KINDA_SMALL_NUMBER<MontagePlayTime-ServerDelay&&TempState!=ECharacterState::Block)
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle,FTimerDelegate::CreateLambda([&]()
		{
			CurrentCharacterState=ECharacterState::Normal;
			OnRep_CurrentCharacterState();
		}),
		MontagePlayTime-ServerDelay,false,-1.f);
	}
	LastAttackStartTime=InStartAttackTime;
	PlayActionMontage(TempState,Num);
	
	for (APlayerController* PC:TActorRange<APlayerController>(GetWorld()))
	{
		if (IsValid(PC)==true&&GetController()!=PC) // Find Other Client
		{
			if (ABaseCharacter* OtherClientCharacter=Cast<ABaseCharacter>(PC->GetPawn()))
			{
				OtherClientCharacter->ClientRPCPlayActionMontage(TempState,Num,this);
			}
		}
	}
}

bool ABaseCharacter::ServerRPCAction_Validate(ECharacterState InState, float InStartAttackTime, const int32 Num)
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
void ABaseCharacter::ClientRPCPlayActionMontage_Implementation(ECharacterState InState, const int32 Num, ABaseCharacter* InTargetCharacter)
{
	if (IsValid(InTargetCharacter)==true)
	{
		InTargetCharacter->PlayActionMontage(InState,Num);
	}
}

void ABaseCharacter::ExecuteBufferedAction()
{
	const float CurrentTime=GetWorld()->GetTimeSeconds();
	const int32 InputAction=static_cast<int32>(InputBuffer.InputAttack);
	//Execute Buffered Input Action
	if (InputAction>=0&&InputAction<15&&(CurrentTime-InputBuffer.BufferedTime<=BufferThreshold))
	{
		ExecuteActionByIndex(InputBuffer.InputState,InputAction);
		UE_LOG(LogTemp,Warning,TEXT("Execute Buffered Input(Index: %d)"),InputAction);
	}
	// Clear Buffer
	InputBuffer=FBufferedInput();
}

void ABaseCharacter::OnRep_CurrentCharacterState()
{
	switch (CurrentCharacterState)
	{
	case ECharacterState::Normal:
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		ExecuteBufferedAction();
		break;
	case ECharacterState::Hitted:
		// GetCharacterMovement()->SetMovementMode(MOVE_None);
		PlayActionMontage(ECharacterState::Hitted,0);
		break;
	case ECharacterState::Dead:
		// GetCharacterMovement()->SetMovementMode(MOVE_None);
		PlayActionMontage(ECharacterState::Dead,0);
		BattleComponent->ResetCombo();
	default:
		// GetCharacterMovement()->SetMovementMode(MOVE_None);
		break;
	}
}

// void ABaseCharacter::OnRep_CurrentResistanceState()
// {
// 	
// }

void ABaseCharacter::ExecuteActionByIndex(ECharacterState InState, const int32 Index)
{
	if ((CurrentCharacterState==ECharacterState::Normal&&GetCharacterMovement()->IsFalling()==false)||
		(CurrentCharacterState==ECharacterState::Normal&&GetCharacterMovement()->IsFalling()&&Index>7&&Index<14)||
		(CurrentCharacterState==ECharacterState::Hitted&&Index==15)||
		(InState==ECharacterState::Normal))
	{
		if (Index==15&&StatusComponent->GetBurstMeter()>=2500)
		{
			StatusComponent->AddBurstMeter(-2500);
		}
		//UE_LOG(LogTemp,Warning,TEXT("Attack1 Called !!"));
		ServerRPCAction(InState,GetWorld()->GetGameState()->GetServerWorldTimeSeconds(), Index);
		// Play Montage in Owning Client
		if (HasAuthority()==false&&IsLocallyControlled()==true)
		{
			CurrentCharacterState=InState;
			// bCanAttack=false;
			OnRep_CurrentCharacterState();
			PlayActionMontage(InState,Index);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Buffered Action Type: %d, Buffered Input Index: %d"),InState,Index);
		InputBuffer=FBufferedInput(InState, static_cast<EAttackType>(Index),GetWorld()->GetTimeSeconds());
	}
}

void ABaseCharacter::Attack1(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		ExecuteActionByIndex(ECharacterState::Attack,8);
	}
	else
	{
		ExecuteActionByIndex(ECharacterState::Attack,0);	
	}
}
void ABaseCharacter::Attack2(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		ExecuteActionByIndex(ECharacterState::Attack,9);
	}
	else if (GetCharacterMovement()->MaxWalkSpeed==BalanceStats.MaxRunSpeed)
	{
		ExecuteActionByIndex(ECharacterState::Attack,2);
	}
	else
	{
		ExecuteActionByIndex(ECharacterState::Attack,1);
	}
}


void ABaseCharacter::Attack4(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		ExecuteActionByIndex(ECharacterState::Attack,10);
	}
	else
	{
		ExecuteActionByIndex(ECharacterState::Attack,3);
	}
}

void ABaseCharacter::Attack5(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		ExecuteActionByIndex(ECharacterState::Attack,11);
	}
	else if (GetCharacterMovement()->MaxWalkSpeed==BalanceStats.MaxRunSpeed)
	{
		ExecuteActionByIndex(ECharacterState::Attack,5);
	}
	else
	{
		ExecuteActionByIndex(ECharacterState::Attack,4);
	}
}

void ABaseCharacter::Attack7(const FInputActionValue& Value)
{
	if (CurrentCharacterState==ECharacterState::Hitted)
	{
		ExecuteActionByIndex(ECharacterState::Attack,15);
	}
	else if (GetCharacterMovement()->IsFalling())
	{
		ExecuteActionByIndex(ECharacterState::Attack,12);
	}
	else
	{
		ExecuteActionByIndex(ECharacterState::Attack,6);
	}
}

void ABaseCharacter::Attack8(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		ExecuteActionByIndex(ECharacterState::Attack,13);
	}
	else
	{
		ExecuteActionByIndex(ECharacterState::Attack,7);
	}
}

void ABaseCharacter::Guard()
{
	UE_LOG(LogTemp,Warning,TEXT("Guard Start"));
	if (CurrentCharacterState!=ECharacterState::Block)
	{
		ExecuteActionByIndex(ECharacterState::Block,0);	
	}
}

void ABaseCharacter::StopGuard()
{
	UE_LOG(LogTemp,Warning,TEXT("Guard Stopped"));
	if (CurrentCharacterState==ECharacterState::Block)
	{
		ExecuteActionByIndex(ECharacterState::Normal,0);
	}
}

void ABaseCharacter::Dodge()
{
	ExecuteActionByIndex(ECharacterState::Dodge,0);
}

void ABaseCharacter::Invincible()
{
	ExecuteActionByIndex(ECharacterState::Attack,14);
}

void ABaseCharacter::PlayActionMontage(ECharacterState InState, const int32 Num)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimMontage* PlayMontage=nullptr;
	switch (InState)
	{
	case ECharacterState::Attack:
		PlayMontage=Anim.AttackMontage[Num];
		break;
	case ECharacterState::Block:
		PlayMontage=Anim.GuardMontage;
		break;
	case ECharacterState::Dodge:
		PlayMontage=Anim.DodgeMontage;
		break;
	case ECharacterState::Dead:
		PlayMontage=Anim.DeathMontage;
		break;
	case ECharacterState::Hitted:
		PlayMontage=Anim.HittedMontage;
		break;
	default:
		break;
	}
	//둘다 유효
	if (AnimInstance&&PlayMontage)
	{
		UE_LOG(LogTemp,Warning,TEXT("CurState: %d, Character: %s, PlayMontage: %s"),CurrentCharacterState,*GetName(),*PlayMontage->GetName());
		AnimInstance->Montage_Stop(0.2f);
		//몽타주 실행
		AnimInstance->Montage_Play(PlayMontage);
	}
	else if (!PlayMontage&&InState==ECharacterState::Normal)
	{
		//UE_LOG(LogTemp,Warning,TEXT("Stop Montage"));
		//Stop montage
		AnimInstance->Montage_Stop(0.2f);
	}
	GetCharacterMovement()->MaxWalkSpeed=BalanceStats.MaxWalkSpeed;
	bIsDoubleTab=false;
	//UE_LOG(LogTemp,Warning,TEXT("CurWalkSpeed: %f, Character: %s"),GetCharacterMovement()->MaxWalkSpeed,*GetName());
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
	UE_LOG(LogTemp,Warning,TEXT("TakeDamage: %.1f"),ScaledDamage);
	StatusComponent->SetCurrentHP(NewHealth);
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
	TakeHitstun(60);
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

	OnRep_CurrentCharacterState();
	float DealtDamage = TakeNormalDamage(Damage, HitData.MinimumDamage);
	TakeHitstun(Hitstun);
	TakeHitlagAndStoredKnockback(VictimHitlag, HitData.GetWorldKnockbackDirection(Attacker), HitData.KnockbackForce);

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
	//UE_LOG(LogTemp,Warning,TEXT("Authority: %d, LocallyControlled: %d"),HasAuthority(),IsLocallyControlled());
	
	//Set off Collision and change state to dead
	CurrentCharacterState=ECharacterState::Dead;
	//Set off Overlap event
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	//Reset Combo Count
	BattleComponent->ResetCombo();
	//Raise KillCount of DamageCauser
	if (LastDamageCauser)
	{
		ACharacter* DamageCauserCharacter=Cast<ACharacter>(LastDamageCauser);
		AArenaPlayerState* DamageCauserPS=Cast<AArenaPlayerState>(DamageCauserCharacter->GetPlayerState());
		if (IsValid(DamageCauserPS))
		{
			const int32 CurrentKillCount=DamageCauserPS->GetKillCount();
			DamageCauserPS->SetKillCount(CurrentKillCount+1);
		}
	}
	UpdateStockCount();
	if (IsValid(Anim.DeathMontage))
	{
		//Montage end -> destroy actor
		float MontageLength=Anim.DeathMontage->GetPlayLength();
		FTimerHandle DestroyTimerHandle;
		GetWorldTimerManager().SetTimer(
			DestroyTimerHandle,
			[this](){
				Destroy();
				//Update StockCount
			},MontageLength,false);
	}

	if (ACharacterController* CharacterController = Cast<ACharacterController>(GetController()))
	{
		CharacterController->NotifyPawnDeath();
	}
}


void ABaseCharacter::ModifySuperMeter(float Amount)
{
	float NewSuperMeter = StatusComponent->GetSuperMeter()+Amount;
	StatusComponent->SetSuperMeter(NewSuperMeter);
}

void ABaseCharacter::ModifyGuardMeter(float Amount)
{
	StatusComponent->AddBlockMeter(Amount);
}

void ABaseCharacter::GainBurstMeter(float Amount)
{
	StatusComponent->AddBurstMeter(Amount);
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
				for (int32 i=0;i<n-2;i++)
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

			//Gurad, Dodge, Burst
			EnhancedInputComponent->BindAction(MyController->GuardAction,ETriggerEvent::Started,this,&ABaseCharacter::Guard);
			EnhancedInputComponent->BindAction(MyController->GuardAction,ETriggerEvent::Completed,this,&ABaseCharacter::StopGuard);
			EnhancedInputComponent->BindAction(MyController->DodgeAction,ETriggerEvent::Triggered,this,&ABaseCharacter::Dodge);
			EnhancedInputComponent->BindAction(MyController->BurstAction,ETriggerEvent::Triggered,this,&ABaseCharacter::Invincible);
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
			if (ACharacterController* CharacterController = Cast<ACharacterController>(GetController()))
			{
				CharacterController->NotifyPawnDeath();
				UpdateStockCount();
				Destroy();
			}
		}
	}
}
