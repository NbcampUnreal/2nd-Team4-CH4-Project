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
        ItemManager->OnItemInteract(GetOwner(), this, Item); // �������� ��ȣ�ۿ� ����
    }
}

void UItemInteractionComponent::HandleInteractionEffects(ASpawnableItem* Item)
{
	if (Item)
	{
		Multicast_PlayInteractionEffects(Item); // Ŭ���̾�Ʈ���� ȿ�� ���
	}
}

void UItemInteractionComponent::Multicast_PlayInteractionEffects_Implementation(ASpawnableItem* Item)
{
	Item->OnInteract(); // ��ȣ�ۿ� ȿ�� ����
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
		// ���� ����
		Character->SetResistanceState(ResistanceState);

		// Weak �����ͷ� ĳ���� ���� ĸó
		TWeakObjectPtr<ABaseCharacter> WeakCharacter = Character;

		// ���� Ÿ�̸Ӱ� �ִٸ� ����
		if (GetWorld()->GetTimerManager().IsTimerActive(ResistivityResetTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(ResistivityResetTimer);
		}

		// ����: ���� �ð� �� ���� ����
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
		if (Character->HasAuthority())  // ���������� ó��
		{
			// �������� ũ�� ����
			Character->SetActorScale3D(NewScale);
			// ��� Ŭ���̾�Ʈ�� ũ�� ����ȭ
			MulticastSetActorScale(NewScale);
			// ���� �ð� �� ���� ũ��� �ǵ�����
			FTimerDelegate TimerDel;
			TimerDel.BindLambda([this, Character, OriginalScale]()
				{
					if (Character->HasAuthority())
					{
						Character->SetActorScale3D(OriginalScale);
						MulticastSetActorScale(OriginalScale); // ���� ũ��� ����ȭ
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

		if (Character->HasAuthority())  // ���������� ó��
		{
			// �������� ũ�� ����
			Character->SetActorScale3D(NewScale);

			// ��� Ŭ���̾�Ʈ�� ũ�� ����ȭ
			MulticastSetActorScale(NewScale);

			// ���� �ð� �� ���� ũ��� �ǵ�����
			FTimerDelegate TimerDel;
			TimerDel.BindLambda([this, Character, OriginalScale]()
				{
					if (Character->HasAuthority())
					{
						Character->SetActorScale3D(OriginalScale);
						MulticastSetActorScale(OriginalScale); // ���� ũ��� ����ȭ
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
		// ��� Ŭ���̾�Ʈ�� ���� ũ�� ����
		Character->SetActorScale3D(NewScale);
	}
}
