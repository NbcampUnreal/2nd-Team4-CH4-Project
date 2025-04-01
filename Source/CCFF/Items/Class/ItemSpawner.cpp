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
    //if (HasAuthority()) // ���������� ����
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
    // ���� Ÿ�̸Ӱ� ���� ��� ����
    if (GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    }

    // ���ο� Ÿ�̸� ����
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AItemSpawner::SpawnItemDelayed, SpawnCooldown, false);
    bIsItemActive = false;
}

void AItemSpawner::SpawnItemDelayed()
{
    if (!bIsItemActive)
    {
        SpawnItem();
          // �������� Ȱ��ȭ�� ���·� ����
    }
}