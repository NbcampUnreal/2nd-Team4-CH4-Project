#include "Items/Manager/ItemManager.h"
#include "Items/Manager/ItemPoolManager.h"
#include "Items/Class/ItemSpawner.h"
#include "Items/Class/SpawnableItem.h"
#include "EngineUtils.h"

void UItemManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency<UItemPoolManager>();
	ItemPoolManager = GetGameInstance()->GetSubsystem<UItemPoolManager>();
	if (!ItemPoolManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemManager: Failed to get ItemPoolManager!"));
		return;
	}

	if (GetWorld())
	{
		GetWorld()->OnWorldBeginPlay.AddUObject(this, &UItemManager::OnWorldReady);
		UE_LOG(LogTemp, Log, TEXT("ItemManager: Waiting for world to start..."));
	}
}

void UItemManager::OnWorldReady()
{
	UE_LOG(LogTemp, Log, TEXT("ItemManager: World is ready. Initializing item pool."));

	if (IsServer())
	{
		ItemPoolManager->InitializePool();
		ActivateAllSpawners();
	}	
}

void UItemManager::ActivateAllSpawners()
{
	UE_LOG(LogTemp, Log, TEXT("ItemManager: Activating all spawners..."));

	int32 SpawnerCount = 0;
	for (TActorIterator<AItemSpawner> It(GetWorld()); It; ++It)
	{
		AItemSpawner* Spawner = *It;
		if (Spawner)
		{
			UE_LOG(LogTemp, Log, TEXT("ItemManager: Found spawner: %s"), *Spawner->GetName());

			SpawnItemAtSpawner(Spawner);
			SpawnerCount++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("ItemManager: Activated %d spawners."), SpawnerCount);
}

void UItemManager::SpawnItemAtSpawner(AItemSpawner* Spawner)
{
	if (!ItemPoolManager || !IsServer()) return;

	UE_LOG(LogTemp, Log, TEXT("ItemManager: Spawning item at spawner %s"), *Spawner->GetName());

	ASpawnableItem* NewItem = ItemPoolManager->GetRandomItemFromPool();
	if (NewItem)
	{
		FVector SpawnLocation = Spawner->GetActorLocation();
		SpawnLocation.Z += 50.0f;
		NewItem->SetActorLocation(SpawnLocation);
		NewItem->SetActorHiddenInGame(false);
		NewItem->SetActorEnableCollision(true);
		NewItem->OnSpawned();

		ActiveItems.Add(NewItem);
		UE_LOG(LogTemp, Log, TEXT("ItemManager: Item spawned at %s"), *Spawner->GetName());
	}
}

// need to be replaced
void UItemManager::Server_PickupItem_Implementation(APlayerController* Player, ASpawnableItem* Item)
{
	if (!Item || !Player) return;

	UE_LOG(LogTemp, Log, TEXT("ItemManager: Player %s picked up item %s"), *Player->GetName(), *Item->GetName());

	Item->SetActorHiddenInGame(true);
	Item->SetActorEnableCollision(false);

	if (ItemPoolManager)
	{
		ItemPoolManager->ReturnItemToPool(Item);
		UE_LOG(LogTemp, Log, TEXT("ItemManager: Item returned to pool."));
	}

	ActiveItems.Remove(Item);

	Multicast_UpdateItemState(Item, false);
}

// need to be replaced
void UItemManager::Multicast_UpdateItemState_Implementation(ASpawnableItem* Item, bool bIsActive)
{
	if (!Item) return;

	UE_LOG(LogTemp, Log, TEXT("ItemManager: Item %s state updated. Active: %s"),
		*Item->GetName(), bIsActive ? TEXT("true") : TEXT("false"));

	Item->SetActorHiddenInGame(!bIsActive);
	Item->SetActorEnableCollision(bIsActive);
}

bool UItemManager::IsServer() const
{
	return GetWorld() && GetWorld()->GetNetMode() != NM_Client;
}
