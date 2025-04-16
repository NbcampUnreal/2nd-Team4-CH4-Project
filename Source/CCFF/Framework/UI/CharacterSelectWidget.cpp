#include "Framework/UI/CharacterSelectWidget.h"
#include "Components/Button.h"
#include "Framework/Application/SlateApplication.h" 

void UCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CharacterButton0 && !CharacterButton0->OnClicked.IsAlreadyBound(this, &UCharacterSelectWidget::HandleCharacterClick_0))
	{
		CharacterButton0->OnClicked.AddDynamic(this, &UCharacterSelectWidget::HandleCharacterClick_0);
	}
	if (CharacterButton1 && !CharacterButton1->OnClicked.IsAlreadyBound(this, &UCharacterSelectWidget::HandleCharacterClick_1))
	{
		CharacterButton1->OnClicked.AddDynamic(this, &UCharacterSelectWidget::HandleCharacterClick_1);
	}
	if (CharacterButton2 && !CharacterButton2->OnClicked.IsAlreadyBound(this, &UCharacterSelectWidget::HandleCharacterClick_2))
	{
		CharacterButton2->OnClicked.AddDynamic(this, &UCharacterSelectWidget::HandleCharacterClick_2);
	}
	if (CharacterButton3 && !CharacterButton3->OnClicked.IsAlreadyBound(this, &UCharacterSelectWidget::HandleCharacterClick_3))
	{
		CharacterButton3->OnClicked.AddDynamic(this, &UCharacterSelectWidget::HandleCharacterClick_3);
	}
	if (CharacterButton4 && !CharacterButton4->OnClicked.IsAlreadyBound(this, &UCharacterSelectWidget::HandleCharacterClick_4))
	{
		CharacterButton4->OnClicked.AddDynamic(this, &UCharacterSelectWidget::HandleCharacterClick_4);
	}
	if (CharacterButton5 && !CharacterButton5->OnClicked.IsAlreadyBound(this, &UCharacterSelectWidget::HandleCharacterClick_5))
	{
		CharacterButton5->OnClicked.AddDynamic(this, &UCharacterSelectWidget::HandleCharacterClick_5);
	}
}

void UCharacterSelectWidget::HandleCharacterClick_0() { BroadcastSelection(0); }
void UCharacterSelectWidget::HandleCharacterClick_1() { BroadcastSelection(1); }
void UCharacterSelectWidget::HandleCharacterClick_2() { BroadcastSelection(2); }
void UCharacterSelectWidget::HandleCharacterClick_3() { BroadcastSelection(3); }
void UCharacterSelectWidget::HandleCharacterClick_4() { BroadcastSelection(4); }
void UCharacterSelectWidget::HandleCharacterClick_5() { BroadcastSelection(5); }

void UCharacterSelectWidget::BroadcastSelection(int32 Index)
{
	if (!CharacterIDList.IsValidIndex(Index)) return;

	const FName SelectedID = CharacterIDList[Index];
	OnCharacterSelected.ExecuteIfBound(SelectedID);

	FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::SetDirectly);

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}
