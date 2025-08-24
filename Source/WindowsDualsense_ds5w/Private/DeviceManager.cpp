﻿// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "DeviceManager.h"
#include "Core/DeviceContainerManager.h"
#include "Core/Interfaces/SonyGamepadTriggerInterface.h"
#include "Windows/WindowsApplication.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "Misc/CoreDelegates.h"

class UDualSenseLibrary;

DeviceManager::DeviceManager(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler,
                             bool Lazily): MessageHandler(InMessageHandler)
{
	LazyLoading = Lazily;
	
	DeviceMapper = FWindowsPlatformApplicationMisc::CreatePlatformInputDeviceManager();
	FCoreDelegates::OnUserLoginChangedEvent.AddRaw(this, &DeviceManager::OnUserLoginChangedEvent);
	DeviceMapper->Get().GetOnInputDeviceConnectionChange().AddRaw(this, &DeviceManager::OnConnectionChange);
	DeviceMapper->Get().GetOnInputDevicePairingChange().AddRaw(this, &DeviceManager::OnChangedPairing);
}

DeviceManager::~DeviceManager()
{
	FCoreDelegates::OnUserLoginChangedEvent.RemoveAll(this);
	DeviceMapper->Get().GetOnInputDeviceConnectionChange().RemoveAll(this);
	DeviceMapper->Get().GetOnInputDevicePairingChange().RemoveAll(this);
}

void DeviceManager::Tick(float DeltaTime)
{
	if (LazyLoading)
	{
		return;
	}

	PollAccumulator += DeltaTime;
	if (PollAccumulator < PollInterval)
	{
		return;
	}
	
	PollAccumulator = 0.0f;
	
	TArray<FInputDeviceId> OutInputDevices;
	OutInputDevices.Reset();

	DeviceMapper->Get().GetAllConnectedInputDevices(OutInputDevices);
	for (const FInputDeviceId& DeviceId : OutInputDevices)
	{
		FInputDeviceId Device = FInputDeviceId::CreateFromInternalId(DeviceId.GetId());
		FPlatformUserId UserId = IPlatformInputDeviceMapper::Get().GetUserForInputDevice(Device);
		if (const int32 ControllerId = FPlatformMisc::GetUserIndexForPlatformUser(UserId); ControllerId == -1)
		{
			continue;
		}

		ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(DeviceId.GetId());
		if (!Gamepad)
		{
			Disconnect(DeviceId);
			continue;
		}
		
		if (!UDeviceContainerManager::Get()->GetSettings().Contains(DeviceId.GetId()))
		{
			Disconnect(DeviceId);
			continue;
		}

		FString ContextDrive = TEXT("DualSense");
		if (Gamepad->GetDeviceType() == EDeviceType::DualShock4)
		{
			ContextDrive = TEXT("DualShock4");
		}
		if (Gamepad->GetDeviceType() == EDeviceType::DualSenseEdge)
		{
			ContextDrive = TEXT("DualSenseEdge");
		}
	
		FInputDeviceScope InputScope(this, TEXT("DeviceManager"), Device.GetId(), ContextDrive);
		if (!Gamepad->UpdateInput(MessageHandler, UserId, Device))
		{
			Disconnect(DeviceId);
			UDeviceContainerManager::Get()->RemoveLibraryInstance(DeviceId.GetId());
		}
		UE_LOG(LogTemp, Log, TEXT("DeviceManager::Tick() - DeviceId: %d"), DeviceId.GetId());
		UE_LOG(LogTemp, Log, TEXT("DeviceManager::Tick() - UserID: %d"), UserId.GetInternalId());
	}
}

void DeviceManager::SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property)
{
	if (LazyLoading || !Property) return;

	if (Property->Name == "InputDeviceLightColor")
	{
		const FInputDeviceLightColorProperty* ColorProperty = static_cast<const FInputDeviceLightColorProperty*>(Property);
		SetLightColor(ControllerId, ColorProperty->Color);
	}

	if (Property->Name == FName("InputDeviceTriggerResistance"))
	{
		ISonyGamepadTriggerInterface* GamepadTrigger = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
		if (!IsValid(GamepadTrigger->_getUObject()))
		{
			return;
		}
		GamepadTrigger->SetTriggers(Property);
	}
}

