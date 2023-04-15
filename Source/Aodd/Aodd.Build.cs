// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Aodd : ModuleRules
{
	public Aodd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
		"Core", 
		"CoreUObject",
		"Engine", 
		"InputCore", 
		"GameplayTasks",
		"NavigationSystem" 
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] 
		{
		"Aodd/Public/MainHero",
		"Aodd/Public/Components",
		"Aodd/Public/Weapons",
		"Aodd/Public/Enemy",
		"Aodd/Public/Units",
		"Aodd/Public/Tasks",
		"Aodd/Public/Animations",
		"Aodd/Public/UI",
		"Aodd/Public/Else",
		"Aodd/Public/Menu",
		"Aodd/Public/SaveGame"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
