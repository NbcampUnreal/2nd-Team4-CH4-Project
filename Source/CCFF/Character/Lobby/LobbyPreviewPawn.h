#pragma once

#include "CoreMinimal.h"
#include "Character/Base/BasePreviewPawn.h"
#include "LobbyPreviewPawn.generated.h"

UCLASS()
class CCFF_API ALobbyPreviewPawn : public ABasePreviewPawn
{
	GENERATED_BODY()
	
public:
	ALobbyPreviewPawn();

	virtual void OnRep_PlayerState() override;

	void SetPlayerName(const FString& Name);
	void SetReadyState(bool bReady);

protected:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* NameTagWidget;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* ReadyStatusWidget;

};
