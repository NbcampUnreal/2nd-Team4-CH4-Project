// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/DataAsset/CustomizationItemAsset.h"
#include "CharacterCustomizationComponent.generated.h"


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
	
protected:
	UPROPERTY()
	TMap<EEquipSlot, UStaticMeshComponent*> EquippedMeshComponents;
	UPROPERTY()
	TMap<EEquipSlot, UCustomizationItemAsset*> EquippedItems;
};
