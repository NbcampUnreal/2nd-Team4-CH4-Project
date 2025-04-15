#include "Framework/UI/BaseUserWidget.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

void UBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BindAllButtonsWithClickSound();
}

void UBaseUserWidget::PlayClickSound()
{
	if (ClickSound && GetWorld())
	{
		UE_LOG(LogTemp, Log, TEXT("[UBaseUserWidget] PlayClickSound called"));
		UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBaseUserWidget] ClickSound or World is NULL"));
	}
}

void UBaseUserWidget::BindAllButtonsWithClickSound()
{
	if (!WidgetTree) return;

	TArray<UWidget*> AllWidgets;
	WidgetTree->GetAllWidgets(AllWidgets);

	for (UWidget* Widget : AllWidgets)
	{
		if (UButton* Button = Cast<UButton>(Widget))
		{
			// 이미 바인딩되어 있지 않은 경우에만 추가
			if (!Button->OnClicked.IsAlreadyBound(this, &UBaseUserWidget::PlayClickSound))
			{
				Button->OnClicked.AddUniqueDynamic(this, &UBaseUserWidget::PlayClickSound);
			}
		}
	}
}
