#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainingDummy.generated.h"

UCLASS()
class CCFF_API ATrainingDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrainingDummy();

protected:
	UFUNCTION(BlueprintCallable, Category = "CCFF|Traning")
	void ReciveDamage(float DamageAmount);

	UPROPERTY(VisibleAnywhere, Category = "CCFF|Traning")
	USkeletalMeshComponent* DummyMesh;

};
