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
TMap<int32, int32> UDeviceContainerManager::Settings;

UDeviceContainerManager* UDeviceContainerManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UDeviceContainerManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

int32 UDeviceContainerManager::ToMap(const int32 Index)
{
	if (UDeviceSettings* SettingsDevice = NewObject<UDeviceSettings>())
	{
		if (Settings.Num() == 0)
		{
			Settings = {
				{SettingsDevice->SonyGamepadOne.UserId, SettingsDevice->SonyGamepadOne.DeviceId},
				{SettingsDevice->SonyGamepadTwo.UserId, SettingsDevice->SonyGamepadTwo.DeviceId},
				{SettingsDevice->SonyGamepadThree.UserId, SettingsDevice->SonyGamepadThree.DeviceId},
				{SettingsDevice->SonyGamepadFour.UserId, SettingsDevice->SonyGamepadFour.DeviceId}
			};
		}
	}

	if (Settings.Contains(Index))
	{
		return Settings[Index];
	}
	return -1;
}

ISonyGamepadInterface* UDeviceContainerManager::GetLibraryOrReconnect(int32 ControllerId)
{
	const int32 GamepadId = ToMap(ControllerId);
	if (GamepadId < 0)
	{
		return nullptr;
	}
	
	if (LibraryInstances.Contains(GamepadId))
	{
		if (LibraryInstances[GamepadId]->IsConnected())
		{
			LibraryInstances[GamepadId]->SetControllerId(GamepadId);
			LibraryInstances[GamepadId]->SetUserId(FPlatformUserId::CreateFromInternalId(ControllerId));
			return LibraryInstances[GamepadId];
		}

		RemoveLibraryInstance(GamepadId); // destruct instance to reconnect
	}

	if (!LibraryInstances.Contains(GamepadId))
	{
		ISonyGamepadInterface* DSLibrary = CreateLibraryInstance(ControllerId);
		if (!DSLibrary)
		{
			return nullptr;
		}
		LibraryInstances.Add(GamepadId, DSLibrary);
	}
	return LibraryInstances[GamepadId];
}

ISonyGamepadInterface* UDeviceContainerManager::GetLibraryInstance(int32 ControllerId)
{
	const int32 Id = ToMap(ControllerId);
	if (!LibraryInstances.Contains(Id))
	{
		return nullptr;
	}

	if (!LibraryInstances[Id]->IsConnected())
	{
		return nullptr;
	}
	return LibraryInstances[Id];
}

void UDeviceContainerManager::RemoveLibraryInstance(int32 ControllerId)
{
	if (const int32 Id = ToMap(ControllerId); LibraryInstances.Contains(Id))
	{
		LibraryInstances[Id]->ShutdownLibrary();
		LibraryInstances.Remove(Id);
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
			if (const UDeviceSettings* SettingsDevice = NewObject<UDeviceSettings>())
			{
				switch (DeviceIndex)
				{
				case 0:
					ControllerId = SettingsDevice->SonyGamepadOne.DeviceId;
					UserId = FPlatformUserId::CreateFromInternalId(SettingsDevice->SonyGamepadOne.UserId);
					break;
				case 1:
					ControllerId = SettingsDevice->SonyGamepadTwo.DeviceId;
					UserId = FPlatformUserId::CreateFromInternalId(SettingsDevice->SonyGamepadTwo.UserId);
					break;
				case 2:
					ControllerId = SettingsDevice->SonyGamepadThree.DeviceId;
					UserId = FPlatformUserId::CreateFromInternalId(SettingsDevice->SonyGamepadThree.UserId);
					break;
				case 3:
					ControllerId = SettingsDevice->SonyGamepadFour.DeviceId;
					UserId = FPlatformUserId::CreateFromInternalId(SettingsDevice->SonyGamepadFour.UserId);
					break;
				default:
					ControllerId = 0;
					UserId = FPlatformUserId::CreateFromInternalId(0);
					break;
				}
			}
			SetSettings(UserId.GetInternalId(), ControllerId);
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
	int32 OffsetDetected = 0;
	if (const UDeviceSettings* SettingsDevice = NewObject<UDeviceSettings>())
	{
		OffsetDetected = SettingsDevice->OffsetPosition;
	}

	const int32 Id = ToMap(ControllerID) - OffsetDetected;
	if (Id < 0)
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
	
	if (Id >= DetectedDevices.Num())
	{
		return nullptr;
	}
	
	FDeviceContext Context = DetectedDevices[Id];
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
		SonyGamepad->SetControllerId(Id + OffsetDetected);
		SonyGamepad->SetUserId(FPlatformUserId::CreateFromInternalId(ControllerID));
		SonyGamepad->InitializeLibrary(Context);
		return SonyGamepad;
	}
	return nullptr;
}