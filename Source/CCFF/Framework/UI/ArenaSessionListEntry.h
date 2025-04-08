#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Framework/Online/SessionDataStruct.h"
#include "ArenaSessionListEntry.generated.h"

DECLARE_DELEGATE_OneParam(FOnJoinSessionRequested, const FSessionInfo&);

UCLASS()
class CCFF_API UArenaSessionListEntry : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeSessionEntry(const FSessionInfo& InSessionInfo);

	FOnJoinSessionRequested OnJoinSessionRequested;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnJoinButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerCountText;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	FSessionInfo CachedSession;

};
