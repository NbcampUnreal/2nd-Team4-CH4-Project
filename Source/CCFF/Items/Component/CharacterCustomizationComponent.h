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

    // ĳ���Ϳ� Ŀ���͸���¡ ����
    UFUNCTION(BlueprintCallable, Category = "Customization")
    void ApplyCustomization(const FCustomizationPreset& Preset);

    // ���� Ŀ���͸���¡ �ʱ�ȭ
    UFUNCTION(BlueprintCallable, Category = "Customization")
    void ClearCustomization();

protected:
    virtual void BeginPlay() override;

    // ������ StaticMeshComponent ���
    UPROPERTY()
    TMap<EEquipSlot, UStaticMeshComponent*> AttachedMeshes;

    // ���ο��� Socket�� StaticMeshComponent ���̴� �Լ�
    void AttachItemToSlot(EEquipSlot Slot, UStaticMesh* Mesh, FName SocketName);
};
