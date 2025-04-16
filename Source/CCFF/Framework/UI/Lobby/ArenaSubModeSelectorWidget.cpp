#include "Framework/UI/Lobby/ArenaSubModeSelectorWidget.h"
#include "Components/Button.h"

void UArenaSubModeSelectorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (DeathMatchButton && !DeathMatchButton->OnClicked.IsAlreadyBound(this, &UArenaSubModeSelectorWidget::OnDeathMatchClicked))
	{
		DeathMatchButton->OnClicked.AddDynamic(this, &UArenaSubModeSelectorWidget::OnDeathMatchClicked);
	}

	if (EliminationButton && !EliminationButton->OnClicked.IsAlreadyBound(this, &UArenaSubModeSelectorWidget::OnEliminationClicked))
	{
		EliminationButton->OnClicked.AddDynamic(this, &UArenaSubModeSelectorWidget::OnEliminationClicked);
	}

	CurrentSelectedMode = EArenaSubMode::Elimination;
	UpdateSelectionUI();
}

void UArenaSubModeSelectorWidget::SetCurrentSelectedMode(EArenaSubMode NewMode)
{
	CurrentSelectedMode = NewMode;
	UpdateSelectionUI();
}

void UArenaSubModeSelectorWidget::UpdateSelectionUI()
{
	if (DeathMatchButton)
	{
		const bool bSelected = (CurrentSelectedMode == EArenaSubMode::DeathMatch);

		FLinearColor OriginalColor = DeathMatchButton->GetBackgroundColor();
		OriginalColor.A = bSelected ? SelectedValue : UnselectedValue;

		FLinearColor HSV = OriginalColor.LinearRGBToHSV();
		HSV.B = bSelected ? SelectedValue : UnselectedValue;

		DeathMatchButton->SetBackgroundColor(HSV.HSVToLinearRGB());
	}

	if (EliminationButton)
	{
		const bool bSelected = (CurrentSelectedMode == EArenaSubMode::Elimination);

		FLinearColor OriginalColor = EliminationButton->GetBackgroundColor();
		OriginalColor.A = bSelected ? SelectedValue : UnselectedValue;

		FLinearColor HSV = OriginalColor.LinearRGBToHSV();
		HSV.B = bSelected ? SelectedValue : UnselectedValue;

		EliminationButton->SetBackgroundColor(HSV.HSVToLinearRGB());
	}
}

void UArenaSubModeSelectorWidget::OnEliminationClicked()
{
	OnSubModeSelected.Broadcast(EArenaSubMode::Elimination);
}

void UArenaSubModeSelectorWidget::OnDeathMatchClicked()
{
	OnSubModeSelected.Broadcast(EArenaSubMode::DeathMatch);
}
