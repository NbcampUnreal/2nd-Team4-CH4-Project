#include "Items/Class/SpeedItem.h"
#include "Character/Base/BaseCharacter.h"
#include "Items/Component/ItemInteractionComponent.h"

ASpeedItem::ASpeedItem()
{
	PrimaryActorTick.bCanEverTick = false;
	EffectDuration = 5.0f;
	SpeedMultiplier = 1.5f;
}

void ASpeedItem::Interact(AActor* Activator)
{
	Super::Interact(Activator);

	UE_LOG(LogTemp, Warning, TEXT("SpeedItem Interacted On Server"));
	ABaseCharacter* Character = Cast<ABaseCharacter>(Activator);
	if (Character)
	{
		UItemInteractionComponent* ItemInteractionComponent = Character->FindComponentByClass<UItemInteractionComponent>();
		if (ItemInteractionComponent)
		{
			ItemInteractionComponent->HandleSpeedModifier(SpeedMultiplier, EffectDuration);
		}
	}
}