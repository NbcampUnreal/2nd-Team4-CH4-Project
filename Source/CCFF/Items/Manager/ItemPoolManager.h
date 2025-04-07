#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemPoolManager.generated.h"

class ASpawnableItem;
class UDataTable;
struct FItemSpawnRow;

UCLASS()
class CCFF_API UItemPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    void InitializePool();
    ASpawnableItem* GetRandomItemFromPool();
    void ReturnItemToPool(ASpawnableItem* Item);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    UPROPERTY()
    TArray<ASpawnableItem*> ItemPool;
    UPROPERTY()
    TSubclassOf<ASpawnableItem> SpawnableItemClass;
    bool IsServer() const;
    int32 GetSpawnerCount() const;
    UDataTable* ItemDataTable;
};
