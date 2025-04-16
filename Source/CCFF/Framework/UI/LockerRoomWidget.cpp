#include "Framework/UI/LockerRoomWidget.h"
#include "Character/Base/BasePreviewPawn.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Items/DataTable/CustomItemData.h"
#include "Items/Manager/CustomizationManager.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Kismet/GameplayStatics.h"

void ULockerRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton && !BackButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::RequestReturnToMainMenu))
	{
		BackButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::RequestReturnToMainMenu);
	}

    if (L_PresetButton && !L_PresetButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnLeftPresetButtonClicked))
    {
        L_PresetButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnLeftPresetButtonClicked);
    }
    if (R_PresetButton && !R_PresetButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnRightPresetButtonClicked))
    {
        R_PresetButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnRightPresetButtonClicked);
    }

    if (L_HeadButton && !L_HeadButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnLeftHeadButtonClicked))
    {
        L_HeadButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnLeftHeadButtonClicked);
    }
    if (R_HeadButton && !R_HeadButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnRightHeadButtonClicked))
    {
        R_HeadButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnRightHeadButtonClicked);
    }

    if (L_FaceButton && !L_FaceButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnLeftFaceButtonClicked))
    {
        L_FaceButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnLeftFaceButtonClicked);
    }
    if (R_FaceButton && !R_FaceButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnRightFaceButtonClicked))
    {
        R_FaceButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnRightFaceButtonClicked);
    }

    if (L_ShoulderButton && !L_ShoulderButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnLeftShoulderButtonClicked))
    {
        L_ShoulderButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnLeftShoulderButtonClicked);
    }
    if (R_ShoulderButton && !R_ShoulderButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnRightShoulderButtonClicked))
    {
        R_ShoulderButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnRightShoulderButtonClicked);
    }

	if (SaveButton && !SaveButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnSaveButtonClicked))
	{
		SaveButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnSaveButtonClicked);
	}
	if (ClearButton && !ClearButton->OnClicked.IsAlreadyBound(this, &ULockerRoomWidget::OnClearButtonClicked))
	{
		ClearButton->OnClicked.AddDynamic(this, &ULockerRoomWidget::OnClearButtonClicked);
	}
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetWidgetToFocus(TakeWidget());

		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}

	InitializeCustomizationManager();
	InitializePreviewPawn();
	BindToCharacterChangedEvent();
}

void ULockerRoomWidget::InitializePreviewPawn()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABasePreviewPawn::StaticClass());
	if (FoundActor)
	{
		APawn* PawnActor = Cast<APawn>(FoundActor);
		PreviewPawn = Cast<ABasePreviewPawn>(FoundActor);
		UE_LOG(LogTemp, Log, TEXT("BasePreviewPawn found: %s"), *PreviewPawn->GetName());

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->Possess(PreviewPawn);
			UE_LOG(LogTemp, Log, TEXT("PlayerController now possesses %s"), *PreviewPawn->GetName());
		}
	}
}

void ULockerRoomWidget::InitializeCustomizationManager()
{
	CustomizationManager = GetGameInstance()->GetSubsystem<UCustomizationManager>();
	if (CustomizationManager)
	{
		UE_LOG(LogTemp, Log, TEXT("CustomizationManager initialized successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to initialize CustomizationManager."));
	}

	CountTotalItemNums();
}

void ULockerRoomWidget::RequestReturnToMainMenu()
{
	if (OnBackToMainMenuRequested.IsBound())
	{
		OnBackToMainMenuRequested.Execute();
	}
}


void ULockerRoomWidget::OnLeftPresetButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Left Preset Button Clicked"));
	CurrentPresetIndex = FMath::Clamp(CurrentPresetIndex - 1, 0, 2);
	UpdatePresetNumberText();
	LoadPreset(CurrentPresetIndex);
}

void ULockerRoomWidget::OnRightPresetButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Right Preset Button Clicked"));
	CurrentPresetIndex = FMath::Clamp(CurrentPresetIndex + 1, 0, 2);
	UpdatePresetNumberText();
	LoadPreset(CurrentPresetIndex);
}

void ULockerRoomWidget::OnLeftHeadButtonClicked()
{
	CurrentHeadIndex = FMath::Clamp(CurrentHeadIndex - 1, 0, TotalHeadNums - 1);

    UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
    if (CustomizationComp)
    {
        CustomizationComp->EquipItemByID(CurrentHeadIndex, EItemSlot::Head);
    }

	UpdateHeadNameText();
}

void ULockerRoomWidget::OnRightHeadButtonClicked()
{
	CurrentHeadIndex = FMath::Clamp(CurrentHeadIndex + 1, 0, TotalHeadNums - 1);

    UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
    if (CustomizationComp)
    {
        CustomizationComp->EquipItemByID(CurrentHeadIndex, EItemSlot::Head);
    }

	UpdateHeadNameText();
}