void DeviceManager::SetHapticFeedbackValues(const int32 ControllerId, const int32 Hand, const FHapticFeedbackValues& Values)
{
	if (LazyLoading) return;

	ISonyGamepadTriggerInterface* GamepadTrigger = Cast<ISonyGamepadTriggerInterface>(UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId));
	if (!IsValid(GamepadTrigger->_getUObject()))
	{
		return;
	}
	
	GamepadTrigger->SetHapticFeedback(Hand, &Values);
}

void DeviceManager::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values)
{
	if (LazyLoading) return;

	ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId);
	if (!Gamepad)
	{
		return;
	}
	
	Gamepad->SetVibration(Values);
}

void DeviceManager::SetLightColor(const int32 ControllerId, const FColor Color)
{
	if (LazyLoading) return;

	ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId);
	if (!Gamepad)
	{
		return;
	}

	Gamepad->SetLightbar(Color);
}

void DeviceManager::ResetLightColor(const int32 ControllerId)
{
	if (LazyLoading) return;

	ISonyGamepadInterface* Gamepad = UDeviceContainerManager::Get()->GetLibraryInstance(ControllerId);
	if (!Gamepad)
	{
		return;
	}
	
	Gamepad->SetLightbar(FColor::Blue);
}

void DeviceManager::Reconnect(const FInputDeviceId& Device) const
{
	if (LazyLoading) return;

	if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) == EInputDeviceConnectionState::Disconnected)
	{
		DeviceMapper->Get().Internal_SetInputDeviceConnectionState(Device, EInputDeviceConnectionState::Connected);
	}
}

void DeviceManager::Disconnect(const FInputDeviceId& Device) const
{
	if (LazyLoading) return;

	if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) == EInputDeviceConnectionState::Connected)
	{
		DeviceMapper->Get().Internal_SetInputDeviceConnectionState(Device, EInputDeviceConnectionState::Disconnected);
	}
}

void DeviceManager::OnConnectionChange(EInputDeviceConnectionState Connected, FPlatformUserId PlatformUserId,
											  FInputDeviceId InputDeviceId) const
{
	const bool bIsConnected = (Connected == EInputDeviceConnectionState::Connected);
	if (DeviceMapper->Get().GetInputDeviceConnectionState(InputDeviceId) != EInputDeviceConnectionState::Connected &&
		bIsConnected)
	{
		DeviceMapper->Get().Internal_MapInputDeviceToUser(InputDeviceId, PlatformUserId, EInputDeviceConnectionState::Connected);
		return;
	}

	if (DeviceMapper->Get().GetInputDeviceConnectionState(InputDeviceId) == EInputDeviceConnectionState::Connected &&
		!bIsConnected)
	{
		DeviceMapper->Get().Internal_MapInputDeviceToUser(InputDeviceId, PlatformUserId, EInputDeviceConnectionState::Disconnected);
	}
}

bool bIsChange = false;
void DeviceManager::OnChangedPairing(const FInputDeviceId ControllerId, const FPlatformUserId NewUser, const FPlatformUserId OldUer) const
{
	if (bIsChange)
	{
		return;
	}
	bIsChange = true;
	
	DeviceMapper->Get().Internal_ChangeInputDeviceUserMapping(ControllerId, NewUser, OldUer);
	DeviceMapper->Get().Internal_SetInputDeviceConnectionState(ControllerId, EInputDeviceConnectionState::Connected);
	bIsChange = false;
}
void DeviceManager::OnUserLoginChangedEvent(bool bLoggedIn, int32 UserId, int32 UserIndex) const
{
	if (!bLoggedIn) return;

	const FInputDeviceId Device = FInputDeviceId::CreateFromInternalId(UserId);
	const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(UserIndex);
	
	if (const FPlatformUserId& UserPair = DeviceMapper->Get().GetUserForInputDevice(Device); UserPair != User)
	{
		if (DeviceMapper->Get().GetInputDeviceConnectionState(Device) != EInputDeviceConnectionState::Connected)
		{
			DeviceMapper->Get().Internal_MapInputDeviceToUser(Device, User, EInputDeviceConnectionState::Connected);
		}
	}
}
