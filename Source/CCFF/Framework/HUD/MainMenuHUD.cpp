#include "Framework/HUD/MainMenuHUD.h"
#include "Framework/UI/MainMenuWidget.h"
#include "Framework/UI/ArenaModeWidget.h"
#include "Framework/UI/SelectModeWidget.h"
#include "Framework/UI/LockerRoomWidget.h"
#include "Framework/UI/SettingsWidget.h"
#include "Framework/UI/ConfirmPopupWidget.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenuWidget();
}

void AMainMenuHUD::PushWidget(UUserWidget* WidgetClass)
{
	if (WidgetStack.Num() > 0 && WidgetStack.Last())
	{
		WidgetStack.Last()->RemoveFromParent();
	}

	if (WidgetClass)
	{
		WidgetClass->AddToViewport();
		WidgetStack.Add(WidgetClass);
	}
}

void AMainMenuHUD::PopWidget()
{
	if (WidgetStack.Num() == 0)
	{
		return;
	}

	UUserWidget* Top = WidgetStack.Pop();
	if (IsValid(Top))
	{
		Top->RemoveFromParent();
	}

	if (WidgetStack.Num() > 0 && IsValid(WidgetStack.Last()))
	{
		WidgetStack.Last()->AddToViewport();
	}
}

void AMainMenuHUD::ReturnToPreviousWidget()
{
	PopWidget();
}

void AMainMenuHUD::ShowMainMenuWidget()
{
	if (!MainMenuWidget && MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	}

	PushWidget(MainMenuWidget);
}

void AMainMenuHUD::ShowSelectModeWidget()
{
	if (!SelectModeWidget && SelectModeWidgetClass)
	{
		SelectModeWidget = CreateWidget<USelectModeWidget>(GetWorld(), SelectModeWidgetClass);
		
		if (!SelectModeWidget->OnArenaModeRequested.IsAlreadyBound(this, &AMainMenuHUD::ShowArenaWidget))
		{
			SelectModeWidget->OnArenaModeRequested.AddDynamic(this, &AMainMenuHUD::ShowArenaWidget);
		}
	}

	PushWidget(SelectModeWidget);
}

void AMainMenuHUD::ShowArenaWidget()
{
	if (!ArenaModeWidget && ArenaModeWidgetClass)
	{
		ArenaModeWidget = CreateWidget<UArenaModeWidget>(GetWorld(), ArenaModeWidgetClass);
		
		if (!ArenaModeWidget->OnBackToSelectMode.IsBoundToObject(this))
		{
			ArenaModeWidget->OnBackToSelectMode.BindUObject(this, &AMainMenuHUD::ReturnToPreviousWidget);
		}
	}

	PushWidget(ArenaModeWidget);
}

void AMainMenuHUD::ShowLockerRoomWidget()
{
	if (LockerRoomWidgetClass)
	{
		if (!LockerRoomWidget)
		{
			LockerRoomWidget = CreateWidget<ULockerRoomWidget>(GetWorld(), LockerRoomWidgetClass);
		}

		if (!LockerRoomWidget->OnBackToMainMenuRequested.IsBoundToObject(this))
		{
			LockerRoomWidget->OnBackToMainMenuRequested.BindUObject(this, &AMainMenuHUD::ReturnToPreviousWidget);
		}

		PushWidget(LockerRoomWidget);
	}
}

void AMainMenuHUD::ShowShopWidget()
{
	// TODO : 
}

void AMainMenuHUD::ShowSettingsWidget()
{
	if (SettingsWidgetClass)
	{
		if (!SettingsWidget)
		{
			SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsWidgetClass);
		}

		SettingsWidget->AddToViewport(10);
	}
}

void AMainMenuHUD::HideSettingsWidget()
{
	if (SettingsWidget)
	{
		SettingsWidget->RemoveFromParent();
		SettingsWidget = nullptr;
	}
}

void AMainMenuHUD::ShowLoadingWidget()
{
	if (!LoadingWidget && LoadingWidgetClass)
	{
		LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
	}

	if (LoadingWidget)
	{
		LoadingWidget->AddToViewport(100);
	}
}

void AMainMenuHUD::HideLoadingWidget()
{
	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromParent();
		LoadingWidget = nullptr;
	}
}

void AMainMenuHUD::ShowErrorPopup(const FText& Message)
{
	if (!ErrorPopup && ErrorPopupClass)
	{
		ErrorPopup = CreateWidget<UConfirmPopupWidget>(GetWorld(), ErrorPopupClass);
	}

	if (ErrorPopup)
	{
		ErrorPopup->SetMessage(Message);
		ErrorPopup->AddToViewport(200);
		ErrorPopup->OnConfirmPopupConfirmed.AddDynamic(this, &AMainMenuHUD::HandleErrorPopupConfirmed);
	}
}

void AMainMenuHUD::HandleErrorPopupConfirmed()
{
	if (ErrorPopup)
	{
		ErrorPopup->RemoveFromParent();
		ErrorPopup = nullptr;
	}
}
