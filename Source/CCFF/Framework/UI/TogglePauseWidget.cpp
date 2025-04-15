#include "Framework/UI/TogglePauseWidget.h"  
#include "Framework/UI/ConfirmPopupWidget.h"
#include "Framework/UI/SettingsWidget.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/Controller/CharacterController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


UTogglePauseWidget::UTogglePauseWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

void UTogglePauseWidget::NativeConstruct()
{  
   Super::NativeConstruct();

   if (IsFocusable())
   {
	   if (IsValid(BackButton))
		   BackButton->OnClicked.AddDynamic(this, &UTogglePauseWidget::OnBackButtonClicked);

	   if (IsValid(SettingButton))
		   SettingButton->OnClicked.AddDynamic(this, &UTogglePauseWidget::OnSettingButtonClicked);

	   if (IsValid(LobbyButton))
		   LobbyButton->OnClicked.AddDynamic(this, &UTogglePauseWidget::OnLobbyButtonClicked);
   }
}

void UTogglePauseWidget::OnBackButtonClicked()
{
	ABaseInGameHUD* MyHUD = Cast<ABaseInGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (MyHUD)
	{
		UTogglePauseWidget* TogglePauseWidget = MyHUD->GetTogglePauseWidget();
		if (TogglePauseWidget)
		{
			TogglePauseWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (ACharacterController* PC = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		PC->bIsPause = false;
	}
}

void UTogglePauseWidget::OnSettingButtonClicked()  
{
	ABaseInGameHUD* MyHUD = Cast<ABaseInGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (MyHUD)
	{
		MyHUD->ShowSettingsWidget();
	}
}

void UTogglePauseWidget::OnLobbyButtonClicked()  
{
	if (!MoveLobbyPopup && MoveLobbyPopupClass)
	{
		MoveLobbyPopup = CreateWidget<UConfirmPopupWidget>(GetWorld(), MoveLobbyPopupClass);
		if (UConfirmPopupWidget* ConfirmPopup = Cast<UConfirmPopupWidget>(MoveLobbyPopup))
		{
			ConfirmPopup->SetMessage(FText::FromString(TEXT("로비 ㄱ?")));
			ConfirmPopup->AddToViewport(10);
			ConfirmPopup->OnConfirmPopupConfirmed.AddDynamic(this, &UTogglePauseWidget::OnMoveLobbyConfirmed);
			ConfirmPopup->OnConfirmPopupCanceled.AddDynamic(this, &UTogglePauseWidget::OnMoveLobbyCanceled);
		}
	}
}

void UTogglePauseWidget::OnMoveLobbyConfirmed()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("MainMenuMap")));
}

void UTogglePauseWidget::OnMoveLobbyCanceled()
{
	if (MoveLobbyPopup)
	{
		MoveLobbyPopup->SetVisibility(ESlateVisibility::Collapsed);
		MoveLobbyPopup = nullptr;
	}

	SetKeyboardFocus();
}
