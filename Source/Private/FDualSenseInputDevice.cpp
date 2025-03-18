﻿// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "FDualSenseInputDevice.h"
#include "DualSenseLibrary.h"
#include "FDualSenseLibraryManager.h"
#include "Windows/WindowsApplication.h"


FDualSenseInputDevice::FDualSenseInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler):
	FGenericPlatformInputDeviceMapper(true, false),
	MessageHandler(InMessageHandler)
{
}

void FDualSenseInputDevice::Tick(float DeltaTime)
{
	TArray<FInputDeviceId> DeviceIds;
	Get().GetAllConnectedInputDevices(DeviceIds);

	for (FInputDeviceId& Device : DeviceIds)
	{
		UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(Device.GetId());
		if (!DsLibrary && !IsValid(DsLibrary))
		{
			continue;
		}

		const FPlatformUserId UserId = GetUserForInputDevice(Device);
		DsLibrary->UpdateInput(MessageHandler, UserId, Device);
	}
}

void FDualSenseInputDevice::SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property)
{
	if (!Property)
	{
		return;
	}

	if (Property->Name == "InputDeviceLightColor")
	{
		const FInputDeviceLightColorProperty* ColorProperty = static_cast<const FInputDeviceLightColorProperty*>(Property);
		SetLightColor(ControllerId, ColorProperty->Color);
	}
	
	if (Property->Name != "InputDeviceLightColor")
	{
		UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
		if (!DsLibrary)
		{
			return;
		}
		
		DsLibrary->SetTriggers(Property);
	}
}

void FDualSenseInputDevice::SetLightColor(const int32 ControllerId, const FColor Color)
{
	UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->UpdateColorOutput(Color);
}

void FDualSenseInputDevice::ResetLightColor(const int32 ControllerId)
{
	UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->UpdateColorOutput(FColor::Blue);
}

void FDualSenseInputDevice::Reconnect(FInputDeviceId& Device)
{
	MappedInputDevices[Device].ConnectionState = EInputDeviceConnectionState::Connected;
}

void FDualSenseInputDevice::Disconnect(FInputDeviceId& Device)
{
	MappedInputDevices[Device].ConnectionState = EInputDeviceConnectionState::Disconnected;
}

void FDualSenseInputDevice::SetHapticFeedbackValues(const int32 ControllerId, const int32 Hand, const FHapticFeedbackValues& Values)
{
	UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->SetHapticFeedbackValues(Hand, &Values);
}

void FDualSenseInputDevice::GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const
{
	MinFrequency = 0.0f;
	MaxFrequency = 1.0f;
}

bool FDualSenseInputDevice::SupportsForceFeedback(int32 ControllerId)
{
	return true;
}

void FDualSenseInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values)
{
	UDualSenseLibrary* DsLibrary = UFDualSenseLibraryManager::Get()->GetLibraryInstance(ControllerId);
	if (!DsLibrary)
	{
		return;
	}
	DsLibrary->SetVibration(values);
}
