#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ApplySettingsInterface.generated.h"

UINTERFACE(MinimalAPI)
class UApplySettingsInterface : public UInterface
{
	GENERATED_BODY()
};

class CCFF_API IApplySettingsInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	void ApplySettings();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	void ResetSettings();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Settings")
	void CancelSettings();
};
