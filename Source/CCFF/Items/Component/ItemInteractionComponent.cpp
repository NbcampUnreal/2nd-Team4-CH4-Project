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
		// Set the Resistance State
		Character->SetResistanceState(ResistanceState);

		// Lamda: Set Timer to revert the Resistance state after Duration
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([Character]()
			{
				UE_LOG(LogTemp, Warning, TEXT("Reverting ResistanceState to Normal"));
				Character->SetResistanceState(EResistanceState::Normal);
			});

		// If the timer is already active, clear it before setting a new one
		if (GetWorld()->GetTimerManager().IsTimerActive(ResistivityResetTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(ResistivityResetTimer);
		}

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

			MovementComponent->MaxWalkSpeed *= SpeedMultiplier;

			FTimerDelegate TimerDel;
			TimerDel.BindLambda([Character, MovementComponent, SpeedMultiplier]()
				{
					UE_LOG(LogTemp, Warning, TEXT("Reverting SpeedMultiplier to 1.0"));
					MovementComponent->MaxWalkSpeed /= SpeedMultiplier;
				});

			GetWorld()->GetTimerManager().SetTimer(ResistivityResetTimer, TimerDel, Duration, false);
		}
	}
}