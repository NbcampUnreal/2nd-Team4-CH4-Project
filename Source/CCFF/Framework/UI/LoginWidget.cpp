#include "Framework/UI/LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Framework/Controller/TitlePlayerController.h"
#include "Framework/UI/ConfirmPopupWidget.h"
#include "Framework/UI/CheckPopupWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Framework/Application/SlateApplication.h"

ULoginWidget::ULoginWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(LoginButton) && !LoginButton->OnClicked.IsAlreadyBound(this, &ULoginWidget::OnLoginButtonClicked))
	{
		LoginButton.Get()->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginButtonClicked);
	}

	if (IsValid(ExitButton) && !ExitButton.Get()->OnClicked.IsAlreadyBound(this, &ULoginWidget::OnExitButtonClicked))
	{
		ExitButton.Get()->OnClicked.AddDynamic(this, &ULoginWidget::OnExitButtonClicked);
	}

	if (IsValid(IDText))
	{
		FTimerHandle FocusTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FocusTimerHandle, [this]()
			{
				if (IDText)
				{
					IDText->SetKeyboardFocus();
				}
			}, 0.1f, false);
	}

	if (IsValid(PasswordText) && !PasswordText->OnTextCommitted.IsAlreadyBound(this, &ULoginWidget::HandlePasswordCommitted))
	{
		PasswordText->OnTextCommitted.AddDynamic(this, &ULoginWidget::HandlePasswordCommitted);
	}
}

FReply ULoginWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Virtual_Accept)
	{
		OnLoginButtonClicked();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULoginWidget::HandlePasswordCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnLoginButtonClicked();
	}
}

void ULoginWidget::OnLoginButtonClicked()
{
	if (!IsValid(IDText) || !IsValid(PasswordText))
	{
		ShowErrorPopup(TEXT("ID 또는 비밀번호 입력 UI가 없습니다."));
		return;
	}

	const FString ID = IDText->GetText().ToString().TrimStartAndEnd();
	const FString Password = PasswordText->GetText().ToString().TrimStartAndEnd();

	if (ID.IsEmpty() || Password.IsEmpty())
	{
		ShowErrorPopup(TEXT("ID와 비밀번호를 모두 입력해주세요."));
		return;
	}

	UCCFFGameInstance* GameInstance = Cast<UCCFFGameInstance>(GetGameInstance());
	if (IsValid(GameInstance) == true)
	{
		if (GameInstance->TryLogin(ID, Password))
		{
			OnLoginSuccess.Broadcast();
			return;
		}
		else
		{
			ShowErrorPopup(TEXT("ID 또는 비밀번호가 올바르지 않습니다."));
		}
	}
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

			if (!ExitGamePopup->OnConfirmPopupConfirmed.IsAlreadyBound(this, &ULoginWidget::HandleExitGameConfirmed))
			{
				ExitGamePopup->OnConfirmPopupConfirmed.AddDynamic(this, &ULoginWidget::HandleExitGameConfirmed);
			}
			if (!ExitGamePopup->OnConfirmPopupCanceled.IsAlreadyBound(this, &ULoginWidget::HandleExitGameCanceled))
			{
				ExitGamePopup->OnConfirmPopupCanceled.AddDynamic(this, &ULoginWidget::HandleExitGameCanceled);
			}
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

	if (IDText)
	{
		IDText->SetKeyboardFocus();
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
	if (!ErrorPopupClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[LoginWidget] ShowErrorPopup : ErrorPopupClass is NULL!"));
		return;
	}

	ErrorPopup = CreateWidget<UCheckPopupWidget>(GetWorld(), ErrorPopupClass);
	if (ErrorPopup)
	{
		ErrorPopup->SetMessage(FText::FromString(Message));
		ErrorPopup->AddToViewport();
		ErrorPopup->SetKeyboardFocus();

		ErrorPopup->OnCheckPopupConfirmed.AddDynamic(this, &ULoginWidget::HandleErrorPopupClosed);
	}
}

void ULoginWidget::HandleErrorPopupClosed()
{
	ErrorPopup = nullptr;
	if (IDText)
	{
		IDText->SetKeyboardFocus();
	}
}
