#include "Items/Class/InvincibilityItem.h"
#include "Character/Base/BaseCharacter.h"
#include "Character/Base/CharacterEnum.h"
#include "Items/Component/ItemInteractionComponent.h"

AInvincibilityItem::AInvincibilityItem()
{
	ItemType = EItemType::Resistivity;
	EffectDuration = 5.0f; // Default value, can be overridden in the editor
}

void AInvincibilityItem::Interact(AActor* Activator)
{
	Super::Interact(Activator);

	UE_LOG(LogTemp, Warning, TEXT("InvincibilityItem Interacted On Server"));
	ABaseCharacter* Character = Cast<ABaseCharacter>(Activator);
	if (Character)
	{
		UItemInteractionComponent* ItemInteractionComponent = Character->FindComponentByClass<UItemInteractionComponent>();
		if (ItemInteractionComponent)
		{
			ItemInteractionComponent->HandleResistivityModifier(EResistanceState::HyperArmor, EffectDuration);
		}
	}
}