// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
	//Initialize Comps
	RootComponent=CreateDefaultSubobject<USceneComponent>("Root");
	// Set Collision Components
	CollisionComp=CreateDefaultSubobject<USphereComponent>("CollisionComp");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit); //Bind Hit Event
	CollisionComp->SetSphereRadius(15.0f);
	RootComponent=CollisionComp;
	// Set ProjectileMovement Components
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComp->SetUpdatedComponent(CollisionComp);
	ProjectileMovementComp->InitialSpeed = 3000.0f;
	ProjectileMovementComp->MaxSpeed = 3000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.3f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	//Set Static Mesh Components
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	StaticMeshComp->SetupAttachment(RootComponent);
	//Set Life Span
	InitialLifeSpan=3.0f;
}

void ABaseProjectile::SetFireDirection(const FVector& Direction)
{
	ProjectileMovementComp->Velocity = Direction*ProjectileMovementComp->InitialSpeed;
}


void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor!=this&&OtherComponent->IsSimulatingPhysics())
	{
		// Damage
	}
	Destroy();
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
