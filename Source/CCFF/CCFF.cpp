#include "CCFF.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, CCFF, "CCFF" );
 
#pragma region GeneralLogging

DEFINE_LOG_CATEGORY(LogCF);

TAutoConsoleVariable<bool> CVarDebugGeneral(TEXT("CF.Debug.General"), false, TEXT("일반적인 디버깅 정보를 띄우고자 할 때 사용합니다.\n"), ECVF_Cheat);

#pragma endregion

#pragma region NetLogging

DEFINE_LOG_CATEGORY(LogCFNet);

#pragma endregion