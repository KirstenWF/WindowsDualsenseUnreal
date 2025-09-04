// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include <atomic>
#include <chrono>
#include <concrt.h>

class FRunnableThread;

/**
 * A dedicated runnable class for managing periodic HID device polling.
 *
 * The `FHIDPollingRunnable` class provides an abstraction for executing
 * periodic tasks, such as pinging a Human Interface Device (HID) to ensure
 * its availability or to perform other periodic maintenance operations. It
 * is designed to run in its own thread, executing its logic at a configurable
 * time interval.
 *
 * This class inherits from `FRunnable`, enabling it to integrate seamlessly
 * with Unreal's threading constructs. It handles initialization of resources,
 * execution of polling logic, and cleanup upon termination. It also provides
 * functionality for gracefully stopping the thread.
 *
 * The primary responsibilities of this class include setting up the device handle,
 * maintaining regular intervals for polling operations, and managing the thread's
 * lifecycle to ensure stability and resource management.
 */
class FHIDPollingRunnable final : public FRunnable
{
	/**
	 * Constructs an instance of the `FHIDPollingRunnable` class, initializing it with the given device handle and polling interval.
	 *
	 * This constructor sets up the required resources for the polling operation, including
	 * the target device handle and the interval at which polling tasks will be executed.
	 *
	 * @param InDeviceHandle The handle to the HID device that this polling runnable will manage.
	 * @param InInterval The time interval, in milliseconds, defining how frequently the polling operation should occur.
	 * @return A newly constructed `FHIDPollingRunnable` instance prepared for polling operations.
	 */
public:
	FHIDPollingRunnable(HANDLE InDeviceHandle, const std::chrono::milliseconds InInterval);
	/**
	 * Destructor for the `FHIDPollingRunnable` class.
	 *
	 * Responsible for performing cleanup operations when an instance of
	 * `FHIDPollingRunnable` is destroyed. This involves releasing resources,
	 * terminating any ongoing operations, and ensuring that the class instance
	 * is properly disposed of to prevent memory leaks or dangling pointers.
	 *
	 * The destructor ensures that the HID polling thread is stopped and all
	 * associated resources are safely released before the object is destroyed.
	 */
	virtual ~FHIDPollingRunnable() override;

	/**
	 * Initializes the runnable before execution begins.
	 *
	 * This method is called to perform any necessary setup required for the
	 * `FHIDPollingRunnable` instance. By default, it delegates to the base
	 * class `FRunnable::Init` for initialization. Override this if additional
	 * setup logic specific to `FHIDPollingRunnable` is required.
	 *
	 * @return True if the initialization succeeds, otherwise false.
	 */
	virtual bool Init() override;
	/**
	 * Executes the main logic of the runnable in a dedicated thread.
	 *
	 * This method contains the core operations to be performed in the thread,
	 * managing periodic pinging of a connected HID device. It ensures efficient
	 * timing using `std::this_thread::sleep_until`, handles device communication
	 * via `FHIDDeviceInfo::PingOnce`, and gracefully terminates the loop when the
	 * device is disconnected or a stop request is issued.
	 *
	 * The method computes the next scheduled operation time and ensures stable
	 * intervals are maintained, even under system slowdowns. If the device is
	 * considered disconnected based on the result of pinging or other criteria,
	 * the method logs the event and terminates the thread.
	 *
	 * @return An exit code of `0` indicating the thread completed its execution.
	 */
	virtual uint32 Run() override;
	/**
	 * Signals the runnable to stop execution.
	 *
	 * This method is called to request termination of the `FHIDPollingRunnable` instance.
	 * It ensures that the thread managing the periodic polling operations is safely notified
	 * to stop. The method delegates to the base class `FRunnable::Stop` for the core stop
	 * handling logic. Override this if specific stop behavior for `FHIDPollingRunnable`
	 * is needed.
	 *
	 * Once invoked, the runnable will complete any in-progress operation and terminate
	 * gracefully at the next appropriate checkpoint.
	 */
	virtual void Stop() override;
	/**
	 * Cleans up resources and performs shutdown tasks after execution ends.
	 *
	 * This method is invoked to handle any necessary cleanup for the
	 * `FHIDPollingRunnable` instance once the execution has finished. By default,
	 * it delegates cleanup operations to the base class `FRunnable::Exit`.
	 * Override this method to implement additional teardown logic specific to
	 * `FHIDPollingRunnable`, if needed.
	 */
	virtual void Exit() override;

	/**
	 *
	 */
	void StartThread();

	/**
	 * A handle to a device used for communication or management in HID operations.
	 *
	 * This member represents a system-specific HANDLE, typically used to interface
	 * with a connected device, such as opening, reading from, or writing to it.
	 * In the context of the `FHIDPollingRunnable` class, it is utilized to perform
	 * actions like sending periodic pings or ensuring the device connection remains valid.
	 *
	 * Proper initialization and cleanup of this handle are critical to avoid resource
	 * leaks or undefined behavior when interacting with hardware devices.
	 */
private:
	HANDLE DeviceHandle;
	
	/**
	 * Pointer to the thread managing the execution of the runnable instance.
	 *
	 * This member represents the dedicated thread created to run the `FHIDPollingRunnable` instance.
	 * It is responsible for initializing, executing, and managing the lifecycle of the runnable's
	 * `Run` method. The thread ensures that the polling logic operates independently and does not
	 * block the main execution flow.
	 *
	 * Proper management of this thread, including starting, stopping, and cleanup, is crucial to
	 * prevent resource leaks and ensure smooth operation of the system.
	 */
	FRunnableThread* Thread;

	/**
	 * Specifies the time interval for periodic operations or polling.
	 *
	 * This member indicates the duration between consecutive actions, such as pings
	 * or other scheduled tasks, executed within the context of the `FHIDPollingRunnable` class.
	 * It is represented in milliseconds and guarantees a consistent timing mechanism
	 * for maintaining regular intervals when performing operations like communication
	 * with a device.
	 *
	 * Proper configuration ensures optimal timing and prevents excessive or insufficient
	 * resource utilization during operations.
	 */
	std::chrono::milliseconds Interval;

	/**
	 * A thread-safe flag indicating whether a stop request has been issued.
	 *
	 * This atomic boolean is primarily used to signal a running thread to terminate
	 * its execution in a safe and controlled manner. For example, in the context of the
	 * `FHIDPollingRunnable` class, it indicates if the polling thread should stop its
	 * periodic execution and terminate.
	 *
	 * By using `std::atomic_bool`, it ensures that the flag is updated and read across
	 * threads without introducing race conditions.
	 */
	std::atomic_bool bStopRequested;
	
};
