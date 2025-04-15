#include "Items/Class/SpawnableItem.h"
#include "Items/Component/ItemInteractionComponent.h"
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

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ASpawnableItem::OnItemOverlap);

    bReplicates = true;
    SetReplicateMovement(true);
}

void ASpawnableItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnableItem::OnSpawned()
{
    InitialLocation = GetActorLocation();
    TimeElapsed = 0.0f;
    GetWorldTimerManager().SetTimer(FloatingTimerHandle, this, &ASpawnableItem::UpdateFloating, 0.02f, true);
}

void ASpawnableItem::UpdateFloating()
{
    TimeElapsed += 0.02f;  // 타이머 주기와 맞춤 (0.02초마다 호출됨)
    FVector NewLocation = InitialLocation;
    NewLocation.Z += FMath::Sin(TimeElapsed * 2.0f) * 10.0f;  // 위아래로 10만큼 움직임
    SetActorLocation(NewLocation);
}

// need to CHECK LOGIC ON AUTORITY
void ASpawnableItem::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    APawn* OverlappingPawn = Cast<APawn>(OtherActor);
    if (OverlappingPawn)
    {
        bool bIsLocallyControlled = OverlappingPawn->IsLocallyControlled();
        bool bHasAuthority = HasAuthority();

        FString LogMsg = FString::Printf(TEXT("Overlap Detected: %s | LocallyControlled: %s | HasAuthority: %s"),
            *OtherActor->GetName(),
            bIsLocallyControlled ? TEXT("true") : TEXT("false"),
            bHasAuthority ? TEXT("true") : TEXT("false"));

        UE_LOG(LogTemp, Log, TEXT("%s"), *LogMsg);

#if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, LogMsg);
#endif

        if (bIsLocallyControlled && !bHasAuthority)
        {
            UItemInteractionComponent* InteractionComponent = OtherActor->FindComponentByClass<UItemInteractionComponent>();
            if (InteractionComponent)
            {
                UE_LOG(LogTemp, Log, TEXT("Calling Server_InteractItem on %s"), *GetName());

#if WITH_EDITOR
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
                    FString::Printf(TEXT("Calling Server_InteractItem on %s"), *GetName()));
#endif
                InteractionComponent->Server_InteractItem(this);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No ItemInteractionComponent found on %s"), *OtherActor->GetName());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Overlap failed: %s is not a Pawn"), *OtherActor->GetName());
    }

}

void ASpawnableItem::Interact(AActor* Activator)
{
#if WITH_EDITOR
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Interact!")));
#endif

	// Item Interaction Logic Here
}

void ASpawnableItem::OnInteract()
{
    if (!this)
    {
        UE_LOG(LogTemp, Error, TEXT("OnInteract called on a nullptr object!"));
        return;
    }

    if (InteractSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Interact Sound!")));
    }

    if (InteractParticle)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, InteractParticle, GetActorLocation());
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Interact Particle!")));
    }
}

void ASpawnableItem::ResetItem()
{
    GetWorldTimerManager().ClearTimer(FloatingTimerHandle);

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


