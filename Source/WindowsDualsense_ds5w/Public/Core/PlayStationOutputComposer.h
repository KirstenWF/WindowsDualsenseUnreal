﻿// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include <concrt.h>
#include "CoreMinimal.h"
#include "Core/Structs/FDeviceContext.h"
#include "PlayStationOutputComposer.generated.h"

/**
 * @class UPlayStationOutputComposer
 *
 * A class designed to handle the composition and management of output data
 * for PlayStation controllers, such as DualSense and DualShock devices.
 * It facilitates data formatting, calculation of CRC32 for integrity checks,
 * and configuration of various output features including haptic triggers,
 * LEDs, and audio settings. The class also provides utilities for managing
 * device contexts.
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UPlayStationOutputComposer : public UObject
{
	GENERATED_BODY()

	/**
	 * @var UPlayStationOutputComposer::CRCSeed
	 *
	 * The predefined seed value used for CRC32 hash computation.
	 * Acts as the initial value for the hash generation algorithm employed in the Compute method.
	 * Ensures consistent and reliable hash results by providing a stable starting point.
	 */
	const static UINT32 CRCSeed;
	/**
	 * @variable HashTable
	 *
	 * Represents a precomputed hash lookup table specifically used within
	 * `UPlayStationOutputComposer` for optimizing hash calculations. This table
	 * is a key component in facilitating fast cryptographic or hash operations.
	 *
	 * The `HashTable` variable contains a fixed array of 256 32-bit values,
	 * each defined as hexadecimal constants. These values are statically initialized,
	 * ensuring deterministic results for hash-related operations. It is typically used
	 * in operations related to device communication, such as data integrity checks
	 * or packet processing within DualSense HID interactions.
	 *
	 * The structure of the table ensures constant-time retrieval of hash values,
	 * making it integral to performance-critical systems where such operations are frequent.
	 */
	const static UINT32 HashTable[256];
public:
	/**
	 * @brief Default constructor for the UPlayStationOutputComposer class.
	 *
	 * Initializes a new instance of the class. This constructor sets up
	 * the initial state for the output composer, preparing it for utilization
	 * in composing PlayStation output-related operations.
	 *
	 * @return A new instance of the UPlayStationOutputComposer class.
	 */
	UPlayStationOutputComposer(){};
	/**
	 * @fn UPlayStationOutputComposer::FreeContext(FDeviceContext* Context)
	 *
	 * Releases and invalidates the provided device context, ensuring that
	 * all associated resources are properly disposed of or reset. This method
	 * guarantees that the device handle referenced by the context becomes
	 * unusable after execution to prevent unauthorized or unintended access.
	 *
	 * @param Context The device context to be freed and invalidated. Must not be null.
	 */
	static void FreeContext(FDeviceContext* Context);
	/**
	 * @brief Configures and sends output data to a DualSense device using the provided device context.
	 *
	 * This method builds and formats an output data buffer with specific device settings,
	 * trigger effects, audio configurations, and LED adjustments, applying specifics
	 * based on the connection type (e.g., Bluetooth or USB). Additionally, it computes
	 * and appends a CRC checksum when operating over Bluetooth for data integrity checking.
	 * Finally, the prepared output data is submitted to the device for execution.
	 *
	 * @param DeviceContext A pointer to the device's context that holds input/output
	 *                      buffers, connection details, and configuration preferences.
	 */
	static void OutputDualSense(FDeviceContext* DeviceContext);
	/**
	 * Writes DualShock controller output values into the device buffer for communication.
	 * This method prepares the output packet for a DualShock controller, handling
	 * connection-specific adjustments (e.g., Bluetooth or wired) and configuring output
	 * features like rumble effects and lightbar settings.
	 *
	 * @param DeviceContext The context containing details of the targeted device,
	 *                      including connection type, output buffer, and settings
	 *                      for the controller's output functionalities.
	 */
	static void OutputDualShock(FDeviceContext* DeviceContext);
	/**
	 * Configures the trigger effect settings on a PlayStation controller using the provided haptic effect data.
	 *
	 * @param Trigger A pointer to the memory location where the trigger effect configuration is set.
	 *                This array represents the hardware-specific register values for the controller trigger effects.
	 * @param Effect A reference to an FHapticTriggers structure containing the haptic effect details such as mode, strengths,
	 *               and additional properties for defining the behavior of the trigger.
	 */
	static void SetTriggerEffects(unsigned char* Trigger, FHapticTriggers& Effect);
	/**
	 * Computes the CRC32 hash for the given buffer using a predefined hash table and seed value.
	 * The function iterates through each byte of the input buffer to calculate the resulting hash.
	 *
	 * @param Buffer A pointer to the input buffer containing the data for which the CRC32 hash is to be computed.
	 * @param Len The length of the input buffer in bytes.
	 * @return The computed CRC32 hash value.
	 */
	static UINT32 Compute(const unsigned char* Buffer, size_t Len);
};
