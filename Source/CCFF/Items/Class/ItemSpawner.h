#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/DataTable/ItemSpawnRow.h"
#include "ItemSpawner.generated.h"

class ASpawnableItem;
class UItemManager;
class UItemPoolManager;
class UWidgetComponent;

UCLASS()
class CCFF_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AItemSpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|UI")
	UWidgetComponent* WidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Spawn")
	UDataTable* ItemDataTable;

	void SpawnItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Spawn")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, Category = "Item|Spawn")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Spawn")
	float SpawnCooldown = 3.0f;
	

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleItemReturned(ASpawnableItem* Item);

	UItemPoolManager* ItemPoolManager;
	UItemManager* ItemManager;
	FTimerHandle SpawnTimerHandle; // 타이머 핸들

	bool bIsItemActive=false;

};
