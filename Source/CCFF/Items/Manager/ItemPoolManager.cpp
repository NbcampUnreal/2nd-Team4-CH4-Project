#include "Items/Manager/ItemPoolManager.h"
#include "Items/Manager/ItemManager.h"
#include "Items/Class/SpawnableItem.h"
#include "Items/Class/ItemSpawner.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"

void UItemPoolManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UItemPoolManager::InitializePool()
{
    if (!IsServer() || !GetWorld() || ItemPool.Num() > 0)
        return;

    UE_LOG(LogTemp, Log, TEXT("ItemPoolManager: Initializing item pool."));

    static const FString DataTablePath = TEXT("/Game/CCFF/DataTables/ItemSpawnTable.ItemSpawnTable");
    ItemDataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemPoolManager: Failed to load ItemSpawnTable."));
        return;
    }

    TArray<FItemSpawnRow*> ItemRows;
    ItemDataTable->GetAllRows<FItemSpawnRow>(TEXT(""), ItemRows);

    float TotalChance = 0.0f;
    for (const FItemSpawnRow* Row : ItemRows)
    {
        if (Row)
            TotalChance += Row->SpawnChance;
    }
    if (TotalChance <= 0.0f) return;

    int32 SpawnerCount = GetSpawnerCount();
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
                UE_LOG(LogTemp, Log, TEXT("ItemPoolManager: %s added to ItemPool"), *NewItem->GetName());
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("ItemPoolManager: Initialization complete. Total items in pool: %d."), ItemPool.Num());
}



ASpawnableItem* UItemPoolManager::GetRandomItemFromPool()
{
    if (ItemPool.Num() == 0) return nullptr;

    TArray<ASpawnableItem*> AvailableItems;
    for (ASpawnableItem* Item : ItemPool)
    {
        if (Item && Item->IsHidden())
        {
            AvailableItems.Add(Item);
        }
    }

    if (AvailableItems.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AvailableItems.Num() - 1);
        ASpawnableItem* SelectedItem = AvailableItems[RandomIndex];

        if (SelectedItem)
        {
            SelectedItem->SetActorHiddenInGame(false);
            SelectedItem->SetActorEnableCollision(true);
            return SelectedItem;
        }
    }

    return nullptr;
}

void UItemPoolManager::ReturnItemToPool(ASpawnableItem* Item)
{
    if (!Item || ItemPool.Contains(Item)) return;

    Item->SetActorHiddenInGame(true);
    Item->SetActorEnableCollision(false);
    ItemPool.Add(Item);
}

bool UItemPoolManager::IsServer() const
{
    return GetWorld() && GetWorld()->GetNetMode() != NM_Client;
}

int32 UItemPoolManager::GetSpawnerCount() const
{
    int32 Count = 0;
    for (TActorIterator<AItemSpawner> It(GetWorld()); It; ++It)
    {
        ++Count;
    }
    return Count;
}
