// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DeviceSettings.generated.h"

/**
 * Represents a mapping configuration for gamepad devices, specifically associating
 * a user with a device. This structure is mainly utilized within configuration settings
 * to identify and manage specific user-controller relationships.
 *
 * This structure includes user-related and device-related identifiers, which are stored
 * in the configuration file and editable in the Unreal Editor. It supports systems
 * where multiple gamepads need distinct mappings based on user or device IDs.
 */
USTRUCT(BlueprintType)
struct FGamepadMapping
{
	GENERATED_BODY()

	/**
	 * Identifies a user associated with a specific gamepad device within the input system.
	 * This variable serves as a unique identifier for linking user profiles to their respective
	 * gamepad configurations. It is primarily used in settings to enable proper mapping
	 * between users and devices for tailored input handling.
	 *
	 * The value is stored in configuration files and is editable through the Unreal Editor,
	 * making it suitable for scenarios where dynamic user-device associations are required.
	 */
	UPROPERTY(EditAnywhere, config, Category="Input")
	int32 UserId = 0;

	/**
	 * Represents the unique identifier associated with a specific device, primarily used
	 * to map and manage gamepad devices in the input system. This variable facilitates
	 * the assignment of device-specific configurations and ensures correct user-device
	 * associations.
	 *
	 * The DeviceId is stored in configuration files and can be adjusted within the Unreal
	 * Editor. It is particularly useful in setups involving multiple devices, enabling
	 * precise identification and control of individual gamepad hardware.
	 */
	UPROPERTY(EditAnywhere, config, Category="Input")
	int32 DeviceId = 0;
};


/**
 * Represents the configuration settings for managing Sony gamepads on Windows,
 * specifically tailored for DualSense gamepads.
 *
 * This class is derived from UObject and contains editable properties that
 * define configuration mappings for up to four Sony gamepads, along with
 * additional settings related to control offsets. The settings are
 * editable within the Unreal Editor and are stored in the configuration file
 * to persist user preferences or project configurations.
 */
UCLASS(config=WindowsDualSense, defaultconfig)
class WINDOWSDUALSENSE_DS5W_API UDeviceSettings : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Represents the primary configuration mapping for a Sony gamepad.
	 * This property is editable in the Unreal Editor and is stored in the configuration file.
	 * It is categorized under "Sony Gamepad Map" in the editor.
	 *
	 * Stores details about the gamepad such as the user and device IDs.
	 */
	UPROPERTY(EditAnywhere, config, Category = "Sony Gamepad Map")
	FGamepadMapping SonyGamepadOne;

	/**
	 * Represents the secondary configuration mapping for a Sony gamepad.
	 * This property is editable in the Unreal Editor and is stored in the configuration file.
	 * It is categorized under "Sony Gamepad Map" in the editor.
	 *
	 * Holds information about the gamepad, including the user and device IDs, allowing the system to
	 * manage and associate input settings for a specific gamepad.
	 */
	UPROPERTY(EditAnywhere, config, Category = "Sony Gamepad Map")
	FGamepadMapping SonyGamepadTwo;

	/**
	 * Represents the tertiary configuration mapping for a Sony gamepad.
	 * This property is editable in the Unreal Editor and is stored in the configuration file.
	 * It is categorized under "Sony Gamepad Map" in the editor.
	 *
	 * Maintains information about the gamepad, such as the associated user and device IDs.
	 * Useful in managing specific input settings related to the third Sony gamepad.
	 */
	UPROPERTY(EditAnywhere, config, Category = "Sony Gamepad Map")
	FGamepadMapping SonyGamepadThree;

	/**
	 * Represents the fourth configuration mapping for a Sony gamepad.
	 * This property is editable in the Unreal Editor and is stored in the configuration file.
	 * It is categorized under "Sony Gamepad Map" in the editor.
	 *
	 * Contains details about the gamepad such as the user ID and device ID, allowing the
	 * system to manage input settings specific to the fourth controller in the sequence.
	 */
	UPROPERTY(EditAnywhere, config, Category = "Sony Gamepad Map")
	FGamepadMapping SonyGamepadFour;

	/**
	 * Specifies the number of positions to reserve for other controls before the current one.
	 * This property is editable in the Unreal Editor and stored in the configuration file.
	 * It is categorized under "Sony Gamepad Map" in the editor.
	 *
	 * The value of this property is clamped between 0 and 4 and serves to offset the mapping
	 * of gamepad controls, ensuring proper allocation of control positions for specific
	 * configurations.
	 */
	UPROPERTY(EditAnywhere, config, Category="Sony Gamepad Map", meta = (ClampMin = "0", ClampMax = "4"), DisplayName="The number of positions to reserve for other controls before this one")
	int32 OffsetPosition = 0;
};
