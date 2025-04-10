#include "Framework/UI/Lobby/PreviewReadyStatusWidget.h"
#include "Components/TextBlock.h"

void UPreviewReadyStatusWidget::SetReadyState(bool bIsReady)
{
	UE_LOG(LogTemp, Warning, TEXT("[UPreviewReadyStatusWidget] SetReadyState : ReadyState changed: %s"), bIsReady ? TEXT("Ready") : TEXT("Not Ready"));

	if (IsValid(ReadyStatusText))
	{
		const FText StatusText = bIsReady
			? FText::FromString(TEXT("Ready"))
			: FText::FromString(TEXT("Not Ready"));

		ReadyStatusText->SetText(StatusText);
		UE_LOG(LogTemp, Warning, TEXT("[UPreviewReadyStatusWidget] Text set successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UPreviewReadyStatusWidget] ReadyStatusText is NULL!"));
	}
}