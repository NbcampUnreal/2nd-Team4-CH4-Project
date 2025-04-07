#include "Framework/UI/ConfirmPopupWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UConfirmPopupWidget::SetMessage(const FText& NewMessage)
{
	if (MessageText)
	{
		MessageText->SetText(NewMessage);
	}
}

void UConfirmPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UConfirmPopupWidget::OnConfirm);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UConfirmPopupWidget::OnCancel);
	}
}

void UConfirmPopupWidget::OnConfirm()
{
	OnConfirmPopupConfirmed.Broadcast();
	RemoveFromParent();
}

void UConfirmPopupWidget::OnCancel()
{
	OnConfirmPopupCanceled.Broadcast();
	RemoveFromParent();
}
