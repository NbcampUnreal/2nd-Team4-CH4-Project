#include "Framework/UI/LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
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
	if (!NicknameTextBox)
	{
		return;
	}

	FString Nickname = NicknameTextBox->GetText().ToString();

	if (!Nickname.IsEmpty())
	{
		GetGameInstanceTyped()->SetNickname(Nickname);
		GetGameInstanceTyped()->SaveData();

		OnLoginSuccess.Broadcast();
	}
}

void ULoginWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnConfirmClicked();
	}
}

UCCFFGameInstance* ULoginWidget::GetGameInstanceTyped() const
{
	return Cast<UCCFFGameInstance>(GetGameInstance());
}
