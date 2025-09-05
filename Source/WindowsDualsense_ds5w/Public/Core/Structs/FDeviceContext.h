// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "FOutputContext.h"
#include "Core/Enums/EDeviceConnection.h"
#include "FDeviceContext.generated.h"

/**
 * @brief Represents the context and state of a connected device.
 *
 * This class is primarily used for managing the connection, data buffer, and
 * attributes of devices in the DualSense HID management system. Each device context
 * contains handles, connection details, and configuration needed for proper functionality.
 *
 * It is a crucial component for detecting, initializing, and managing devices
 * using related library systems.
 */
USTRUCT()
struct FDeviceContext
{
	GENERATED_BODY()
	/**
	 * @brief Raw device handle used for communication with a specific input/output hardware device.
	 *
	 * This handle represents a connection to the lower-level system or hardware interface
	 * and is primarily utilized for managing input/output operations, state retrieval,
	 * or other device-related activities.
	 *
	 * - The handle is created through platform-specific mechanisms, such as the `CreateHandle` function
	 *   in `UDualSenseHIDManager`.
	 * - It may represent a connection to a physical hardware device or similar system resources.
	 *
	 * @note Handle validity and state should always be verified before usage, as invalid or
	 * disconnected handles can result in undefined behavior.
	 * For instance, it may hold `INVALID_HANDLE_VALUE` when invalid or disconnected.
	 */
	void* Handle;
	/**
	 * A wchar_t array that represents the path to the device or resource
	 * associated with the FDeviceContext structure. The path is limited
	 * to 260 characters, which is commonly considered a maximum path length
	 * in various systems.
	 */
	wchar_t Path[260];
	/**
	 * @brief Internal data buffer for device communication.
	 *
	 * This buffer serves as a temporary storage space for handling input and output
	 * data during device interactions. It is specifically used in processing data
	 * exchange within the DualSense HID management framework.
	 *
	 * The size of the buffer is designed to accommodate the expected data payload
	 * during these communications, ensuring efficient handling of device protocols.
	 */
	unsigned char Buffer[78];
	/**
	 * @brief Internal data buffer for DualShock 4 Bluetooth communication.
	 *
	 * This buffer is specifically allocated for handling input and output data
	 * when communicating with DualShock 4 controllers connected via Bluetooth.
	 * It is used in the context of parsing and building device reports during
	 * Bluetooth transfers, ensuring compliance with the expected report format
	 * and size required by the DualShock 4 HID protocol.
	 *
	 * The buffer size of 547 bytes matches the standard payload requirements
	 * for DualShock 4 devices in Bluetooth communication scenarios.
	 *
	 * @note This buffer is not intended for DualSense controllers or USB connections;
	 *       for those, use Buffer[78] instead.
	 */
	unsigned char BufferDS4[547];
	/**
	 * A fixed-size buffer for storing input or output data associated with a device context.
	 * This buffer is utilized for reading device input reports or for other data
	 * management tasks. Its size is defined as 78 bytes to accommodate device
	 * input requirements, particularly for devices connected over Bluetooth.
	 *
	 * Buffer is a critical component of FDeviceContext and is managed to ensure
	 * data consistency and proper memory zeroing upon disconnection or failure.
	 *
	 * @note The size of this buffer is determined by the specific device connection
	 *       type or input requirements, ensuring compatibility and sufficient
	 *       data handling capabilities.
	 */
	unsigned char BufferOutput[78];
	/**
	 * Indicates whether the device is connected.
	 *
	 * This boolean variable represents the connection state of a device.
	 * - `true` indicates the device is currently connected.
	 * - `false` indicates the device is not connected.
	 *
	 * It is used to manage and track the status of devices in the context
	 * of operations such as detection, initialization, and communication.
	 */
	bool IsConnected;
	/**
	 * Represents the output configuration for a device context, typically used
	 * to control advanced features of a connected DualSense controller.
	 *
	 * The FOutput structure manages outputs such as lightbar color, microphone
	 * light states, player indicator LEDs, rumble motor intensities, and haptic
	 * trigger effects. Additionally, it includes audio and general feature configurations.
	 *
	 * This variable is initialized within a device context and used in conjunction
	 * with runtime operations to apply desired settings to the connected controller.
	 */
	FOutputContext Output;
	/**
	 * Specifies the type of connection used by a device.
	 *
	 * ConnectionType is an instance of the EDeviceConnection enumeration,
	 * which defines the connection modes a device can use. Supported connection
	 * types include USB, Bluetooth, and Unrecognized.
	 *
	 * This variable is primarily used in the context of managing device communication,
	 * where it helps determine the appropriate handling for input and output operations.
	 *
	 * Possible values:
	 * - Usb: Represents devices connected via USB.
	 * - Bluetooth: Represents devices connected via Bluetooth.
	 * - Unrecognized: Represents devices with an unknown or unsupported connection type.
	 *
	 * In scenarios such as device discovery or IO operations, ConnectionType
	 * influences logic such as input report length, output buffering, and connection state updates.
	 */
	EDeviceConnection ConnectionType;
	/**
	 * @brief Represents the type of device in the context of DualSense HID management.
	 *
	 * DeviceType specifies the category or model of the connected device, enabling
	 * the system to adjust its behavior or configurations accordingly. Examples include
	 * default DualSense controllers, the Edge variation, and older DualShock models.
	 *
	 * It plays a pivotal role in distinguishing devices for operations such as feature
	 * initialization, compatibility checks, and tailored input/output processing.
	 */
	EDeviceType DeviceType;
	/**
	 * @brief Uniquely identifies an input device within the system.
	 *
	 * This identifier is used to distinguish individual devices connected to
	 * the system, ensuring that each device can be accurately managed and tracked.
	 *
	 * Essential for scenarios where multiple input devices are connected,
	 * enabling seamless interaction and device-specific operations.
	 */
	FInputDeviceId UniqueInputDeviceId;
	/**
	 * @brief Represents the unique identifier for a platform-specific user.
	 *
	 * This variable is used to distinguish and manage users on a specific platform.
	 * It ensures that user-related data and operations are correctly assigned to the
	 * appropriate platform user, allowing seamless integration and user management
	 * in multi-user or multi-platform environments.
	 */
	FPlatformUserId UniquePlatformUserId;

	FDeviceContext(): Handle(nullptr), Path{}, Buffer{}, BufferDS4{}, BufferOutput{}, IsConnected(false),
	                  ConnectionType(), DeviceType(),
	                  UniqueInputDeviceId(),
	                  UniquePlatformUserId()
	{
	}
};
