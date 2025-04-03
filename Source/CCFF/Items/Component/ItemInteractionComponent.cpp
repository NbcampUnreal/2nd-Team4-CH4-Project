#include "Items/Component/ItemInteractionComponent.h"
#include "Items/Class/SpawnableItem.h"
#include "Net/UnrealNetwork.h"

UItemInteractionComponent::UItemInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UItemInteractionComponent::Server_InteractItem_Implementation(ASpawnableItem* Item)
{
    if (Item && Item->HasAuthority())
    {
        Item->Interact(GetOwner()); // �������� ��ȣ�ۿ� ����
    }
}
