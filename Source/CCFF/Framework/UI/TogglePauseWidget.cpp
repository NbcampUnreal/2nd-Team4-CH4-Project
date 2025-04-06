#include "Framework/UI/TogglePauseWidget.h"  
#include "Components/Button.h"  
#include "Kismet/GameplayStatics.h"
#include "Framework/HUD/BaseInGameHUD.h"
#include "Framework/UI/SettingsWidget.h"
#include "Character/Base/CharacterController.h"


void UTogglePauseWidget::NativeConstruct()
{  
   Super::NativeConstruct();

   if (IsValid(BackButton))
	   BackButton->OnClicked.AddDynamic(this, &UTogglePauseWidget::OnBackButtonClicked);

   if (IsValid(SettingButton))
	   SettingButton->OnClicked.AddDynamic(this, &UTogglePauseWidget::OnSettingButtonClicked);

   if (IsValid(LobbyButton))
	   LobbyButton->OnClicked.AddDynamic(this, &UTogglePauseWidget::OnLobbyButtonClicked);
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
	UGameplayStatics::OpenLevel(this, FName(TEXT("MainMenuMap")));
}
