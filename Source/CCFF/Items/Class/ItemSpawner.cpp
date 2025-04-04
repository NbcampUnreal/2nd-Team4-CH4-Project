#include "Items/Class/ItemSpawner.h"
#include "Items/Class/SpawnableItem.h"
#include "Items/Manager/ItemManager.h"
#include "Items/Manager/ItemPoolManager.h"
#include "Engine/GameInstance.h"

AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(Scene);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));
    StaticMesh->SetupAttachment(Scene);
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	ItemPoolManager = GetGameInstance()->GetSubsystem<UItemPoolManager>();
}

void AItemSpawner::SpawnItem()
{
    if (bIsItemActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemSpawner %s : Spawn skipped item is already active."), *GetName());
        return;
    }

    if (GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
    {
        UE_LOG(LogTemp, Log, TEXT("ItemSpawner %s : SpawnTimer is active. Clearing it."), *GetName());
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    }

    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemSpawner %s : SpawnItem called but does not have authority."), *GetName());
        return;
    }

    if (ItemPoolManager == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemSpawner %s : ItemPoolManager is nullptr!"), *GetName());
        return;
    }

    ASpawnableItem* NewItem = ItemPoolManager->GetRandomItemFromPool();
    if (NewItem)
    {
        FVector SpawnLocation = GetActorLocation();
        SpawnLocation.Z += 50.0f;
        NewItem->SetActorLocation(SpawnLocation);
        NewItem->SetActorHiddenInGame(false);
        NewItem->SetActorEnableCollision(true);
        NewItem->OnSpawned();
        NewItem->OwningSpawner = this;
        NewItem->OnReturnedToPool.AddDynamic(this, &AItemSpawner::HandleItemReturned);

        UE_LOG(LogTemp, Log, TEXT("ItemSpawner %s : Successfully spawned item %s at location %s"),
            *GetName(),
            *NewItem->GetName(),
            *SpawnLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemSpawner %s : Failed to spawn item pool returned nullptr."), *GetName());
    }

    bIsItemActive = true;
    UE_LOG(LogTemp, Log, TEXT("ItemSpawner %s : bIsItemActive set to true."), *GetName());
}


void AItemSpawner::HandleItemReturned(ASpawnableItem* Item)
{
    bIsItemActive = false;
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AItemSpawner::SpawnItem, SpawnCooldown, false);
	Item->OnReturnedToPool.RemoveDynamic(this, &AItemSpawner::HandleItemReturned);
}
