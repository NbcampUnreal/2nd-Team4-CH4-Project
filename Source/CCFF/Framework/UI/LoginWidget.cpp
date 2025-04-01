#include "Framework/UI/LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/GameInstance/CCFFGameInstance.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &ULoginWidget::OnConfirmClicked);
	}

	if (NicknameTextBox)
	{
		NicknameTextBox->OnTextCommitted.AddDynamic(this, &ULoginWidget::OnTextCommitted);
	}
}

void ULoginWidget::OnConfirmClicked()
{
	if (!NicknameTextBox) return;

	FString Nick = NicknameTextBox->GetText().ToString();

	if (!Nick.IsEmpty())
	{
		GetGameInstanceTyped()->SetNickname(Nick);
		GetGameInstanceTyped()->SaveData();
		UGameplayStatics::OpenLevel(this, TEXT("MainMenuMap"));
	}
}

void ULoginWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnConfirmClicked();
	}
}
