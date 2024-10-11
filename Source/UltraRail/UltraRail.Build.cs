// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UltraRail : ModuleRules
{
	public UltraRail(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Built-in dependencies
        PublicDependencyModuleNames.AddRange(new []
        {
	        "Core",
	        "CoreUObject",
	        "Engine",
	        "InputCore",
	        "NavigationSystem",
	        "AIModule",
	        "Niagara",
	        "EnhancedInput"
        });
        
        
        // custom dependencies
        
        PublicDependencyModuleNames.AddRange(new []
		{
			"CustomAssetsEditorRuntime"
		});

        // custom editor-only dependencies
		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new []
			{
				"CustomAssetsEditor"
			});
		}
    }
}
