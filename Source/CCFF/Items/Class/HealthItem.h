#pragma once

#include "CoreMinimal.h"
#include "Items/Class/SpawnableItem.h"
#include "HealthItem.generated.h"

UCLASS()
class CCFF_API AHealthItem : public ASpawnableItem
{
	GENERATED_BODY()
public: 
	AHealthItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	float HealPercentage;

protected:
	virtual void Interact(AActor* Activator) override;
};
