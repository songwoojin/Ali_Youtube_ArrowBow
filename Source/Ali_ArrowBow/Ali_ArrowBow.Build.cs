// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ali_ArrowBow : ModuleRules
{
	public Ali_ArrowBow(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Ali_ArrowBow",
			"Ali_ArrowBow/Variant_Platforming",
			"Ali_ArrowBow/Variant_Platforming/Animation",
			"Ali_ArrowBow/Variant_Combat",
			"Ali_ArrowBow/Variant_Combat/AI",
			"Ali_ArrowBow/Variant_Combat/Animation",
			"Ali_ArrowBow/Variant_Combat/Gameplay",
			"Ali_ArrowBow/Variant_Combat/Interfaces",
			"Ali_ArrowBow/Variant_Combat/UI",
			"Ali_ArrowBow/Variant_SideScrolling",
			"Ali_ArrowBow/Variant_SideScrolling/AI",
			"Ali_ArrowBow/Variant_SideScrolling/Gameplay",
			"Ali_ArrowBow/Variant_SideScrolling/Interfaces",
			"Ali_ArrowBow/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
