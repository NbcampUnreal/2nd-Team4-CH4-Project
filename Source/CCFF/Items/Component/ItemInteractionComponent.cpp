#include "Items/Component/ItemInteractionComponent.h"
#include "Items/Manager/ItemManager.h"
#include "Items/Class/SpawnableItem.h"
#include "Net/UnrealNetwork.h"

UItemInteractionComponent::UItemInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UItemInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		if (UGameInstance* GameInstance = GetOwner()->GetGameInstance())
		{
			ItemManager = GameInstance->GetSubsystem<UItemManager>();
		}
	}

	if (!ItemManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemManager is nullptr in UItemInteractionComponent"));
	}
}

void UItemInteractionComponent::Server_InteractItem_Implementation(ASpawnableItem* Item)
{
    if (ItemManager->IsServer())
    {
        ItemManager->OnItemInteract(GetOwner(), this, Item); // �������� ��ȣ�ۿ� ����
    }
}

void UItemInteractionComponent::HandleInteractionEffects(ASpawnableItem* Item)
{
	if (Item)
	{
		Multicast_PlayInteractionEffects(Item); // Ŭ���̾�Ʈ���� ȿ�� ���
	}
}

void UItemInteractionComponent::Multicast_PlayInteractionEffects_Implementation(ASpawnableItem* Item)
{
	Item->OnInteract(); // ��ȣ�ۿ� ȿ�� ����
}