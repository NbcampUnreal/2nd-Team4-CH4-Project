#include "Framework/UI/CheckPopupWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Framework/Application/SlateApplication.h"

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
		ConfirmButton->SetKeyboardFocus();
	}
}

FReply UCheckPopupWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Virtual_Accept)
	{
		OnConfirm();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCheckPopupWidget::OnConfirm()
{
	OnCheckPopupConfirmed.Broadcast();
	RemoveFromParent();
}
