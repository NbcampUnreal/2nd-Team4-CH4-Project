#include "Framework/UI/QuitPopupWidget.h"
#include "Components/Button.h"

void UQuitPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UQuitPopupWidget::OnConfirmClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UQuitPopupWidget::OnCancelClicked);
	}
}

void UQuitPopupWidget::OnConfirmClicked()
{
	OnQuitConfirmed.Broadcast();
	RemoveFromParent();
}

void UQuitPopupWidget::OnCancelClicked()
{
	OnQuitCanceled.Broadcast();
	RemoveFromParent();
}
