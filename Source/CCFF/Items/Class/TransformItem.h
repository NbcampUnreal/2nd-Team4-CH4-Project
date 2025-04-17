#pragma once

#include "CoreMinimal.h"
#include "Items/Class/SpawnableItem.h"
#include "TransformItem.generated.h"

UENUM(BlueprintType)
enum class ETransformType : uint8
{
	None				UMETA(DisplayName = "None"),
	Shrink				UMETA(DisplayName = "Shrink"),
	Enlarge 			UMETA(DisplayName = "Enlarge")
};

UCLASS()
class CCFF_API ATransformItem : public ASpawnableItem
{
	GENERATED_BODY()
	
public: 
	ATransformItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	float EffectDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	float ShrinkScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	float EnlargeScale;

protected:
	virtual void Interact(AActor* Activator) override;

private:
	ETransformType GetRandomTransformType();
};
