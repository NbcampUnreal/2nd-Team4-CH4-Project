#include "Framework/Controller/MainMenuPlayerController.h"
#include "Framework/GameInstance/CCFFGameInstance.h"
#include "Character/Base/BasePreviewPawn.h"
#include "Items/Component/CharacterCustomizationComponent.h"
#include "Framework/HUD/MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(nullptr);
	SetInputMode(InputMode);
}

void AMainMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Back", IE_Pressed, this, &AMainMenuPlayerController::HandleBack);
}

void AMainMenuPlayerController::JoinServer(const FString& TargetIP)
{
	UCCFFGameInstance* GameInstance = GetGameInstance<UCCFFGameInstance>();
	if (GameInstance)
	{
		GameInstance->SetServerIP(TargetIP);
	}

	AMainMenuHUD* HUD = Cast<AMainMenuHUD>(GetHUD());
	if (IsValid(HUD))
	{
		HUD->ShowLoadingWidget();
	}

	UE_LOG(LogTemp, Log, TEXT("[MainMenuController] JoinServer : Connecting to Server at %s"), *TargetIP);
	UGameplayStatics::OpenLevel(this, FName(*TargetIP), true);
}

void AMainMenuPlayerController::FindSessions()
{
	UCCFFGameInstance* GameInstance = Cast<UCCFFGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->StartFindSessions(this);
	}
}

void AMainMenuPlayerController::HandleBack()
{
	AMainMenuHUD* HUD = Cast<AMainMenuHUD>(GetHUD());
	if (IsValid(HUD) == true)
	{
		HUD->ReturnToPreviousWidget();
	}
}

void AMainMenuPlayerController::HandleCharacterSelectedFromUI(FName CharacterID)
{
	ABasePreviewPawn* PreviewPawn = Cast<ABasePreviewPawn>(GetPawn());
	if (IsValid(PreviewPawn))
	{
		PreviewPawn->InitializePreview(CharacterID);
	}
}
