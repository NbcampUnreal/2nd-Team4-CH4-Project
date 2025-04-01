using UnrealBuildTool;

public class CCFF : ModuleRules
{
	public CCFF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(new string[]
        {
            "CCFF",
		});
    }
}
