#include "Items/Manager/ItemPoolManager.h"
#include "Items/Class/SpawnableItem.h"
#include "Engine/World.h"

void UItemPoolManager::InitializePool(TSubclassOf<ASpawnableItem> ItemClass, int32 PoolSize)
{
    if (!GetWorld() || ItemPool.Num() > 0) return;  // 이미 풀 초기화됨

    SpawnableItemClass = ItemClass;

    for (int32 i = 0; i < PoolSize; ++i)
    {
        ASpawnableItem* NewItem = GetWorld()->SpawnActor<ASpawnableItem>(ItemClass);
        if (NewItem)
        {
            NewItem->SetActorHiddenInGame(true);
            NewItem->SetActorEnableCollision(false);
            ItemPool.Add(NewItem);
        }
    }
}

ASpawnableItem* UItemPoolManager::GetItemFromPool()
{
    for (ASpawnableItem* Item : ItemPool)
    {
        if (Item && Item->IsHidden())
        {
            Item->SetActorHiddenInGame(false);
            Item->SetActorEnableCollision(true);
            return Item;
        }
    }
    return nullptr;
}

void UItemPoolManager::ReturnItemToPool(ASpawnableItem* Item)
{
    if (Item)
    {
        Item->SetActorHiddenInGame(true);
        Item->SetActorEnableCollision(false);
        ItemPool.Add(Item);
    }
}