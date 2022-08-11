// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FogOfWarExperiments : ModuleRules
{
	public FogOfWarExperiments(ReadOnlyTargetRules Target) : base(Target)
	{
		// GENERATED_UNREAL_PUBLIC_DEPENDENCIES_EDITOR_START
		// GENERATED_UNREAL_PUBLIC_DEPENDENCIES_EDITOR_END
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara" });
    }
}
