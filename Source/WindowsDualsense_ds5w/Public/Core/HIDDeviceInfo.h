// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include <concrt.h>
#include <chrono>
#include <mutex>
#include "DualSenseProxy.h"
#include "Structs/FDeviceContext.h"


struct FPingPolicy {
	std::chrono::milliseconds WakeThreshold{150};
	std::chrono::milliseconds MinInterval{100};
};

struct FPollState {
	std::chrono::steady_clock::time_point LastSuccess = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point LastPing    = std::chrono::steady_clock::time_point::min();
};

enum class EPollResult {
	ReadOk,
	NoIoThisTick,
	TransientError,
	Disconnected
};

/**
 * @brief Represents a static class for HID device management and operations.
 *
 * This class encapsulates various tasks related to managing multiple HID devices, including handling connections,
 * transmitting and receiving data, detecting device presence, and managing device-specific states or contexts.
 */
class FHIDDeviceInfo
{
	/**
	 * @brief Reads data from the specified HID device context.
	 *
	 * This method performs the process of retrieving input data from an HID device, verifying the context's validity,
	 * handling connection states, and managing input buffers. It also utilizes polling mechanisms to detect and
	 * report disconnection events or communication failures during the read process.
	 *
	 * @param Context Pointer to the device context representing the HID device being read. Must not be null and
	 *        should contain a valid device handle and input buffer. If the context is invalid or disconnected,
	 *        the method will handle associated cleanup and reporting.
	 */
public:
	static void Read(FDeviceContext* Context);
	/**
	 * @brief Writes data to the specified HID device context.
	 *
	 * This method handles sending data to an HID device using the provided device context.
	 * It determines the appropriate output report size based on the device type and connection type,
	 * ensures the device handle is valid, and writes the data. Handles errors such as failed writes
	 * or invalid handles and logs necessary debugging information.
	 *
	 * @param Context Pointer to the device context containing relevant information such as the device
	 *        handle, connection type, device type, and output buffer. Must not be null and must
	 *        represent a valid device handle for a successful write operation.
	 */
	static void Write(FDeviceContext* Context);
	/**
	 * @brief Detects available HID devices and updates the provided list of device contexts.
	 *
	 * This method performs an enumeration of all connected HID devices that are currently present and
	 * accessible on the system. It retrieves their attributes, device paths, and initializes valid device
	 * contexts for further interaction. Devices that cannot be accessed or initialized are skipped.
	 *
	 * @param Devices A reference to an array of FDeviceContext objects that will be updated to include
	 *        the detected and initialized HID device contexts. Existing data in the array will be overwritten.
	 */
	static void Detect(TArray<FDeviceContext>& Devices);
	/**
	 * @brief Creates and returns a handle for the specified HID device context.
	 *
	 * This method opens a connection to the HID device represented by the provided
	 * device context and returns a handle for managing further I/O operations.
	 * It ensures the device is accessible and logs errors in case of failure to
	 * open the handle.
	 *
	 * @param DeviceContext Pointer to the FDeviceContext object that contains the
	 *        path and other device-specific information required to establish the
	 *        connection. Must not be null and must represent a valid device path.
	 * @return A HANDLE to the opened device if successful, or INVALID_HANDLE_VALUE
	 *         if the operation fails.
	 */
	static HANDLE CreateHandle(FDeviceContext* Context);
	/**
	 * @brief Invalidates the handle of the specified HID device context and updates its connection status.
	 *
	 * This method ensures that the handle associated with the provided device context is properly invalidated.
	 * If the handle is valid, it will be closed and set to INVALID_HANDLE_VALUE. The connection status of the
	 * device context will also be updated to indicate that the device is no longer connected.
	 *
	 * @param Context Pointer to the device context representing the HID device whose handle is to be invalidated.
	 *        If the provided context is null, the method will return without performing any operations.
	 */
	static void InvalidateHandle(FDeviceContext* Context);
	static void InvalidateHandle(HANDLE Handle);

	/**
	 * @brief Performs a single ping operation on an HID device handle.
	 *
	 * This method checks the accessibility and basic functionality of the specified
	 * HID device by attempting to retrieve file information through the provided handle.
	 * It validates whether the device is responsive and updates the error code if the
	 * operation fails.
	 *
	 * @param Handle The handle to the HID device that will be pinged. Must be a valid and open handle
	 *        to a device that supports the required operations.
	 * @param OutLastError Pointer to a DWORD variable that receives the error code if the operation
	 *        fails. If the operation is successful, this will be set to ERROR_SUCCESS. This parameter
	 *        is optional and can be null.
	 * @return true if the ping operation succeeds and the device is accessible; false if an error occurs.
	 */
	static bool PingOnce(HANDLE Handle, DWORD* OutLastError = nullptr);
	/**
	 * @brief Polls and processes a single tick for a HID device, performing ping and read operations.
	 *
	 * This method checks the device's state, performs a ping operation if necessary, and reads data from the device.
	 * It updates the polling state accordingly and determines the result of this polling iteration.
	 *
	 * @param Handle A handle to the HID device being polled.
	 * @param Buffer A pointer to a buffer where the method writes the data read from the device.
	 * @param Length The maximum number of bytes that can be read into the buffer.
	 * @param Policy The policy specifying the timing and conditions for ping and polling operations.
	 * @param State A reference to an object that tracks the state of the polling operation for the HID device.
	 * @param OutBytesRead A reference to a variable where the number of bytes successfully read will be stored.
	 * @return An enumeration value of type EPollResult indicating the result of the polling operation.
	 */
	static EPollResult PollTick(HANDLE Handle, BYTE* Buffer, DWORD Length, const FPingPolicy& Policy, FPollState& State, DWORD& OutBytesRead);

	/**
	 * @brief Determines whether the given error code should be treated as a device disconnection.
	 *
	 * This method evaluates the provided error code and identifies if it corresponds
	 * to a scenario where a device should be considered as disconnected.
	 *
	 * @param Error The error code to evaluate.
	 * @return true if the error code indicates a device disconnection, false otherwise.
	 */
	static bool ShouldTreatAsDisconnected(const DWORD Error)
	{
		switch (Error)
		{
		case ERROR_DEVICE_NOT_CONNECTED:
		case ERROR_GEN_FAILURE:
		case ERROR_INVALID_HANDLE:
		case ERROR_BAD_COMMAND:
		case ERROR_FILE_NOT_FOUND:
		case ERROR_ACCESS_DENIED:
			return true;
		default:
			return false;
		}
	}

	/**
	 * @brief Represents the association of input device IDs with their respective polling results and states.
	 *
	 * This map associates each input device identifier with a pair consisting of its polling result status
	 * and the current state of the polling process. It provides a comprehensive overview of the interaction
	 * outcomes and states for each device during input polling iterations.
	 */
private:
	static TMap<FInputDeviceId, TPair<EPollResult, FPollState>> PollResults;
};
