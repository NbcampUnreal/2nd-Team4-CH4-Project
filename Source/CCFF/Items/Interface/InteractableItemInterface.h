#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableItemInterface : public UInterface
{
	GENERATED_BODY()
};

class CCFF_API IInteractableItemInterface
{
    GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) PURE_VIRTUAL(IInteractableItemInterface::OnItemOverlap, );

    virtual void Interact(AActor* Activator) PURE_VIRTUAL(IInteractableItemInterface::Interact, );
};