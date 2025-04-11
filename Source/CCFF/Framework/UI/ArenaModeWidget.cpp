#include "Framework/UI/ArenaModeWidget.h"
#include "Components/Button.h"
#include "Components/Scrollbox.h"
#include "Components/EditableTextBox.h"
#include "Framework/UI/ArenaSessionListEntry.h"
#include "Framework/Online/SessionDataStruct.h"
#include "Framework/Controller/MainMenuPlayerController.h"
#include "Framework/GameInstance/CCFFGameInstance.h"

void UArenaModeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RefreshButton && !RefreshButton->OnClicked.IsAlreadyBound(this, &UArenaModeWidget::OnRefreshButtonClicked))
	{
		RefreshButton->OnClicked.AddDynamic(this, &UArenaModeWidget::OnRefreshButtonClicked);
	}

	if (BackButton && !BackButton->OnClicked.IsAlreadyBound(this, &UArenaModeWidget::OnBackButtonClicked))
	{
		BackButton->OnClicked.AddDynamic(this, &UArenaModeWidget::OnBackButtonClicked);
	}

	UCCFFGameInstance* GameInstance = GetGameInstance<UCCFFGameInstance>();
	if (GameInstance)
	{
		if (!GameInstance->OnSessionsFound.IsAlreadyBound(this, &UArenaModeWidget::UpdateSessionList))
		{
			GameInstance->OnSessionsFound.AddDynamic(this, &UArenaModeWidget::UpdateSessionList);
		}
	}

	RequestFindSessions();
}

void UArenaModeWidget::NativeDestruct()
{
	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &UArenaModeWidget::OnBackButtonClicked);
	}

	Super::NativeDestruct();
}

void UArenaModeWidget::OnRefreshButtonClicked()
{
	RequestFindSessions();
}

void UArenaModeWidget::RequestFindSessions()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		AMainMenuPlayerController* MainMenuPlayerController = Cast<AMainMenuPlayerController>(PlayerController);
		if (IsValid(MainMenuPlayerController))
		{
			MainMenuPlayerController->FindSessions();
		}
	}
}

void UArenaModeWidget::OnBackButtonClicked()
{
	if (OnBackToSelectMode.IsBound())
	{
		OnBackToSelectMode.Execute();
	}
}

void UArenaModeWidget::HandleJoinSession(const FSessionInfo& SessionInfo)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		AMainMenuPlayerController* MainMenuPlayerController = Cast<AMainMenuPlayerController>(PlayerController);
		if (IsValid(MainMenuPlayerController))
		{
			MainMenuPlayerController->JoinServer(SessionInfo.IPAddress);
		}
	}
}

void UArenaModeWidget::UpdateSessionList(const TArray<struct FSessionInfo>& FoundSessions)
{
	ClearSessionList();

	UCCFFGameInstance* GameInstance = GetGameInstance<UCCFFGameInstance>();
	const FString DefaultIP = GameInstance ? GameInstance->GetServerIP() : TEXT("127.0.0.1:7777");

	if (IsValid(SessionListBox) == false || IsValid(SessionEntryClass) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UArenaModeWidget] UpdateSessionList : SessionListBox or SessionEntryClass is NULL!"));
		return;
	}

	for (FSessionInfo Session : FoundSessions)
	{
		if (Session.IPAddress.IsEmpty())
		{
			Session.IPAddress = DefaultIP;
		}

		UArenaSessionListEntry* Entry = CreateWidget<UArenaSessionListEntry>(this, SessionEntryClass);
		if (IsValid(Entry) == true)
		{
			Entry->InitializeSessionEntry(Session);
			Entry->OnJoinSessionRequested.BindUObject(this, &UArenaModeWidget::HandleJoinSession);
			SessionListBox->AddChild(Entry);
		}
	}
}

void UArenaModeWidget::ClearSessionList()
{
	if (IsValid(SessionListBox))
	{
		SessionListBox->ClearChildren();
	}
}
