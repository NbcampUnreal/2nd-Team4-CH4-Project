#include "Items/Class/ItemSpawner.h"
#include "Items/Class/SpawnableItem.h"
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

    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        UItemPoolManager* PoolManager = GameInstance->GetSubsystem<UItemPoolManager>();
        if (PoolManager&&ItemClass)
        {
            PoolManager->InitializePool(ItemClass, 4);
            SpawnItem();
        }
    }    
}

void AItemSpawner::SpawnItem()
{
    //if (HasAuthority()) // 서버에서만 실행
    //{
    //    
    //}

    UItemPoolManager* PoolManager = GetGameInstance()->GetSubsystem<UItemPoolManager>();
    if (PoolManager)
    {
        ASpawnableItem* Item = PoolManager->GetItemFromPool();
        if (Item)
        {
            FVector SpawnLocation = GetActorLocation();
            SpawnLocation.Z += 100.0f;

            Item->SetActorLocation(SpawnLocation);
            Item->SetActorHiddenInGame(false);
            Item->SetActorEnableCollision(true);

            Item->OnItemResetDelegate.RemoveDynamic(this, &AItemSpawner::OnItemReset);
            Item->OnItemResetDelegate.AddDynamic(this, &AItemSpawner::OnItemReset);
            bIsItemActive = true;
        }
    }
}

void AItemSpawner::OnItemReset()
{
    // 기존 타이머가 있을 경우 종료
    if (GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    }

    // 새로운 타이머 설정
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AItemSpawner::SpawnItemDelayed, SpawnCooldown, false);
    bIsItemActive = false;
}

void AItemSpawner::SpawnItemDelayed()
{
    if (!bIsItemActive)
    {
        SpawnItem();
          // 아이템이 활성화된 상태로 설정
    }
}