using UnrealBuildTool;

public class CCFF : ModuleRules
{
	public CCFF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Slate", "SlateCore" });

        PublicIncludePaths.AddRange(new string[]
        {
            "CCFF",
		});
    }
}
