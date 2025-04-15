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
}

void UArenaSubModeSelectorWidget::SetCurrentSelectedMode(EArenaSubMode NewMode)
{
	CurrentSelectedMode = NewMode;
	UpdateSelectionVisuals();
}

void UArenaSubModeSelectorWidget::UpdateSelectionVisuals()
{
	if (DeathMatchButton)
	{
		const bool bSelected = (CurrentSelectedMode == EArenaSubMode::DeathMatch);

		FLinearColor Color = DeathMatchButton->GetBackgroundColor();
		Color.A = bSelected ? 0.0f : 1.0f;

		DeathMatchButton->SetBackgroundColor(Color);
	}

	if (EliminationButton)
	{
		const bool bSelected = (CurrentSelectedMode == EArenaSubMode::Elimination);

		FLinearColor Color = EliminationButton->GetBackgroundColor();
		Color.A = bSelected ? 0.0f : 1.0f;

		EliminationButton->SetBackgroundColor(Color);
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
