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
TMap<int32, ISonyGamepadInterface*> UDeviceContainerManager::LibraryInstances;

UDeviceContainerManager* UDeviceContainerManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UDeviceContainerManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

int32 UDeviceContainerManager::ToMap(int32 Index)
{
	if (UDeviceSettings* Settings = NewObject<UDeviceSettings>())
	{
		TMap<int32, int32> ContextMap = {
			{Settings->SonyGamepadOne.UserId, Settings->SonyGamepadOne.DeviceId},
			{Settings->SonyGamepadTwo.UserId, Settings->SonyGamepadTwo.DeviceId},
			{Settings->SonyGamepadThree.UserId, Settings->SonyGamepadThree.DeviceId},
			{Settings->SonyGamepadFour.UserId, Settings->SonyGamepadFour.DeviceId}
		};

		if (ContextMap.Contains(Index))
		{
			return ContextMap[Index];
		}
	}
	return -1;
}

ISonyGamepadInterface* UDeviceContainerManager::GetLibraryOrReconnect(int32 ControllerId)
{
	int32 OldControllerId = ToMap(ControllerId);
	if (OldControllerId == -1)
	{
		return nullptr;
	}
	
	if (LibraryInstances.Contains(OldControllerId))
	{
		if (LibraryInstances[OldControllerId]->IsConnected())
		{
			return LibraryInstances[OldControllerId];
		}

		RemoveLibraryInstance(OldControllerId); // destruct instance to reconnect
	}

	if (!LibraryInstances.Contains(OldControllerId))
	{
		ISonyGamepadInterface* DSLibrary = CreateLibraryInstance(ControllerId);
		if (!DSLibrary)
		{
			return nullptr;
		}
		LibraryInstances.Add(OldControllerId, DSLibrary);
	}
	return LibraryInstances[OldControllerId];
}

ISonyGamepadInterface* UDeviceContainerManager::GetLibraryInstance(int32 ControllerId)
{
	ControllerId = ToMap(ControllerId);
	if (!LibraryInstances.Contains(ControllerId))
	{
		return nullptr;
	}

	if (!LibraryInstances[ControllerId]->IsConnected())
	{
		return nullptr;
	}

	return LibraryInstances[ControllerId];
}

void UDeviceContainerManager::RemoveLibraryInstance(int32 ControllerId)
{
	if (LibraryInstances.Contains(ControllerId))
	{
		LibraryInstances[ControllerId]->ShutdownLibrary();
		LibraryInstances.Remove(ControllerId);
	}
}

void UDeviceContainerManager::RemoveAllLibraryInstance()
{
	for (const auto& LibraryInstance : LibraryInstances)
	{
		LibraryInstance.Value->ShutdownLibrary();
	}
	LibraryInstances.Reset();
}

void UDeviceContainerManager::CreateLibraryInstances()
{
	LibraryInstances.Reset();

	TArray<FDeviceContext> DetectedDevices;
	DetectedDevices.Reset();

	UDeviceHIDManager* HIDManager = NewObject<UDeviceHIDManager>();
	if (!HIDManager->FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SonyGamepad: device not found. Creating default library instance."));
		return;
	}

	if (DetectedDevices.Num() > 8)
	{
		return;
	}
	
	for (int32 DeviceIndex = 0; DeviceIndex < DetectedDevices.Num(); DeviceIndex++)
	{
		FDeviceContext& Context = DetectedDevices[DeviceIndex];
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
				SonyGamepad->ShutdownLibrary();
				continue;
			}


			int32 ControllerId = -1;
			FPlatformUserId UserId;
			if (UDeviceSettings* Settings = NewObject<UDeviceSettings>())
			{
				switch (DeviceIndex)
				{
				case 0:
					ControllerId = Settings->SonyGamepadOne.DeviceId;
					UserId = FPlatformUserId::CreateFromInternalId(Settings->SonyGamepadOne.UserId);
					break;
				case 1:
					ControllerId = Settings->SonyGamepadTwo.DeviceId;
					UserId = FPlatformUserId::CreateFromInternalId(Settings->SonyGamepadTwo.UserId);
					break;
				case 2:
					ControllerId = Settings->SonyGamepadThree.DeviceId;
					UserId = FPlatformUserId::CreateFromInternalId(Settings->SonyGamepadThree.UserId);
					break;
				case 3:
					ControllerId = Settings->SonyGamepadFour.DeviceId;
					UserId = FPlatformUserId::CreateFromInternalId(Settings->SonyGamepadFour.UserId);
					break;
				default:
					ControllerId = 0;
					UserId = FPlatformUserId::CreateFromInternalId(0);
					break;
				}
			}
			
			SonyGamepad->_getUObject()->AddToRoot();
			SonyGamepad->SetUserId(UserId);
			SonyGamepad->SetControllerId(ControllerId);
			SonyGamepad->InitializeLibrary(Context);
			LibraryInstances.Add(ControllerId, SonyGamepad);
		}
	}
}

int32 UDeviceContainerManager::GetAllocatedDevices()
{
	return LibraryInstances.Num();
}

TMap<int32, ISonyGamepadInterface*> UDeviceContainerManager::GetAllocatedDevicesMap()
{
	 return LibraryInstances;
}

ISonyGamepadInterface* UDeviceContainerManager::CreateLibraryInstance(int32 ControllerID)
{
	if (ToMap(ControllerID) == -1)
	{
		return nullptr;
	}
	
	TArray<FDeviceContext> DetectedDevices;
	DetectedDevices.Reset();

	UDeviceHIDManager* HIDManager = NewObject<UDeviceHIDManager>();
	if (!HIDManager->FindDevices(DetectedDevices) || DetectedDevices.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SonyGamepad: device not found. Creating default library instance."));
		return nullptr;
	}

	if (ControllerID >= DetectedDevices.Num())
	{
		return nullptr;
	}
	
	FDeviceContext& Context = DetectedDevices[ControllerID];
	if (Context.IsConnected)
	{
		ISonyGamepadInterface*  SonyGamepad = nullptr;
		Context.Output = FOutputContext();
		Context.Handle = UDeviceHIDManager::CreateHandle(&Context);
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
			return nullptr;
		}

		SonyGamepad->_getUObject()->AddToRoot();
		SonyGamepad->SetControllerId(ToMap(ControllerID));
		SonyGamepad->SetUserId(FPlatformUserId::CreateFromInternalId(ControllerID));
		SonyGamepad->InitializeLibrary(Context);
		return SonyGamepad;
	}
	return nullptr;
}