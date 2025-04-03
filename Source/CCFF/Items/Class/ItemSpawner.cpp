#include "Items/Class/ItemSpawner.h"
#include "Items/Class/SpawnableItem.h"
#include "Items/Manager/ItemManager.h"
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
}

void AItemSpawner::OnItemReset()
{
    if (!HasAuthority()) return; // ���������� ����

    if (GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    }

    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AItemSpawner::SpawnItemDelayed, SpawnCooldown, false);
    bIsItemActive = false;
}

void AItemSpawner::SpawnItemDelayed()
{
    if (!HasAuthority()) return; // ���������� ����

    if (!bIsItemActive && ItemManager)
    {
        ItemManager->SpawnItemAtSpawner(this); // ItemManager�� ��û
        bIsItemActive = true;
    }
}