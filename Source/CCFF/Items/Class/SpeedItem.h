#pragma once

#include "CoreMinimal.h"
#include "Items/Class/SpawnableItem.h"
#include "SpeedItem.generated.h"

UCLASS()
class CCFF_API ASpeedItem : public ASpawnableItem
{
	GENERATED_BODY()
	
public:
	ASpeedItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	float EffectDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	float SpeedMultiplier;

protected:
	virtual void Interact(AActor* Activator) override;

};
