#include "Items/Class/SpawnableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Manager/ItemPoolManager.h"
#include "NiagaraFunctionLibrary.h" 
#include "NiagaraComponent.h"

ASpawnableItem::ASpawnableItem()
{
	PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(Scene);

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Collision->SetupAttachment(Scene);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    StaticMesh->SetupAttachment(Collision);
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractSound = CreateDefaultSubobject<USoundBase>(TEXT("InteractSound"));
    InteractParticle = CreateDefaultSubobject<UNiagaraSystem>(TEXT("InteractParticle"));


    Collision->OnComponentBeginOverlap.AddDynamic(this, &ASpawnableItem::OnItemOverlap);
}

void ASpawnableItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnableItem::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
#if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
#endif
        Interact(OtherActor);
	}
}

void ASpawnableItem::Interact(AActor* Activator)
{
#if WITH_EDITOR
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Interact!")));
#endif

    if (HasAuthority() && Activator)
    {
        Multicast_OnInteract();
        ResetItem();
    }
}

void ASpawnableItem::Multicast_OnInteract_Implementation()
{
    if (InteractSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Interact Sound!")));
    }

    // 파티클 재생
    if (InteractParticle)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, InteractParticle, GetActorLocation());
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Interact Particle!")));
    }
}

void ASpawnableItem::ResetItem()
{
    // 아이템 초기화 및 풀로 반환
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);



    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance found."));

        UItemPoolManager* PoolManager = GameInstance->GetSubsystem<UItemPoolManager>();
        if (PoolManager)
        {
            UE_LOG(LogTemp, Warning, TEXT("Returning item to pool: %s"), *GetName());
            PoolManager->ReturnItemToPool(this);

            UE_LOG(LogTemp, Warning, TEXT("Broadcasting OnItemResetDelegate for %s"), *GetName());
            OnItemResetDelegate.Broadcast();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("PoolManager is null!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance is null!"));
    }
}


