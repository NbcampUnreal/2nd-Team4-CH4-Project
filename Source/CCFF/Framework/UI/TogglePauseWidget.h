#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TogglePauseWidget.generated.h"

class UButton;

UCLASS()
class CCFF_API UTogglePauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnSettingButtonClicked();

	UFUNCTION()
	void OnLobbyButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LobbyButton;
};
