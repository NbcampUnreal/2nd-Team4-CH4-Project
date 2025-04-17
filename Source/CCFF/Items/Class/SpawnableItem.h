#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Interface/InteractableItemInterface.h"
#include "SpawnableItem.generated.h"

class UNiagaraSystem;
class USoundBase;
class USphereComponent;
class AItemSpawner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemReturnedToPool, ASpawnableItem*, Item);

UENUM()
enum class EItemType : uint8
{
	None				UMETA(DisplayName = "None"),
	Healing				UMETA(DisplayName = "Healing"),
	Resistivity			UMETA(DisplayName = "Resistivity"),
	Speed			UMETA(DisplayName = "Speed"),
	Transformation		UMETA(DisplayName = "Transformation")
};

UCLASS()
class CCFF_API ASpawnableItem : public AActor, public IInteractableItemInterface
{

	GENERATED_BODY()
	
public:	
	ASpawnableItem();

	UPROPERTY(Replicated)
	AItemSpawner* OwningSpawner = nullptr;
	UPROPERTY(BlueprintAssignable)
	FOnItemReturnedToPool OnReturnedToPool;

	FORCEINLINE EItemType GetItemType() const { return ItemType; }
	virtual void Interact(AActor* Activator) override;
	void OnInteract();
	void ResetItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType = EItemType::None;
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

	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnSpawned();

private:
	void UpdateFloating();

	FVector InitialLocation;
	float TimeElapsed;

	FTimerHandle FloatingTimerHandle;

};
