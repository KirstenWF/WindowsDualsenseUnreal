// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Core/DeviceContainerManager.h"

#include "DeviceSettings.h"
#include "SonyGamepadProxy.h"
#include "Core/DeviceHIDManager.h"
#include "Core/DualSense/DualSenseLibrary.h"
#include "Core/DualShock/DualShockLibrary.h"

UDeviceContainerManager* UDeviceContainerManager::Instance;
TMap<FInputDeviceId, ISonyGamepadInterface*> UDeviceContainerManager::LibraryInstances;
TMap<FString, FInputDeviceId> UDeviceContainerManager::KnownDevicePaths;

bool PrimaryTick = true;
void UDeviceContainerManager::Tick(float DeltaTime)
{
	if (!PrimaryTick)
	{
		AccumulatorDelta += DeltaTime;
		if (AccumulatorDelta < 10.0f || bIsDeviceDetectionInProgress)
		{
			return;
		}

		AccumulatorDelta = 0.0f;
		bIsDeviceDetectionInProgress = true;
	}
	
	PrimaryTick = false;
	bIsDeviceDetectionInProgress = true;

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this]()
	{
		IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
		TArray<FDeviceContext> DetectedDevices;
		DetectedDevices.Reset();
		if (const UDeviceHIDManager* HIDManager = NewObject<UDeviceHIDManager>(); !HIDManager->FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("SonyGamepad: device not found. Creating default library instance."));
			return;
		}

		if (DetectedDevices.Num() > 8)
		{
			return;
		}

		TSet<FString> CurrentlyConnectedPaths;
		for (const FDeviceContext& Context : DetectedDevices)
		{
			CurrentlyConnectedPaths.Add(Context.Path);
		}

		TArray<FString> DisconnectedPaths;
		for (const TPair<FString, FInputDeviceId>& KnownDevice : KnownDevicePaths)
		{
			const FString& Path = KnownDevice.Key;
			const FInputDeviceId& DeviceId = KnownDevice.Value;

			if (!CurrentlyConnectedPaths.Contains(Path))
			{
				DeviceMapper.Internal_SetInputDeviceConnectionState(DeviceId, EInputDeviceConnectionState::Disconnected);
				if (ISonyGamepadInterface** FoundInstance = LibraryInstances.Find(DeviceId))
				{
					(*FoundInstance)->ShutdownLibrary();
					(*FoundInstance)->_getUObject()->RemoveFromRoot();
					LibraryInstances.Remove(DeviceId);
				}
				DisconnectedPaths.Add(Path);
			}
		}

		for (const FString& Path : DisconnectedPaths)
		{
			KnownDevicePaths.Remove(Path);
		}
	
		for (FDeviceContext& Context : DetectedDevices)
		{
			if (!KnownDevicePaths.Contains(Context.Path))
			{
				Context.Output = FOutputContext();
				Context.Handle = UDeviceHIDManager::CreateHandle(&Context);
				if (Context.IsConnected)
				{
					ISonyGamepadInterface* SonyGamepad = nullptr;
					if (Context.DeviceType == EDeviceType::DualSense || Context.DeviceType == EDeviceType::DualSenseEdge)
					{
						SonyGamepad = Cast<ISonyGamepadInterface>(NewObject<UDualSenseLibrary>(UDualSenseLibrary::StaticClass()));
					}
				
					if (Context.DeviceType == EDeviceType::DualShock4)
					{
						SonyGamepad = Cast<ISonyGamepadInterface>(NewObject<UDualShockLibrary>(UDualShockLibrary::StaticClass()));
					}
				
					if (!SonyGamepad)
					{
						continue;
					}

					const FInputDeviceId NewDeviceId = DeviceMapper.AllocateNewInputDeviceId();
					const FPlatformUserId NewUserId = DeviceMapper.AllocateNewUserId();
					
					const FName UniqueNamespace = TEXT("DeviceManager.WindowsDualsense");
					const FHardwareDeviceIdentifier HardwareId(UniqueNamespace, Context.Path);

					Context.UniqueInputDeviceId = NewDeviceId;
					Context.UniquePlatformUserId = NewUserId;

					
					SonyGamepad->_getUObject()->AddToRoot();
					SonyGamepad->SetControllerId(Context.UniqueInputDeviceId.GetId());
					SonyGamepad->InitializeLibrary(Context);

					KnownDevicePaths.Add(Context.Path, Context.UniqueInputDeviceId);
					LibraryInstances.Add(Context.UniqueInputDeviceId, SonyGamepad);

					DeviceMapper.Internal_MapInputDeviceToUser(Context.UniqueInputDeviceId, Context.UniquePlatformUserId, EInputDeviceConnectionState::Connected);
				}
			}
		}
		bIsDeviceDetectionInProgress = false;
	});
}

UDeviceContainerManager* UDeviceContainerManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UDeviceContainerManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

ISonyGamepadInterface* UDeviceContainerManager::GetLibraryOrReconnect(int32 ControllerId)
{
	const FInputDeviceId Gamepad = FInputDeviceId::CreateFromInternalId(ControllerId);
	if (!LibraryInstances.Contains(Gamepad))
	{
		return nullptr;
	}
	return LibraryInstances[Gamepad];
}

ISonyGamepadInterface* UDeviceContainerManager::GetLibraryInstance(int32 ControllerId)
{
	const FInputDeviceId GamepadId = FInputDeviceId::CreateFromInternalId(ControllerId);
	if (!LibraryInstances.Contains(GamepadId))
	{
		return nullptr;
	}

	if (!LibraryInstances[GamepadId]->IsConnected())
	{
		return nullptr;
	}
	return LibraryInstances[GamepadId];
}

void UDeviceContainerManager::RemoveAllLibraryInstance()
{
	for (const auto& LibraryInstance : LibraryInstances)
	{
		LibraryInstance.Value->ShutdownLibrary();
	}
	LibraryInstances.Reset();
}

int32 UDeviceContainerManager::GetAllocatedDevices()
{
	return LibraryInstances.Num();
}

TMap<FInputDeviceId, ISonyGamepadInterface*> UDeviceContainerManager::GetAllocatedDevicesMap()
{
	 return LibraryInstances;
}
