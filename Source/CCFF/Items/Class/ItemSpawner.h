#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/DataTable/ItemSpawnRow.h"
#include "ItemSpawner.generated.h"

class ASpawnableItem;

UCLASS()
class CCFF_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AItemSpawner();

	UFUNCTION(BlueprintCallable, Category = "Item|Spawn")
	void SpawnRandomItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Spawn")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, Category = "Item|Spawn")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Spawn")
	UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Spawn")
	float SpawnCooldown = 3.0f;


	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnItemReset();


	void SpawnItemDelayed();
	FTimerHandle SpawnTimerHandle; // 타이머 핸들

	bool bIsItemActive=false;

};
