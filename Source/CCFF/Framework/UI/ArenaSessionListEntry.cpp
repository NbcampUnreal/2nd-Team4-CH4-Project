#include "Framework/UI/ArenaSessionListEntry.h"
#include "Components/EditableTextBox.h"
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

	if (ServerIPEditableText && !ServerIPEditableText->OnTextChanged.IsAlreadyBound(this, &UArenaSessionListEntry::OnIPAddressTextChanged))
	{
		ServerIPEditableText->OnTextChanged.AddDynamic(this, &UArenaSessionListEntry::OnIPAddressTextChanged);
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

	if (IsValid(ServerIPEditableText) == true)
	{
		const FString InitialIP = !InSessionInfo.IPAddress.IsEmpty()
			? InSessionInfo.IPAddress
			: TEXT("127.0.0.1:7777");
		ServerIPEditableText->SetText(FText::FromString(InitialIP));
	}
}

void UArenaSessionListEntry::OnJoinButtonClicked()
{
	if (OnJoinSessionRequested.IsBound())
	{
		OnJoinSessionRequested.Execute(CachedSession);
	}
}

void UArenaSessionListEntry::OnIPAddressTextChanged(const FText& NewText)
{
	CachedSession.IPAddress = NewText.ToString();
}