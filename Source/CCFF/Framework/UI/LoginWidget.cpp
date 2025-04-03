#include "Framework/UI/LoginWidget.h"
#include "Components/EditableTextBox.h"
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
	

	if (!IsValid(NicknameTextBox))
	{
		UE_LOG(LogTemp, Error, TEXT("NicknameTextBox IS NULL in NativeConstruct"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NicknameTextBox bound successfully"));
	}

}

void ULoginWidget::OnConfirmClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Entered OnConfirmClicked"));

	if (!IsValid(NicknameTextBox))
	{
		UE_LOG(LogTemp, Error, TEXT("NicknameTextBox is NULL"));
		return;
	}

	FText RawText = NicknameTextBox->GetText();
	UE_LOG(LogTemp, Warning, TEXT("Got RawText"));

	FString Nickname = RawText.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Converted to FString: %s"), *Nickname);

	UCCFFGameInstance* GI = GetGameInstanceTyped();
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is NULL!"));
		return;
	}

	GI->SetNickname(Nickname);
	UE_LOG(LogTemp, Warning, TEXT("Nickname set"));

	GI->SaveData();
	UE_LOG(LogTemp, Warning, TEXT("SaveData called"));

	OnLoginSuccess.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Broadcast done"));
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