void ULockerRoomWidget::OnLeftFaceButtonClicked()
{
	CurrentFaceIndex = FMath::Clamp(CurrentFaceIndex - 1, 0, TotalFaceNums - 1);
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->EquipItemByID(CurrentFaceIndex, EItemSlot::Face);
	}

	UpdateFaceNameText();
}

void ULockerRoomWidget::OnRightFaceButtonClicked()
{
	CurrentFaceIndex = FMath::Clamp(CurrentFaceIndex + 1, 0, TotalFaceNums - 1);
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->EquipItemByID(CurrentFaceIndex, EItemSlot::Face);
	}

	UpdateFaceNameText();
}

void ULockerRoomWidget::OnLeftShoulderButtonClicked()
{
	CurrentShoulderIndex = FMath::Clamp(CurrentShoulderIndex - 1, 0, TotalShoulderNums - 1);
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->EquipItemByID(CurrentShoulderIndex, EItemSlot::Shoulder);
	}

	UpdateShoulderNameText();
}

void ULockerRoomWidget::OnRightShoulderButtonClicked()
{
	CurrentShoulderIndex = FMath::Clamp(CurrentShoulderIndex + 1, 0, TotalShoulderNums - 1);
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->EquipItemByID(CurrentShoulderIndex, EItemSlot::Shoulder);
	}

	UpdateShoulderNameText();
}

void ULockerRoomWidget::OnSaveButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Save Button Clicked"));

	FPresetItemsIndex ItemsIndex;

	ItemsIndex.PresetIndex = CurrentPresetIndex;
	ItemsIndex.HeadIndex = CurrentHeadIndex;
	ItemsIndex.FaceIndex = CurrentFaceIndex;
	ItemsIndex.ShoulderIndex = CurrentShoulderIndex;
	
	UE_LOG(LogTemp, Log, TEXT("Saving current preset..."));

	CustomizationManager->SavePreset(ItemsIndex);
}

void ULockerRoomWidget::OnClearButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Clear Button Clicked"));
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->UnequipAllItems();
	}

	CurrentHeadIndex = 0;
	CurrentFaceIndex = 0;
	CurrentShoulderIndex = 0;

	UpdateHeadNameText();
	UpdateFaceNameText();
	UpdateShoulderNameText();
}

void ULockerRoomWidget::UpdatePresetNumberText()
{
	if (PresetNumberText)
	{
		FString PresetText = FString::Printf(TEXT("Preset %d"), CurrentPresetIndex);
		PresetNumberText->SetText(FText::FromString(PresetText));
	}
}

void ULockerRoomWidget::UpdateHeadNameText()
{
	if (HeadNameText)
	{
		FString HeadName = CustomizationManager->GetItemName(CurrentHeadIndex, EItemSlot::Head).ToString();
		HeadNameText->SetText(FText::FromString(HeadName));
	}
}

void ULockerRoomWidget::UpdateFaceNameText()
{
	if (FaceNameText)
	{
		FString FaceName = CustomizationManager->GetItemName(CurrentFaceIndex, EItemSlot::Face).ToString();
		FaceNameText->SetText(FText::FromString(FaceName));
	}
}

void ULockerRoomWidget::UpdateShoulderNameText()
{
	if (ShoulderNameText)
	{
		FString ShoulderName = CustomizationManager->GetItemName(CurrentShoulderIndex, EItemSlot::Shoulder).ToString();
		ShoulderNameText->SetText(FText::FromString(ShoulderName));
	}
}

void ULockerRoomWidget::UpdateIndexText()
{
	CurrentHeadIndex = CustomizationManager->GetAppliedPresetIndexes().HeadIndex;
	CurrentFaceIndex = CustomizationManager->GetAppliedPresetIndexes().FaceIndex;
	CurrentShoulderIndex = CustomizationManager->GetAppliedPresetIndexes().ShoulderIndex;

	UpdateHeadNameText();
	UpdateFaceNameText();
	UpdateShoulderNameText();
}

void ULockerRoomWidget::LoadPreset(int32 PresetIndex)
{
	if (CustomizationManager)
	{
		CustomizationManager->LoadPreset(PreviewPawn->GetCharacterID(), PresetIndex);
		UpdateIndexText();
	}
}

void ULockerRoomWidget::CountTotalItemNums()
{
	TotalHeadNums = CustomizationManager->GetTotalHeadItems();
	TotalFaceNums = CustomizationManager->GetTotalFaceItems();
	TotalShoulderNums = CustomizationManager->GetTotalShoulderItems();
}

void ULockerRoomWidget::HandleCharacterChanged()
{
	UE_LOG(LogTemp, Log, TEXT("Character changed in LockerRoomWidget"));

	LoadPreset(CurrentPresetIndex);
}

void ULockerRoomWidget::BindToCharacterChangedEvent()
{
	if (PreviewPawn)
	{
		PreviewPawn->OnCharacterChanged.BindUObject(this, &ULockerRoomWidget::HandleCharacterChanged);
	}
}