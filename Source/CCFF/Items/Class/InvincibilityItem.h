#pragma once

#include "CoreMinimal.h"
#include "Items/Class/SpawnableItem.h"
#include "InvincibilityItem.generated.h"

UCLASS()
class CCFF_API AInvincibilityItem : public ASpawnableItem
{
	GENERATED_BODY()
	
public:
	AInvincibilityItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	float EffectDuration;

protected:
	virtual void Interact(AActor* Activator) override;

};
