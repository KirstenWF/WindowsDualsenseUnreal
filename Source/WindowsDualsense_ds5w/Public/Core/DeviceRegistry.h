// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "Windows/WindowsApplication.h"
#include "Async/TaskGraphInterfaces.h"
#include "Core/HIDPollingRunnable.h"
#include "HAL/PlatformProcess.h"
#include "HAL/RunnableThread.h"
#include "Interfaces/SonyGamepadInterface.h"
#include "UObject/Object.h"
#include "DeviceRegistry.generated.h"

/**
 * A manager class that handles the creation, storage, and lifecycle management of device library
 * instances associated with Sony gamepad controllers. This class ensures proper initialization,
 * cleanup, and access control for the controller devices.
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDeviceRegistry : public UObject
{
	GENERATED_BODY()
	/**
	 * Retrieves the static instance of the UDeviceRegistry class. This method
	 * ensures that only a single instance of the manager class is created and provides
	 * global access to it for managing device library instances of Sony gamepad controllers.
	 *
	 * @return A pointer to the singleton instance of UDeviceContainerManager. Returns nullptr
	 *         if the instance has not been initialized.
	 */
public:
	static UDeviceRegistry* Get();
	/**
	 * Destructor for the UDeviceRegistry class. Responsible for cleaning up resources associated with the device
	 * library instances. Ensures that all active connection watchers are properly removed and their corresponding
	 * controller instances are cleaned up to prevent resource leakage.
	 */
	virtual ~UDeviceRegistry() override;
	/**
	 * Retrieves the total number of currently allocated device library instances.
	 * This method provides a count of the devices being managed by the container,
	 * useful for monitoring and ensuring proper resource allocation.
	 *
	 * @return The number of allocated device library instances.
	 */
	static int32 GetAllocatedDevices();
	/**
	 * Retrieves the library instance associated with a specific Sony gamepad controller,
	 * identified by its unique controller ID. This method ensures that the appropriate
	 * library instance for the given controller is returned, enabling interaction with
	 * the corresponding input device.
	 *
	 * @param ControllerId The unique identifier of the Sony gamepad controller for which
	 *                     the library instance is to be retrieved.
	 * @return A pointer to the ISonyGamepadInterface instance corresponding to the specified
	 *         controller ID, or nullptr if no matching instance exists.
	 */
	static ISonyGamepadInterface* GetLibraryInstance(int32 ControllerId);
	/**
	 * Retrieves the map of allocated device library instances. This map associates unique integer
	 * keys with instances implementing the Sony gamepad interface, allowing access to the currently
	 * managed devices.
	 */
	static TMap<FInputDeviceId, ISonyGamepadInterface*> GetAllocatedDevicesMap();
	/**
	 * Removes all existing library instances managed by the device container. This method
	 * is responsible for cleaning up and unloading all currently allocated Sony gamepad
	 * controllers' library resources, ensuring proper resource management and preventing
	 * potential memory leaks.
	 */
	static void RemoveAllLibraryInstance();
	/**
	 * Removes a library instance associated with the specified controller ID, disconnecting the
	 * corresponding input device if it is currently connected. Ensures proper removal and cleanup
	 * of the library instance from the internal container.
	 *
	 * @param ControllerId The unique identifier of the controller whose library instance is to be removed.
	 */
	static void RemoveLibraryInstance(int32 ControllerId);
	/**
	 * Creates an instance of a device library based on the provided device context. It initializes and
	 * manages the lifecycle of the Sony gamepad library for controllers like DualSense, DualSense Edge,
	 * or DualShock4. This method ensures proper mapping of device paths, unique IDs, and initialization
	 * of associated input devices.
	 *
	 * @param Context The device context containing the type of controller, its path,
	 *                and information required for initialization and identification.
	 */
	static void CreateLibraryInstance(FDeviceContext& Context);
	/**
	 * Updates the device container manager at regular intervals by processing connected and
	 * disconnected devices. It handles device discovery, connection state updates, lifecycle
	 * management for device libraries, and ensures proper synchronization with previously
	 * known devices.
	 *
	 * @param DeltaTime The time in seconds since the last tick, used to accumulate time for
	 *                  periodic processing of the device lifecycle and connection state.
	 */
	void DetectedChangeConnections(float DeltaTime);
	
private:
	/**
	 * A floating-point variable that represents the change or difference in the accumulator value over time.
	 * Typically used to measure incremental adjustments or deltas in processing or calculations.
	 */
	static float AccumulatorDelta;
	/**
	 * A boolean variable indicating whether the device detection process is currently active.
	 * Used to track the ongoing state of device discovery and initialization within the device manager.
	 */
	static bool bIsDeviceDetectionInProgress;
	/**
	 * A static instance of the UDeviceRegistry, serving as the singleton instance
	 * for managing device library instances of Sony gamepad controllers. This variable
	 * ensures global access and consistent management of the controller devices, while
	 * enforcing a single unique instance of the manager class.
	 */
	static UDeviceRegistry* Instance;
	/**
	 * A static map that holds associations between input device IDs and their corresponding Sony gamepad interface instances.
	 * This map is used to store and manage the lifecycle of gamepad interface objects, enabling efficient lookup and access
	 * to the respective implementations for handling Sony gamepad interactions.
	 */
	static TMap<FInputDeviceId, ISonyGamepadInterface*> LibraryInstances;
	/**
	 * A static map that associates device paths represented as strings with unique input device
	 * identifiers. This serves as a lookup mechanism to manage and track known device connections
	 * effectively within the system.
	 */
	static TMap<FString, FInputDeviceId> KnownDevicePaths;
	/**
	 * A static mapping that associates unique device identifiers with pairs containing input device IDs
	 * and platform user IDs. This data structure is used to track the history of devices interacting
	 * within the system, enabling efficient querying and management of device-user relationships.
	 */
	static TMap<FString, TPair<FInputDeviceId, FPlatformUserId>> HistoryDevices;

	/**
	 * A static map that maintains active connections by associating unique integer identifiers with their
	 * corresponding HID polling runnable instances. This map is used to manage and monitor ongoing input
	 * device connection activities and ensures proper lifecycle control of the associated polling threads.
	 */
	static TMap<int32, TUniquePtr<FHIDPollingRunnable>> ActiveConnectionWatchers;

};
