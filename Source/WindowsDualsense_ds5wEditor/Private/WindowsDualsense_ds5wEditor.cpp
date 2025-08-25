// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5wEditor - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "WindowsDualsense_ds5wEditor.h"
#include "DeviceSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FWindowsDualsense_ds5wEditorModule"

void FWindowsDualsense_ds5wEditorModule::StartupModule()
{

#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "Sony Gamepad",
			FText::FromString("Sony Gamepad Settings"),
			FText::FromString("Configuration settings for Windows Sony Gamepad plugin support."),
			GetMutableDefault<UDeviceSettings>());
	}
#endif
    
}

void FWindowsDualsense_ds5wEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FWindowsDualsense_ds5wEditorModule, WindowsDualsense_ds5wEditor)