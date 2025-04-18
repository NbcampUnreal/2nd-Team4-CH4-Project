using UnrealBuildTool;

public class CCFF : ModuleRules
{
	public CCFF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"UMG", "Niagara", "Slate", "SlateCore", "ApplicationCore",

            "OnlineSubsystem", "OnlineSubsystemUtils", "NetCore",
            "Json", "JsonUtilities"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] { "CCFF" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
