#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Items/DataTable/ItemSpawnRow.h"
#include "ItemPoolManager.generated.h"

class ASpawnableItem;

UCLASS()
class CCFF_API UItemPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    void InitializePool(UDataTable* DataTable);
    ASpawnableItem* GetRandomItemFromPool();
    void ReturnItemToPool(ASpawnableItem* Item);

private:
    TArray<ASpawnableItem*> ItemPool;
    TSubclassOf<ASpawnableItem> SpawnableItemClass;
};
