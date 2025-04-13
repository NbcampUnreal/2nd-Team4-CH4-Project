#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Base/DamageAble.h"
#include "EnvironmentItem.generated.h"

UCLASS()
class CCFF_API AEnvironmentItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironmentItem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category = "Item|Component|Effects")
	USoundBase* OnHitSound;

private:
	UPROPERTY(EditAnywhere, Category = "Item|Component|Effects")
	float MaxDurability;
};
