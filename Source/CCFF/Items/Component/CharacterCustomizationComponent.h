#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/DataAsset/CustomizationItemAsset.h"
#include "Items/Structure/CustomizationPreset.h"
#include "CharacterCustomizationComponent.generated.h"

USTRUCT(BlueprintType)
struct FEquippedItemEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipSlot Slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	FEquippedItemEntry() : Slot(EEquipSlot::None), ItemID(NAME_None) {}
	FEquippedItemEntry(EEquipSlot InSlot, FName InItemID) : Slot(InSlot), ItemID(InItemID) {}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CCFF_API UCharacterCustomizationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterCustomizationComponent();

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(UCustomizationItemAsset* ItemData);
	void Server_EquipItem_Implementation(UCustomizationItemAsset* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Customization")
	void EquipItem(UCustomizationItemAsset* ItemData);
	UFUNCTION(BlueprintCallable, Category = "Customization")
	void UnequipSlot(EEquipSlot Slot);
	
	UFUNCTION(BlueprintCallable)
	void ApplyPreset(int32 Index);
	UFUNCTION(BlueprintCallable)
	void SaveCurrentToPreset(int32 Index);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedItems)
	TArray<FEquippedItemEntry> EquippedItems;
	UPROPERTY()
	TMap<EEquipSlot, UStaticMeshComponent*> EquippedMeshComponents;

	UPROPERTY()
	TArray<FCustomizationPreset> Presets;

	void BeginPlay() override;
	void UnequipAll();
	UFUNCTION()
	void OnRep_EquippedItems();

private:
	UCustomizationItemAsset* FindCustomizationAssetByID(FName ItemID);
};
