#pragma once

#pragma region GeneralLogging

CCFF_API DECLARE_LOG_CATEGORY_EXTERN(LogCF, Log, All);

#define CFCheckF(Expression, Format, ...) \
{ \
	if ((Expression) == false) \
	{ \
		UE_LOG(LogCF, Error, TEXT("[%s(%d)] " Format), *FString(__FUNCTION__), __LINE__, ##__VA_ARGS__); \
		check(Expression); \
	} \
}

extern TAutoConsoleVariable<bool> CVarDebugGeneral;

#pragma endregion 

//Log Macro for Multiplay Debugging
#pragma region NetLogging

CCFF_API DECLARE_LOG_CATEGORY_EXTERN(LogCFNet, Log, All);

#define NETMODE_TCHAR ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))

#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))

#define CF_LOG_NET(LogCategory, Verbosity, Format, ...) \
    UE_LOG(LogCategory, Verbosity, TEXT("[%s] %s %s"), NETMODE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))

#pragma endregion
