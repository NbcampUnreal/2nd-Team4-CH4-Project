#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "LobbyWidget.generated.h"

UCLASS()
class CCFF_API ULobbyWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

protected:
    UFUNCTION()
    void OnReadyClicked();

    UPROPERTY(meta = (BindWidget))
    class UButton* ReadyButton;

};
