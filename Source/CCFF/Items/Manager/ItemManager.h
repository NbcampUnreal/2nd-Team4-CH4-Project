#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManager.generated.h"

class ASpawnableItem;
class AItemSpawner;
class UItemPoolManager;
class UItemInteractionComponent;

UCLASS()
class CCFF_API UItemManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool IsServer() const;
	void OnItemInteract(AActor* Interactor, UItemInteractionComponent* ItemInteractionComponent, ASpawnableItem* Item);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    UPROPERTY()
    TArray<ASpawnableItem*> ActiveItems;

    UPROPERTY()
    UItemPoolManager* ItemPoolManager;
    

    void OnWorldReady();
    void ActivateAllSpawners();
};