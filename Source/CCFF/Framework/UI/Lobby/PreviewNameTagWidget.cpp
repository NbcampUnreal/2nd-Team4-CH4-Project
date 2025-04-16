#include "Framework/UI/Lobby/PreviewNameTagWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPreviewNameTagWidget::SetPlayerName(const FString& NewName)
{
	if (IsValid(PlayerNameText) == true)
	{
		PlayerNameText->SetText(FText::FromString(NewName));
	}
}

void UPreviewNameTagWidget::SetHostMarkVisible(bool bVisible)
{
	if (HostImage)
	{
		HostImage->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);	}
}
