using UnrealBuildTool;

public class Async_VR_Project : ModuleRules
{
	public Async_VR_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore", "OnlineSubsystem", "OnlineSubsystemUtils" });

		PrivateDependencyModuleNames.AddRange(new string[] { "SteamVR", "SteamVRController", "HeadMountedDisplay", "NavigationSystem" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
