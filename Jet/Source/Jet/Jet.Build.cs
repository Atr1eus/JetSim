// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Jet : ModuleRules
{
	public Jet(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
