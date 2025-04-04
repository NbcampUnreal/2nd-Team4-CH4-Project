#include "TrainingDummy.h"


ATrainingDummy::ATrainingDummy()
{
	PrimaryActorTick.bCanEverTick = false;

	DummyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DummyMesh"));
	RootComponent = DummyMesh;

	DummyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DummyMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	DummyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

void ATrainingDummy::ReciveDamage(float DamageAmount)
{
	UE_LOG(LogTemp, Log, TEXT("Dummy received %f damage"), DamageAmount);
}


