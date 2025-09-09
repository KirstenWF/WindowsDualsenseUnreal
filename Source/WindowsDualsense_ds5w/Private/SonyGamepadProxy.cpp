// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "SonyGamepadProxy.h"
#include "Misc/CoreDelegates.h"
#include "Core/DeviceRegistry.h"
#include "Core/Interfaces/SonyGamepadInterface.h"

EDeviceType USonyGamepadProxy::GetDeviceType(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return EDeviceType::NotFound;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return EDeviceType::NotFound;
	}

	return Gamepad->GetDeviceType();
}

EDeviceConnection USonyGamepadProxy::GetConnectionType(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return EDeviceConnection::Unrecognized;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return EDeviceConnection::Unrecognized;
	}

	return Gamepad->GetConnectionType();
}

bool USonyGamepadProxy::DeviceIsConnected(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return false;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return false;
	}
	
	return true;
}

bool USonyGamepadProxy::DeviceDisconnect(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return false;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return true;
	}
	
	Gamepad->Disconnect();
	return true;
}

float USonyGamepadProxy::LevelBatteryDevice(int32 ControllerId)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return 0.0f;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return 0.0f;
	}
	
	return Gamepad->GetBattery();
}

void USonyGamepadProxy::LedColorEffects(int32 ControllerId, FColor Color, float BrightnessTime, float ToogleTime)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetLightbar(Color, BrightnessTime, ToogleTime);
}

void USonyGamepadProxy::LedMicEffects(int32 ControllerId, ELedMicEnum Value)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetMicrophoneLed(Value);
}

void USonyGamepadProxy::EnableTouch(int32 ControllerId, bool bEnableTouch)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetTouch(bEnableTouch);
}

void USonyGamepadProxy::EnableAccelerometerValues(int32 ControllerId, bool bEnableAccelerometer)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetAcceleration(bEnableAccelerometer);
}

void USonyGamepadProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	const FInputDeviceId DeviceId = GetGamepadInterface(ControllerId);
	if (!DeviceId.IsValid())
	{
		return;
	}
	
	ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(DeviceId.GetId());
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetGyroscope(bEnableGyroscope);
}

FInputDeviceId USonyGamepadProxy::GetGamepadInterface(int32 ControllerId)
{
	TArray<FInputDeviceId> Devices;
	Devices.Reset();
	
	IPlatformInputDeviceMapper::Get().GetAllInputDevicesForUser(FPlatformUserId::CreateFromInternalId(ControllerId), Devices);
	
	for (int32 i = 0; i < Devices.Num(); i++)
	{
		if (ISonyGamepadInterface* Gamepad = FDeviceRegistry::Get()->GetLibraryInstance(i))
		{
			return Gamepad->GetDeviceId();
		}
	}
	return FInputDeviceId::CreateFromInternalId(INDEX_NONE);
}
