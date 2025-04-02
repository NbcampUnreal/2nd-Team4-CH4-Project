#include "Items/Manager/ItemPoolManager.h"
#include "Items/Class/SpawnableItem.h"
#include "Items/Class/ItemSpawner.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Engine/DataTable.h"

void UItemPoolManager::InitializePool(UDataTable* ItemDataTable)
{
    if (!GetWorld() || !ItemDataTable || ItemPool.Num() > 0) return; // �ʱ�ȭ ���� Ȯ��

    // �ʿ� �����ϴ� Spawner ���� Ȯ��
    int32 SpawnerCount = 0;
    for (TActorIterator<AItemSpawner> It(GetWorld()); It; ++It)
    {
        ++SpawnerCount;
    }
    if (SpawnerCount == 0) return;

    // DataTable���� ������ ���� ��������
    TArray<FItemSpawnRow*> ItemRows;
    ItemDataTable->GetAllRows<FItemSpawnRow>(TEXT(""), ItemRows);

    // SpawnChance�� ���� ���
    float TotalChance = 0.0f;
    for (const FItemSpawnRow* Row : ItemRows)
    {
        TotalChance += Row->SpawnChance;
    }
    if (TotalChance <= 0.0f) return;

    // ������ Ǯ ���� (Spawner ���� �������� �й�)
    for (const FItemSpawnRow* Row : ItemRows)
    {
        if (!Row || !Row->ItemClass) continue;

        int32 ItemCount = FMath::RoundToInt((Row->SpawnChance / TotalChance) * SpawnerCount);
        for (int32 i = 0; i < ItemCount; ++i)
        {
            ASpawnableItem* NewItem = GetWorld()->SpawnActor<ASpawnableItem>(Row->ItemClass);
            if (NewItem)
            {
                NewItem->SetActorHiddenInGame(true);
                NewItem->SetActorEnableCollision(false);
                ItemPool.Add(NewItem);
            }
        }
    }
}


ASpawnableItem* UItemPoolManager::GetRandomItemFromPool() // Need to be fixed
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