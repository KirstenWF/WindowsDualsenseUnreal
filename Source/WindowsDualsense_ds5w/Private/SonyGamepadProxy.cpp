// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "SonyGamepadProxy.h"
#include "Core/DeviceContainerManager.h"


class UDeviceSettings;

void USonyGamepadProxy::RemapControllerIdToUser(int32 GamepadId, int32 UserId, int32 OldUser)
{
	if (TMap<int32, int32> MapUsers = UDeviceContainerManager::GetSettings(); MapUsers.Contains(UserId))
	{
		const int32 OldDevice = MapUsers[UserId];
		UDeviceContainerManager::Get()->SetSettings(OldUser, OldDevice);
	}
	
	UDeviceContainerManager::Get()->SetSettings(UserId, GamepadId);

	const FPlatformUserId User = FPlatformUserId::CreateFromInternalId(UserId);
	const FPlatformUserId Old  = FPlatformUserId::CreateFromInternalId(OldUser);
	const FInputDeviceId Device = FInputDeviceId::CreateFromInternalId(GamepadId);
	PlatformInputDeviceMapper.Get().GetOnInputDevicePairingChange().Broadcast(Device, User, Old);
}

bool USonyGamepadProxy::DeviceIsConnected(int32 ControllerId)
{
	if (const ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryOrReconnect(ControllerId); !Gamepad)
	{
		return false;
	}

	PlatformInputDeviceMapper.Get().GetOnInputDeviceConnectionChange().Broadcast(
		EInputDeviceConnectionState::Connected,
		FPlatformUserId::CreateFromInternalId(UDeviceContainerManager::ToMap(ControllerId)),
		FInputDeviceId::CreateFromInternalId(UDeviceContainerManager::ToMap(ControllerId))
	);
	return true;
}

EDeviceType USonyGamepadProxy::GetDeviceType(int32 ControllerId)
{
	ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId);
	if (!Gamepad)
	{
		return EDeviceType::NotFound;
	}

	return Gamepad->GetDeviceType();
}

EDeviceConnection USonyGamepadProxy::GetConnectionType(int32 ControllerId)
{
	ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId);
	if (!Gamepad)
	{
		return EDeviceConnection::Unrecognized;
	}

	return Gamepad->GetConnectionType();
}

bool USonyGamepadProxy::DeviceReconnect(int32 ControllerId)
{
	if (const ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryOrReconnect(ControllerId); !Gamepad)
	{
		return false;
	}

	return true;
}

bool USonyGamepadProxy::DeviceDisconnect(int32 ControllerId)
{
	UDeviceContainerManager::Get()->RemoveLibraryInstance(UDeviceContainerManager::ToMap(ControllerId));
	return true;
}

float USonyGamepadProxy::LevelBatteryDevice(int32 ControllerId)
{
	ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId);
	if (!Gamepad)
	{
		return 0.0f;
	}

	return Gamepad->GetBattery();
}

void USonyGamepadProxy::LedColorEffects(int32 ControllerId, FColor Color, float BrightnessTime, float ToogleTime)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!Gamepad)
	{
		return;
	}

	Gamepad->Get()->SetLightbar(Color, BrightnessTime, ToogleTime);
}

void USonyGamepadProxy::LedMicEffects(int32 ControllerId, ELedMicEnum Value)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetMicrophoneLed(Value);
}

void USonyGamepadProxy::EnableTouch(int32 ControllerId, bool bEnableTouch)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetTouch(bEnableTouch);
}

void USonyGamepadProxy::EnableAccelerometerValues(int32 ControllerId, bool bEnableAccelerometer)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetAcceleration(bEnableAccelerometer);
}

void USonyGamepadProxy::EnableGyroscopeValues(int32 ControllerId, bool bEnableGyroscope)
{
	ISonyGamepadInterface* Gamepad = Cast<ISonyGamepadInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetGyroscope(bEnableGyroscope);
}
