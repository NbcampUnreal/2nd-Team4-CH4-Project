#include "Items/Manager/ItemManager.h"
#include "Items/Manager/ItemPoolManager.h"
#include "Items/Class/ItemSpawner.h"
#include "Items/Class/SpawnableItem.h"
#include "Items/Component/ItemInteractionComponent.h"
#include "EngineUtils.h"

void UItemManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency<UItemPoolManager>();
	InitializeItemManager();
}

void UItemManager::InitializeItemManager()
{
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
			Spawner->SpawnItem(); 
			SpawnerCount++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("ItemManager: Activated %d spawners."), SpawnerCount);
}


void UItemManager::OnItemInteract(AActor* Interactor, UItemInteractionComponent* ItemInteractionComponent, ASpawnableItem* Item)
{
	if (!IsServer() || !Item) return;
	UE_LOG(LogTemp, Log, TEXT("ItemManager: %s interacted with item %s"), *Interactor->GetName(), *Item->GetName());

	ActiveItems.Remove(Item);
	ItemPoolManager->ReturnItemToPool(Item);
	// Include Item Interaction Decision Logic here 
	
	ItemInteractionComponent->HandleInteractionEffects(Item);
	Item->Interact(Interactor);
	Item->OwningSpawner = nullptr;
}

bool UItemManager::IsServer() const
{
	return GetWorld() && GetWorld()->GetNetMode() != NM_Client;
}
