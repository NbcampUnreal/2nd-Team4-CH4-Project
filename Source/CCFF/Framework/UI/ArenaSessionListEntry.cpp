#include "Framework/UI/ArenaSessionListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Framework/Controller/MainMenuPlayerController.h"

void UArenaSessionListEntry::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (JoinButton && !JoinButton->OnClicked.IsAlreadyBound(this, &UArenaSessionListEntry::OnJoinButtonClicked))
	{
		JoinButton->OnClicked.AddDynamic(this, &UArenaSessionListEntry::OnJoinButtonClicked);
	}
}

void UArenaSessionListEntry::InitializeSessionEntry(const FSessionInfo& InSessionInfo)
{
	CachedSession = InSessionInfo;

	if (IsValid(SessionNameText) == true)
	{
		SessionNameText->SetText(FText::FromString(InSessionInfo.SessionName));
	}

	if (IsValid(PlayerCountText) == true)
	{
		const FString CountStr = FString::Printf(TEXT("%d / %d"), InSessionInfo.CurrentPlayers, InSessionInfo.MaxPlayers);
		PlayerCountText->SetText(FText::FromString(CountStr));
	}
}

void UArenaSessionListEntry::OnJoinButtonClicked()
{
	if (OnJoinSessionRequested.IsBound())
	{
		OnJoinSessionRequested.Execute(CachedSession);
	}
}
