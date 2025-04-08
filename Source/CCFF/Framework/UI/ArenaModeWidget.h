#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "ArenaModeWidget.generated.h"

DECLARE_DELEGATE(FOnBackToSelectMode);

UCLASS()
class CCFF_API UArenaModeWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void NativeDestruct();
	
	UFUNCTION()
	void UpdateSessionList(const TArray<struct FSessionInfo>& FoundSessions);
	
	void ClearSessionList();

	FOnBackToSelectMode OnBackToSelectMode;

protected:
	UFUNCTION()
	void OnRefreshButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void HandleJoinSession(const struct FSessionInfo& SessionInfo);

	void RequestFindSessions();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> RefreshButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BackButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> SessionListBox;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UArenaSessionListEntry> SessionEntryClass;

};
