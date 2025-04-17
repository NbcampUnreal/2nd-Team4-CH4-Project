#include "Items/Class/HealthItem.h"
#include "Character/Base/BaseCharacter.h"
#include "Items/Component/ItemInteractionComponent.h"

AHealthItem::AHealthItem()
{
	PrimaryActorTick.bCanEverTick = false;
	ItemType = EItemType::Healing;
	HealPercentage = 0.3f; // Default value, can be overridden in the editor
}

void AHealthItem::Interact(AActor* Activator)
{
	Super::Interact(Activator);

	UE_LOG(LogTemp, Warning, TEXT("HealthItem Interacted On Server"));
	ABaseCharacter* Character = Cast<ABaseCharacter>(Activator);
	if (Character)
	{
		UItemInteractionComponent* ItemInteractionComponent = Character->FindComponentByClass<UItemInteractionComponent>();
		if (ItemInteractionComponent)
		{
			ItemInteractionComponent->HandleHealEffect(0.3f);
		}
	}
}
