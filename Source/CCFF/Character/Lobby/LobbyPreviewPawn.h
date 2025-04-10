#pragma once

#include "CoreMinimal.h"
#include "Character/Base/BasePreviewPawn.h"
#include "LobbyPreviewPawn.generated.h"

class UWidgetComponent;

UCLASS()
class CCFF_API ALobbyPreviewPawn : public ABasePreviewPawn
{
	GENERATED_BODY()

public:
	ALobbyPreviewPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_PlayerState() override;

public:
	void SetPlayerName(const FString& InName);
	void SetReadyState(bool bIsReady);

private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UWidgetComponent> NameTagWidget;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UWidgetComponent> ReadyStatusWidget;

	bool bNameSet = false;
};