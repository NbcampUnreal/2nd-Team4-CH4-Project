#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManager.generated.h"

class ASpawnableItem;
class AItemSpawner;
class UItemPoolManager;

UCLASS()
class CCFF_API UItemManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_PickupItem(APlayerController* Player, ASpawnableItem* Item);
    void Server_PickupItem_Implementation(APlayerController* Player, ASpawnableItem* Item);
    bool Server_PickupItem_Validate(APlayerController* Player, ASpawnableItem* Item) { return true; };

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_UpdateItemState(ASpawnableItem* Item, bool bIsActive);
    void Multicast_UpdateItemState_Implementation(ASpawnableItem* Item, bool bIsActive);

    void SpawnItemAtSpawner(AItemSpawner* Spawner);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    UPROPERTY()
    TArray<ASpawnableItem*> ActiveItems;

    UPROPERTY()
    UItemPoolManager* ItemPoolManager;
    
    bool IsServer() const;
    void OnWorldReady();
    void ActivateAllSpawners();
};