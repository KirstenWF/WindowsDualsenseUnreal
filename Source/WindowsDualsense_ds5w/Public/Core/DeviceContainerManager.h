// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/SonyGamepadInterface.h"
#include "UObject/Object.h"
#include "DeviceContainerManager.generated.h"

/**
 * A manager class that handles the creation, storage, and lifecycle management of device library
 * instances associated with Sony gamepad controllers. This class ensures proper initialization,
 * cleanup, and access control for the controller devices.
 */
UCLASS()
class WINDOWSDUALSENSE_DS5W_API UDeviceContainerManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()


	/**
	 * Updates the device container manager at regular intervals by processing connected and
	 * disconnected devices. It handles device discovery, connection state updates, lifecycle
	 * management for device libraries, and ensures proper synchronization with previously
	 * known devices.
	 *
	 * @param DeltaTime The time in seconds since the last tick, used to accumulate time for
	 *                  periodic processing of the device lifecycle and connection state.
	 */
public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UDeviceContainerManager, STATGROUP_Tickables);
	}
	virtual bool IsTickableInEditor() const override
	{
		return true;
	}
	virtual bool IsTickable() const override
	{
		return true;
	}

	bool bIsDeviceDetectionInProgress = false;
	/**
	 * Destroys the device container manager and cleans up any allocated resources. This method
	/**
	 * Retrieves the static instance of the UDeviceContainerManager class. This method
	 * ensures that only a single instance of the manager class is created and provides
	 * global access to it for managing device library instances of Sony gamepad controllers.
	 *
	 * @return A pointer to the singleton instance of UDeviceContainerManager. Returns nullptr
	 *         if the instance has not been initialized.
	 */
	static UDeviceContainerManager* Get();
	/**
	 * Removes all existing library instances managed by the device container. This method
	 * is responsible for cleaning up and unloading all currently allocated Sony gamepad
	 * controllers' library resources, ensuring proper resource management and preventing
	 * potential memory leaks.
	 */
	static void RemoveAllLibraryInstance();
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
	 * Attempts to retrieve the library instance associated with the specified Sony gamepad
	 * controller ID. If no library instance is found, the method attempts to reconnect and
	 * reinitialize the library for the specified controller, ensuring continued functionality.
	 *
	 * @param ControllerId The unique identifier of the Sony gamepad controller for which
	 *                     the library instance is to be retrieved or reconnected.
	 * @return A pointer to the ISonyGamepadInterface instance associated with the specified
	 *         controller ID, or nullptr if the library could not be retrieved or reconnected.
	 */
	static ISonyGamepadInterface* GetLibraryOrReconnect(int32 ControllerId);
	/**
	 * Retrieves the total number of currently allocated device library instances.
	 * This method provides a count of the devices being managed by the container,
	 * useful for monitoring and ensuring proper resource allocation.
	 *
	 * @return The number of allocated device library instances.
	 */
	static int32 GetAllocatedDevices();
	/**
	 * Retrieves the map of allocated device library instances. This map associates unique integer
	 * keys with instances implementing the Sony gamepad interface, allowing access to the currently
	 * managed devices.
	 */
	static TMap<FInputDeviceId, ISonyGamepadInterface*> GetAllocatedDevicesMap();
	
private:
	/**
	 * A floating-point variable that represents the change or difference in the accumulator value over time.
	 * Typically used to measure incremental adjustments or deltas in processing or calculations.
	 */
	float AccumulatorDelta = 0.0f;
	/**
	 * A static instance of the UDeviceContainerManager, serving as the singleton instance
	 * for managing device library instances of Sony gamepad controllers. This variable
	 * ensures global access and consistent management of the controller devices, while
	 * enforcing a single unique instance of the manager class.
	 */
	static UDeviceContainerManager* Instance;
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
};
