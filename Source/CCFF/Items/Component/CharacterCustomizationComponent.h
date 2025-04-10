#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/DataAsset/CustomizationItemAsset.h"
#include "Items/Structure/CustomizationPresetTypes.h"
#include "CharacterCustomizationComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CCFF_API UCharacterCustomizationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterCustomizationComponent();

    // 캐릭터에 커스터마이징 적용
    UFUNCTION(BlueprintCallable, Category = "Customization")
    void ApplyCustomization(const FCustomizationPreset& Preset);

    // 현재 커스터마이징 초기화
    UFUNCTION(BlueprintCallable, Category = "Customization")
    void ClearCustomization();

protected:
    virtual void BeginPlay() override;

    // 부착된 StaticMeshComponent 목록
    UPROPERTY()
    TMap<EEquipSlot, UStaticMeshComponent*> AttachedMeshes;

    // 내부에서 Socket에 StaticMeshComponent 붙이는 함수
    void AttachItemToSlot(EEquipSlot Slot, UStaticMesh* Mesh, FName SocketName);
};
