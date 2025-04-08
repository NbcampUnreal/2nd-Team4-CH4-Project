#include "Framework/UI/LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/UI/ConfirmPopupWidget.h"
#include "Kismet/KismetSystemLibrary.h"

ULoginWidget::ULoginWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(NicknameText))
	{
		NicknameText.Get()->OnTextCommitted.AddDynamic(this, &ULoginWidget::OnTextCommitted);
	}

	if (IsValid(LoginButton))
	{
		LoginButton.Get()->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginButtonClicked);
	}

	if (IsValid(ExitButton))
	{
		ExitButton.Get()->OnClicked.AddDynamic(this, &ULoginWidget::OnExitButtonClicked);
	}
}

void ULoginWidget::OnLoginButtonClicked()
{
	if (IsValid(NicknameText) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginWidget] OnLoginButtonClicked : NicknameText is NULL"));
		return;
	}

	FString Nickname = NicknameText->GetText().ToString();

	FString ErrorMessage;
	if (!IsNicknameValid(Nickname, ErrorMessage))
	{
		ShowErrorPopup(ErrorMessage);
		return;
	}

	UCCFFGameInstance* GameInstance = Cast<UCCFFGameInstance>(GetGameInstance());
	if (IsValid(GameInstance) == true)
	{
		GameInstance->SetNickname(Nickname);
		GameInstance->SaveData();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginWidget] OnConfirmClicked : GameInstance is NULL!"));
		return;
	}

	OnLoginSuccess.Broadcast();
}

void ULoginWidget::OnExitButtonClicked()
{
	if (!ExitGamePopup && ExitGamePopupClass)
	{
		ExitGamePopup = CreateWidget<UConfirmPopupWidget>(GetWorld(), ExitGamePopupClass);
		if (ExitGamePopup)
		{
			ExitGamePopup->SetMessage(FText::FromString(TEXT("진짜 나감?")));
			ExitGamePopup->AddToViewport();
			ExitGamePopup->OnConfirmPopupConfirmed.AddDynamic(this, &ULoginWidget::HandleExitGameConfirmed);
			ExitGamePopup->OnConfirmPopupCanceled.AddDynamic(this, &ULoginWidget::HandleExitGameCanceled);
		}
	}
}

void ULoginWidget::HandleExitGameConfirmed()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
		}
	}
}

void ULoginWidget::HandleExitGameCanceled()
{
	if (ExitGamePopup)
	{
		ExitGamePopup->RemoveFromParent();
		ExitGamePopup = nullptr;
	}

	SetKeyboardFocus();
}

void ULoginWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnLoginButtonClicked();
	}
}

bool ULoginWidget::IsNicknameValid(const FString& Nickname, FString& OutErrorMessage) const
{
	if (Nickname.IsEmpty())
	{
		OutErrorMessage = TEXT("닉네임을 입력해주세요.");
		return false;
	}

	if (Nickname.Contains(TEXT(" ")))
	{
		OutErrorMessage = TEXT("닉네임에 공백은 사용할 수 없습니다.");
		return false;
	}

	const FRegexPattern InvalidCharPattern(TEXT("[^a-zA-Z0-9가-힣]"));
	FRegexMatcher Matcher(InvalidCharPattern, Nickname);
	if (Matcher.FindNext())
	{
		OutErrorMessage = TEXT("닉네임에 특수문자는 사용할 수 없습니다.");
		return false;
	}

	return true;
}

void ULoginWidget::ShowErrorPopup(const FString& Message)
{
	if (!ExitGamePopupClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginWidget] ShowErrorPopup : ExitGamePopupClass is NULL!"));
		return;
	}

	ExitGamePopup = CreateWidget<UConfirmPopupWidget>(GetWorld(), ExitGamePopupClass);
	if (ExitGamePopup)
	{
		ExitGamePopup->SetMessage(FText::FromString(Message));
		ExitGamePopup->AddToViewport();
		ExitGamePopup->OnConfirmPopupConfirmed.AddDynamic(this, &ULoginWidget::HandleExitGameCanceled); // 재입력 유도
		ExitGamePopup->OnConfirmPopupCanceled.AddDynamic(this, &ULoginWidget::HandleExitGameCanceled);
	}
}
