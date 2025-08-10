// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Enums/EDeviceCommons.h"
#include "Core/Structs/FDeviceContext.h"
#include "Core/Structs/FDeviceSettings.h"
#include "InputCoreTypes.h"
#include "Misc/CoreDelegates.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "SonyGamepadInterface.generated.h"

USTRUCT(BlueprintType)
struct FFeatureReport
{
	GENERATED_BODY()
};

/**
 * Interface for interacting with a Sony gamepad in a system.
 */
UINTERFACE()
class USonyGamepadInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface defining the operations and configurations for a Sony gamepad device.
 * This interface must be implemented to interact with the gamepad, handling tasks
 * such as connection management, input updates, and device settings.
 */
class WINDOWSDUALSENSE_DS5W_API ISonyGamepadInterface
{
	GENERATED_BODY()

	
public:
	virtual ISonyGamepadInterface* Get() = 0;
	/**
	 * Pure virtual function that must be implemented by derived classes.
	 * Used to attempt reconnection with the associated Sony gamepad device.
	 * Typically, the implementation should ensure the gamepad is reconnected
	 * and restored to its operational state.
	 *
	 * This function is expected to be executed when the connection to the gamepad
	 * has been lost or is in a disconnected state.
	 */
	virtual void Reconnect() = 0;
	/**
	 * Pure virtual function that checks the connection status of the gamepad.
	 *
	 * @return True if the gamepad is connected, false otherwise.
	 */
	virtual bool IsConnected() = 0;

	/**
	 * Configures device settings for the Sony gamepad interface.
	 * This pure virtual function must be implemented by derived classes to apply
	 * specific settings defined in the FDeviceSettings struct to the gamepad device.
	 *
	 * @param Settings Reference to an FDeviceSettings struct containing the desired configuration parameters for the device.
	 */
	virtual void Settings(const FSettings<FFeatureReport>& Settings) = 0;

	/**
	 * Initializes the gamepad library using the specified device context.
	 *
	 * This function is used to establish the initial connection and setup
	 * of the gamepad library for communication with the appropriate hardware
	 * or system interface. The `FDeviceContext` provides necessary information
	 * about the device such as its handle, connection type, and related settings.
	 *
	 * @param Context A reference to an `FDeviceContext` structure containing
	 *                the device's configuration and connection details.
	 * @return A boolean value indicating whether the library was successfully initialized.
	 *         - `true` if the initialization was successful.
	 *         - `false` if the initialization failed.
	 */
	virtual bool InitializeLibrary(const FDeviceContext& Context) = 0;
	/**
	 * Shuts down and cleans up resources related to the gamepad library.
	 *
	 * This method is responsible for properly deallocating and releasing any
	 * resources initialized or managed by the gamepad library. It should be
	 * called when the library is no longer needed to ensure no resource leaks
	 * or dangling references remain.
	 *
	 * Implementers must provide the logic to safely terminate any running
	 * processes, close device connections, and reset internal states as necessary.
	 *
	 * This function must be implemented by classes that inherit from the
	 * ISonyGamepadInterface interface.
	 */
	virtual void ShutdownLibrary() = 0;
	/**
	 * Sets the identifier for the controller to associate it with a specific device or context.
	 *
	 * @param ControllerId An integer representing the unique identifier for the controller.
	 */
	virtual void SetControllerId(int32 ControllerId) = 0;
	/**
	 * Sets the color of the lightbar on the Sony gamepad.
	 *
	 * @param Color The desired color for the lightbar, represented as an FColor object.
	 */
	virtual void SetLightbar(FColor Color) = 0;
	/**
	 * Sets the LED associated with the player on the Sony gamepad to a specified brightness level.
	 *
	 * @param Led The LED associated with a specific player, represented as an ELedPlayerEnum.
	 * @param Brightness The desired brightness level for the LED, represented as an ELedBrightnessEnum.
	 */
	virtual void SetPlayerLed(ELedPlayerEnum Led, ELedBrightnessEnum Brightness) = 0;
	/**
	 * Sets the state of the microphone LED on the gamepad.
	 *
	 * @param Led The desired state of the microphone LED, represented by ELedMicEnum.
	 */
	virtual void SetMicrophoneLed(ELedMicEnum Led) = 0;
	/**
	 * Sets the touch state for the device.
	 *
	 * @param bIsTouch A boolean indicating whether touch input is enabled (true) or disabled (false).
	 */
	virtual void SetTouch(const bool bIsTouch) = 0;
	/**
	 * Sets the acceleration mode for the gamepad.
	 *
	 * @param bIsAccelerometer If true, enables accelerometer-based input; otherwise, disables it.
	 */
	virtual void SetAcceleration(bool bIsAccelerometer) = 0;
	/**
	 * Enables or disables the gyroscope functionality in the gamepad.
	 * @param bIsGyroscope Indicates whether the gyroscope should be enabled (true) or disabled (false).
	 */
	virtual void SetGyroscope(bool bIsGyroscope) = 0;
	/**
	 * Stops all currently active operations or actions associated with the interface.
	 * This method must be implemented by any derived class to handle the termination
	 * of all ongoing processes.
	 */
	virtual void StopAll() = 0;
	/**
	 * Retrieves the current battery level of the Sony gamepad.
	 *
	 * @return The battery level as a floating-point value, where the exact range
	 *         and representation are dependent on the implementation. Typically,
	 *         values may range between 0.0 (empty) and 1.0 (full).
	 */
	virtual float GetBattery() const = 0;
	/**
	 * Sets the vibration effect for the Sony gamepad.
	 *
	 * @param Values A reference to an FForceFeedbackValues struct containing the force feedback
	 *               intensity and duration for the vibration effect.
	 */
	virtual void SetVibration(const FForceFeedbackValues& Values) = 0;
	/**
	 * Pure virtual function that sends data or commands to the connected gamepad.
	 * This function must be implemented by any class inheriting this interface.
	 */
	virtual void SendOut() = 0;
	/**
	 * Updates input state for the gamepad.
	 *
	 * @param InMessageHandler A shared reference to the generic application message handler to process input.
	 * @param UserId The platform user identifier associated with the device.
	 * @param InputDeviceId The unique identifier for the input device being updated.
	 */
	virtual bool UpdateInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler, const FPlatformUserId UserId, const FInputDeviceId InputDeviceId) = 0;
};
