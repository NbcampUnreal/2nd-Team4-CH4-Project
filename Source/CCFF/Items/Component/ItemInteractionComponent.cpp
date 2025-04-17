#include "Items/Component/ItemInteractionComponent.h"
#include "Items/Manager/ItemManager.h"
#include "Items/Class/SpawnableItem.h"
#include "Character/Base/BaseCharacter.h"
#include "Character/Components/StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UItemInteractionComponent::UItemInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UItemInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		if (UGameInstance* GameInstance = GetOwner()->GetGameInstance())
		{
			ItemManager = GameInstance->GetSubsystem<UItemManager>();
		}
	}

	if (!ItemManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemManager is nullptr in UItemInteractionComponent"));
	}
}

void UItemInteractionComponent::Server_InteractItem_Implementation(ASpawnableItem* Item)
{
    if (ItemManager->IsServer())
    {
        ItemManager->OnItemInteract(GetOwner(), this, Item); // 서버에서 상호작용 실행
    }
}

void UItemInteractionComponent::HandleInteractionEffects(ASpawnableItem* Item)
{
	if (Item)
	{
		Multicast_PlayInteractionEffects(Item); // 클라이언트에서 효과 재생
	}
}

void UItemInteractionComponent::Multicast_PlayInteractionEffects_Implementation(ASpawnableItem* Item)
{
	Item->OnInteract(); // 상호작용 효과 실행
}

void UItemInteractionComponent::HandleHealEffect(float HealPercent)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		UStatusComponent* StatusComp = Character->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float CurrentHP = StatusComp->GetCurrentHP();
			float MaxHP = StatusComp->GetMaxHP();
			float HealAmount = MaxHP * HealPercent;
			// Heal the character
			StatusComp->SetCurrentHP(FMath::Clamp(CurrentHP + HealAmount, 0.0f, MaxHP));
			UE_LOG(LogTemp, Warning, TEXT("Healed %s by %.2f%%"), *Character->GetName(), HealPercent * 100.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("StatusComponent not found on %s"), *GetOwner()->GetName());
		}
	}
}

void UItemInteractionComponent::HandleResistivityModifier(EResistanceState ResistanceState, float Duration)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleResistivityModifier called with %s for %.2f seconds"), *UEnum::GetValueAsString(ResistanceState), Duration);

	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		// 상태 설정
		Character->SetResistanceState(ResistanceState);

		// Weak 포인터로 캐릭터 안전 캡처
		TWeakObjectPtr<ABaseCharacter> WeakCharacter = Character;

		// 기존 타이머가 있다면 제거
		if (GetWorld()->GetTimerManager().IsTimerActive(ResistivityResetTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(ResistivityResetTimer);
		}

		// 람다: 일정 시간 후 상태 복구
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([WeakCharacter]()
			{
				if (WeakCharacter.IsValid())
				{
					WeakCharacter->SetResistanceState(EResistanceState::Normal);
					UE_LOG(LogTemp, Warning, TEXT("Reverting ResistanceState to Normal"));
				}
			});

		GetWorld()->GetTimerManager().SetTimer(ResistivityResetTimer, TimerDel, Duration, false);
	}
}


void UItemInteractionComponent::HandleSpeedModifier(float SpeedMultiplier, float Duration)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleSpeedModifier called with %.2f for %.2f seconds"), SpeedMultiplier, Duration);
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
		if (MovementComponent)
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(ResistivityResetTimer))
			{
				UE_LOG(LogTemp, Warning, TEXT("SpeedModifier timer is already active. Ignoring new request."));
				return;
			}

			float OriginalSpeed = MovementComponent->MaxWalkSpeed;
			MovementComponent->MaxWalkSpeed *= SpeedMultiplier;

			TWeakObjectPtr<ABaseCharacter> WeakCharacter = Character;

			FTimerDelegate TimerDel;
			TimerDel.BindLambda([WeakCharacter, OriginalSpeed]()
				{
					if (WeakCharacter.IsValid())
					{
						if (UCharacterMovementComponent* MoveComp = WeakCharacter->GetCharacterMovement())
						{
							MoveComp->MaxWalkSpeed = OriginalSpeed;
							UE_LOG(LogTemp, Warning, TEXT("Reverted MaxWalkSpeed to %.2f"), OriginalSpeed);
						}
					}
				});

			GetWorld()->GetTimerManager().SetTimer(ResistivityResetTimer, TimerDel, Duration, false);
		}
	}
}


void UItemInteractionComponent::HandleMeshShrink(float Scale, float Duration)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(TransformationTimer))
		{
			UE_LOG(LogTemp, Warning, TEXT("Transformation timer is already active. Ignoring new request."));
			return;
		}
		FVector OriginalScale = Character->GetActorScale3D();
		FVector NewScale = OriginalScale * Scale;
		if (Character->HasAuthority())  // 서버에서만 처리
		{
			// 서버에서 크기 변경
			Character->SetActorScale3D(NewScale);
			// 모든 클라이언트에 크기 동기화
			MulticastSetActorScale(NewScale);
			// 일정 시간 후 원래 크기로 되돌리기
			FTimerDelegate TimerDel;
			TimerDel.BindLambda([this, Character, OriginalScale]()
				{
					if (Character->HasAuthority())
					{
						Character->SetActorScale3D(OriginalScale);
						MulticastSetActorScale(OriginalScale); // 원래 크기로 동기화
						UE_LOG(LogTemp, Warning, TEXT("Mesh Shrink effect reverted for %s"), *Character->GetName());
					}
				});
			GetWorld()->GetTimerManager().SetTimer(TransformationTimer, TimerDel, Duration, false);
		}
	}
}


void UItemInteractionComponent::HandleMeshEnlarge(float Scale, float Duration)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(TransformationTimer))
		{
			UE_LOG(LogTemp, Warning, TEXT("Transformation timer is already active. Ignoring new request."));
			return;
		}

		FVector OriginalScale = Character->GetActorScale3D();
		FVector NewScale = OriginalScale * Scale;

		if (Character->HasAuthority())  // 서버에서만 처리
		{
			// 서버에서 크기 변경
			Character->SetActorScale3D(NewScale);

			// 모든 클라이언트에 크기 동기화
			MulticastSetActorScale(NewScale);

			// 일정 시간 후 원래 크기로 되돌리기
			FTimerDelegate TimerDel;
			TimerDel.BindLambda([this, Character, OriginalScale]()
				{
					if (Character->HasAuthority())
					{
						Character->SetActorScale3D(OriginalScale);
						MulticastSetActorScale(OriginalScale); // 원래 크기로 동기화
						UE_LOG(LogTemp, Warning, TEXT("Mesh Enlarge effect reverted for %s"), *Character->GetName());
					}
				});

			GetWorld()->GetTimerManager().SetTimer(TransformationTimer, TimerDel, Duration, false);
		}
	}
}

void UItemInteractionComponent::MulticastSetActorScale_Implementation(FVector NewScale)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		// 모든 클라이언트에 대해 크기 적용
		Character->SetActorScale3D(NewScale);
	}
}
