#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/BaseUserWidget.h"
#include "Framework/UI/Settings/ApplySettingsInterface.h"
#include "ControlSettingsWidget.generated.h"

UCLASS()
class CCFF_API UControlSettingsWidget : public UBaseUserWidget, public IApplySettingsInterface
{
	GENERATED_BODY()
	
public:
	virtual void ApplySettings_Implementation() override {}
	virtual void ResetSettings_Implementation() override {}
	virtual void CancelSettings_Implementation() override {}

};
