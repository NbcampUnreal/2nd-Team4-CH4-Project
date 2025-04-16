#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Framework/UI/BaseUserWidget.h"
#include "TitleHUD.generated.h"

UENUM(BlueprintType)
enum class ETitleUIState : uint8
{
	ETITLE_PRESSSTART	UMETA(DisplayName = "Press Start"),
	ETITLE_LOGIN		UMETA(DisplayName = "Login"),
	ETITLE_MAX UMETA(Hidden)
};

UCLASS()
class CCFF_API ATitleHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ATitleHUD();
	virtual void BeginPlay() override;

	void NotifyAnyKeyPressed();
	void SwitchUI(ETitleUIState NewState);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TitleWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PressStartWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoginWidgetClass;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> TitleWidgetInstance = nullptr;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> SubWidget = nullptr;

private:
	void RemoveCurrentWidget();

	UFUNCTION()
	void HandleLoginSuccess();
};
