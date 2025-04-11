#include "Framework/UI/LockerRoomWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Base/BasePreviewPawn.h"
#include "Items/Structure/CustomizationPreset.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Items/DataTable/CustomItemData.h"

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

	InitializePreviewPawn();
}

void ULockerRoomWidget::InitializePreviewPawn()
{
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABasePreviewPawn::StaticClass());
    if (FoundActor)
    {
        PreviewPawn = Cast<APawn>(FoundActor);
        UE_LOG(LogTemp, Log, TEXT("BasePreviewPawn found: %s"), *PreviewPawn->GetName());
    }
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
}

void ULockerRoomWidget::OnRightPresetButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Right Preset Button Clicked"));
}

void ULockerRoomWidget::OnLeftHeadButtonClicked()
{
	CurrentHeadIndex = FMath::Clamp(CurrentHeadIndex - 1, 0, 3);

    UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
    if (CustomizationComp)
    {
        CustomizationComp->EquipItemByID(CurrentHeadIndex, EItemSlot::Head);
    }
}

void ULockerRoomWidget::OnRightHeadButtonClicked()
{
    CurrentHeadIndex = FMath::Clamp(CurrentHeadIndex + 1, 0, 3);

    UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
    if (CustomizationComp)
    {
        CustomizationComp->EquipItemByID(CurrentHeadIndex, EItemSlot::Head);
    }
}

void ULockerRoomWidget::OnLeftFaceButtonClicked()
{
	CurrentFaceIndex = FMath::Clamp(CurrentFaceIndex - 1, 0, 3);
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->EquipItemByID(CurrentFaceIndex, EItemSlot::Face);
	}
}

void ULockerRoomWidget::OnRightFaceButtonClicked()
{
	CurrentFaceIndex = FMath::Clamp(CurrentFaceIndex + 1, 0, 3);
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->EquipItemByID(CurrentFaceIndex, EItemSlot::Face);
	}
}

void ULockerRoomWidget::OnLeftShoulderButtonClicked()
{
	CurrentShoulderIndex = FMath::Clamp(CurrentShoulderIndex - 1, 0, 2);
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->EquipItemByID(CurrentShoulderIndex, EItemSlot::Shoulder);
	}
}

void ULockerRoomWidget::OnRightShoulderButtonClicked()
{
	CurrentShoulderIndex = FMath::Clamp(CurrentShoulderIndex + 1, 0, 2);
	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->EquipItemByID(CurrentShoulderIndex, EItemSlot::Shoulder);
	}
}

void ULockerRoomWidget::OnSaveButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Save Button Clicked"));

	FPresetItemsindex ItemsIndex;

	ItemsIndex.PresetIndex = CurrentPresetIndex;
	ItemsIndex.HeadIndex = CurrentHeadIndex;
	ItemsIndex.FaceIndex = CurrentFaceIndex;
	ItemsIndex.ShoulderIndex = CurrentShoulderIndex;
	
	SaveCurrentPreset(ItemsIndex);
}

void ULockerRoomWidget::SaveCurrentPreset(FPresetItemsindex& Indexes)
{
	UE_LOG(LogTemp, Log, TEXT("Saving current preset..."));

	UCharacterCustomizationComponent* CustomizationComp = PreviewPawn->FindComponentByClass<UCharacterCustomizationComponent>();
	if (CustomizationComp)
	{
		CustomizationComp->SavePreset(Indexes);
	} 
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
}