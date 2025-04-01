#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Interface/InteractableItemInterface.h"
#include "SpawnableItem.generated.h"

class UNiagaraSystem;
class USoundBase;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemResetDelegate);

UCLASS()
class CCFF_API ASpawnableItem : public AActor, public IInteractableItemInterface
{

	GENERATED_BODY()
	
public:	
	ASpawnableItem();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnInteract();
	void Multicast_OnInteract_Implementation();

	UPROPERTY(BlueprintAssignable)
	FItemResetDelegate OnItemResetDelegate;

	void ResetItem(); // 풀링을 위한 초기화

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;
	UPROPERTY(EditAnywhere, Category = "Item|Effects")
	USoundBase* InteractSound;
	UPROPERTY(EditAnywhere, Category = "Item|Effects")
	UNiagaraSystem* InteractParticle;

	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void Interact(AActor* Activator) override;
	virtual FName GetItemType() const override { return ItemType; };

	virtual void BeginPlay() override;

};
