#include "Framework/UI/CheckPopupWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCheckPopupWidget::SetMessage(const FText& NewMessage)
{
	if (MessageText)
	{
		MessageText->SetText(NewMessage);
	}
}

void UCheckPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton && !ConfirmButton->OnClicked.IsAlreadyBound(this, &UCheckPopupWidget::OnConfirm))
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UCheckPopupWidget::OnConfirm);
	}
}

void UCheckPopupWidget::OnConfirm()
{
	OnCheckPopupConfirmed.Broadcast();
	RemoveFromParent();
}
