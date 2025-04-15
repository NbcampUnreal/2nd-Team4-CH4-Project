#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Framework/Data/ArenaSubModeType.h"
#include "ArenaSubModeSelectorWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArenaSubModeSelectedDelegate, EArenaSubMode, SelectedMode);

UCLASS()
class CCFF_API UArenaSubModeSelectorWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetCurrentSelectedMode(EArenaSubMode NewMode);

	UFUNCTION()
	void UpdateSelectionVisuals();

	UPROPERTY()
	FArenaSubModeSelectedDelegate OnSubModeSelected;

protected:
	UFUNCTION()
	void OnEliminationClicked();

	UFUNCTION()
	void OnDeathMatchClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* EliminationButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DeathMatchButton;

	UPROPERTY()
	EArenaSubMode CurrentSelectedMode;

};
