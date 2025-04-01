#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemPoolManager.generated.h"

class ASpawnableItem;

UCLASS()
class CCFF_API UItemPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    void InitializePool(TSubclassOf<ASpawnableItem> ItemClass, int32 PoolSize);

    ASpawnableItem* GetItemFromPool();

    void ReturnItemToPool(ASpawnableItem* Item);

private:
    TArray<ASpawnableItem*> ItemPool;
    TSubclassOf<ASpawnableItem> SpawnableItemClass;
};
