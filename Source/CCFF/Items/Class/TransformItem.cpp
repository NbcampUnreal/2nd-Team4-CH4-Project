#include "Items/Class/TransformItem.h"
#include "Character/Base/BaseCharacter.h"
#include "Items/Component/ItemInteractionComponent.h"

ATransformItem::ATransformItem()
{
	PrimaryActorTick.bCanEverTick = false;
	EffectDuration = 10.0f;
	ShrinkScale = 0.4f;
	EnlargeScale = 2.0f;
}

void ATransformItem::Interact(AActor* Activator)
{
	Super::Interact(Activator);
	UE_LOG(LogTemp, Warning, TEXT("TransformItem Interacted On Server"));
	ABaseCharacter* Character = Cast<ABaseCharacter>(Activator);
	if (Character)
	{
		UItemInteractionComponent* ItemInteractionComponent = Character->FindComponentByClass<UItemInteractionComponent>();
		if (ItemInteractionComponent)
		{
			ETransformType TransformType = GetRandomTransformType();
			switch (TransformType)
			{
			case ETransformType::Shrink:
				ItemInteractionComponent->HandleMeshShrink(ShrinkScale, EffectDuration);
				UE_LOG(LogTemp, Warning, TEXT("TransformItem Shrink Effect Applied"));
				break;
			case ETransformType::Enlarge:
				ItemInteractionComponent->HandleMeshEnlarge(EnlargeScale, EffectDuration);
				UE_LOG(LogTemp, Warning, TEXT("TransformItem Enlarge Effect Applied"));
				break;
			default:
				break;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemInteractionComponent not found on %s"), *Character->GetName());
		}
	}
}

ETransformType ATransformItem::GetRandomTransformType()
{
	TArray<ETransformType> TransformTypes = { ETransformType::Shrink, ETransformType::Enlarge };
	int32 RandomIndex = FMath::RandRange(0, TransformTypes.Num() - 1);
	return TransformTypes[RandomIndex];
}