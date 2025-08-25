// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5wEditor - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

using UnrealBuildTool;

public class WindowsDualsense_ds5wEditor : ModuleRules
{
    public WindowsDualsense_ds5wEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", "WindowsDualsense_ds5w"
            }
        );
    }
}