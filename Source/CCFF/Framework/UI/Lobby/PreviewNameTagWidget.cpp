#include "Framework/UI/Lobby/PreviewNameTagWidget.h"
#include "Components/TextBlock.h"

void UPreviewNameTagWidget::SetPlayerName(const FString& NewName)
{
	if (IsValid(PlayerNameText) == true)
	{
		PlayerNameText->SetText(FText::FromString(NewName));
	}
}
