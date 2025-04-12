#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "CharacterSelectWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnCharacterSelected, FName);

UCLASS()
class CCFF_API UCharacterSelectWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FOnCharacterSelected OnCharacterSelected;

protected:
	// Per-button handlers
	UFUNCTION() void HandleCharacterClick_0();
	UFUNCTION() void HandleCharacterClick_1();
	UFUNCTION() void HandleCharacterClick_2();
	UFUNCTION() void HandleCharacterClick_3();
	UFUNCTION() void HandleCharacterClick_4();
	UFUNCTION() void HandleCharacterClick_5();

	void BroadcastSelection(int32 Index);

	// Character selection buttons
	UPROPERTY(meta = (BindWidget))	class UButton* CharacterButton0;
	UPROPERTY(meta = (BindWidget))	class UButton* CharacterButton1;
	UPROPERTY(meta = (BindWidget))	class UButton* CharacterButton2;
	UPROPERTY(meta = (BindWidget))	class UButton* CharacterButton3;
	UPROPERTY(meta = (BindWidget))	class UButton* CharacterButton4;
	UPROPERTY(meta = (BindWidget))	class UButton* CharacterButton5;

	UPROPERTY(EditAnywhere, Category = "Character")
	TArray<FName> CharacterIDList;

};
