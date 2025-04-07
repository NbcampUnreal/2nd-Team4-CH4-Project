#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemInteractionComponent.generated.h"

class ASpawnableItem;
class UItemManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CCFF_API UItemInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemInteractionComponent();

	UFUNCTION(Server, Reliable)
	void Server_InteractItem(ASpawnableItem* Item);
	void Server_InteractItem_Implementation(ASpawnableItem* Item);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayInteractionEffects(ASpawnableItem* Item);
	void Multicast_PlayInteractionEffects_Implementation(ASpawnableItem* Item);

	void HandleInteractionEffects(ASpawnableItem* Item);

	UItemManager* ItemManager;

protected:
	virtual void BeginPlay() override;
};
