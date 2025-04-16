#include "Framework/UI/Lobby/PreviewReadyStatusWidget.h"
#include "Components/TextBlock.h"

void UPreviewReadyStatusWidget::SetReadyState(bool bIsReady)
{
	if (IsValid(ReadyStatusText))
	{
		const FText StatusText = bIsReady
			? FText::FromString(TEXT("Ready"))
			: FText::FromString(TEXT("Not Ready"));

		ReadyStatusText->SetText(StatusText);
	}
}