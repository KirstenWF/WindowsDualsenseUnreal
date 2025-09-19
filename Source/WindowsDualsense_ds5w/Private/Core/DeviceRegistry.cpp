// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Core/DeviceRegistry.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Core/HIDPollingRunnable.h"
#include "Core/HIDDeviceInfo.h"
#include "Windows/WindowsApplication.h"
#include "Core/DualSense/DualSenseLibrary.h"
#include "Core/DualShock/DualShockLibrary.h"
#include "GameFramework/InputSettings.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "Core/Structs/FDeviceContext.h"
#include "Core/Structs/FOutputContext.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Core/Interfaces/SonyGamepadInterface.h"

TSharedPtr<FDeviceRegistry> FDeviceRegistry::Instance;
TMap<FString, FInputDeviceId> FDeviceRegistry::KnownDevicePaths;
TMap<FString, FInputDeviceId> FDeviceRegistry::HistoryDevices;
TMap<FInputDeviceId, ISonyGamepadInterface*> FDeviceRegistry::LibraryInstances;
TMap<int32, TUniquePtr<FHIDPollingRunnable>> FDeviceRegistry::ActiveConnectionWatchers;

bool PrimaryTick = true;
float AccumulateSecurity = 0;
void FDeviceRegistry::DetectedChangeConnections(float DeltaTime)
{
	AccumulateSecurity += DeltaTime;
	if (bIsDeviceDetectionInProgress && AccumulateSecurity >= 1.f)
	{
		bIsDeviceDetectionInProgress = false;
	}

	if (!PrimaryTick)
	{
		AccumulatorDelta += DeltaTime;
		if (AccumulatorDelta < 2.0f || bIsDeviceDetectionInProgress)
		{
			return;
		}

		AccumulatorDelta = 0.0f;
	}

	PrimaryTick = false;
	bIsDeviceDetectionInProgress = true;

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [WeakManager = AsWeak()]()
	{
			TArray<FDeviceContext> DetectedDevices;
			DetectedDevices.Reset();

			FHIDDeviceInfo::Detect(DetectedDevices);
			AsyncTask(ENamedThreads::GameThread, [WeakManager, DetectedDevices = MoveTemp(DetectedDevices)]() mutable
			{
				const TSharedPtr<FDeviceRegistry> Manager = WeakManager.Pin();
				if (!Manager)
				{
					return;
				}

				const TUniquePtr<FHIDDeviceInfo> HidManagerObj;
			
				TSet<FString> CurrentlyConnectedPaths;
				for (const FDeviceContext& Context : DetectedDevices)
				{
					CurrentlyConnectedPaths.Add(Context.Path);
				}

				TArray<FString> DisconnectedPaths;
				for (const TPair<FString, FInputDeviceId>& KnownDevice : Manager->KnownDevicePaths)
				{
					const FString& Path = KnownDevice.Key;
					const FInputDeviceId& DeviceId = KnownDevice.Value;
					if (!CurrentlyConnectedPaths.Contains(Path))
					{
						if (Manager->LibraryInstances.Contains(DeviceId))
						{
							IPlatformInputDeviceMapper::Get().Internal_SetInputDeviceConnectionState(DeviceId, EInputDeviceConnectionState::Disconnected);

							Manager->RemoveLibraryInstance(DeviceId.GetId());
							DisconnectedPaths.Add(Path);
						}
					}
				}

				for (const FString& Path : DisconnectedPaths)
				{
					if (Manager->KnownDevicePaths.Contains(Path))
					{
						Manager->KnownDevicePaths.Remove(Path);
					}
				}

				for (FDeviceContext& Context : DetectedDevices)
				{
					if (!Manager->KnownDevicePaths.Contains(Context.Path))
					{
						Context.Output = FOutputContext();
						Context.Handle = HidManagerObj->CreateHandle(&Context);

						if (Context.Handle == INVALID_HANDLE_VALUE)
						{
							continue;
						}

						Manager->CreateLibraryInstance(Context);
					}
				}
				
				Manager->bIsDeviceDetectionInProgress = false;
			});
	});
}

TSharedPtr<FDeviceRegistry> FDeviceRegistry::Get()
{
	if (!Instance)
	{
		check(IsInGameThread());
		Instance = MakeShared<FDeviceRegistry>();
	}
	return Instance;
}

FDeviceRegistry::~FDeviceRegistry()
{
	TArray<int32> WatcherKeys;
	ActiveConnectionWatchers.GetKeys(WatcherKeys);

	for (const int32 ControllerId : WatcherKeys)
	{
		RemoveLibraryInstance(ControllerId);
	}
}

ISonyGamepadInterface* FDeviceRegistry::GetLibraryInstance(const FInputDeviceId& DeviceId)
{
	if (!LibraryInstances.Contains(DeviceId))
	{
		return nullptr;
	}

	if (!LibraryInstances[DeviceId]->IsConnected())
	{
		return nullptr;
	}
	return LibraryInstances[DeviceId];
}

void FDeviceRegistry::RemoveLibraryInstance(int32 ControllerId)
{
	FInputDeviceId GamepadId = FInputDeviceId::CreateFromInternalId(ControllerId);
	if (
		IPlatformInputDeviceMapper::Get().GetInputDeviceConnectionState(GamepadId) !=
		EInputDeviceConnectionState::Disconnected)
	{
		IPlatformInputDeviceMapper::Get().Internal_SetInputDeviceConnectionState(
			GamepadId, EInputDeviceConnectionState::Disconnected);
	}

	if (!LibraryInstances.Contains(GamepadId))
	{
		return;
	}

	LibraryInstances[GamepadId]->ShutdownLibrary();
	LibraryInstances.Remove(GamepadId);

	const int32 NumRemoved = ActiveConnectionWatchers.Remove(ControllerId);
}

void FDeviceRegistry::CreateLibraryInstance(FDeviceContext& Context)
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
		return;
	}

	TArray<FInputDeviceId> Devices;
	Devices.Reset();
	
	IPlatformInputDeviceMapper::Get().GetAllInputDevicesForUser(
		IPlatformInputDeviceMapper::Get().GetPrimaryPlatformUser(), Devices);

	bool AllocateDeviceToDefaultUser = false;
	if (Devices.Num() <= 1)
	{
		AllocateDeviceToDefaultUser = true;
	}

	const FName UniqueNamespace = TEXT("DeviceManager.WindowsDualsense");
	const FHardwareDeviceIdentifier HardwareId(UniqueNamespace, Context.Path);
	if (HistoryDevices.Contains(Context.Path))
	{
		Context.UniqueInputDeviceId = HistoryDevices[Context.Path];
	}
	else
	{
		Context.UniqueInputDeviceId = IPlatformInputDeviceMapper::Get().AllocateNewInputDeviceId();
		HistoryDevices.Add(Context.Path, Context.UniqueInputDeviceId);
	}

	SonyGamepad->_getUObject()->AddToRoot();
	SonyGamepad->SetControllerId(Context.UniqueInputDeviceId.GetId());
	SonyGamepad->InitializeLibrary(Context);

	KnownDevicePaths.Add(Context.Path, Context.UniqueInputDeviceId);
	LibraryInstances.Add(Context.UniqueInputDeviceId, SonyGamepad);

	FInputDeviceId GamepadId = Context.UniqueInputDeviceId;
	if (
		IPlatformInputDeviceMapper::Get().GetInputDeviceConnectionState(GamepadId) !=
		EInputDeviceConnectionState::Connected)
	{
		FPlatformUserId UserId = IPlatformInputDeviceMapper::Get().GetUserForInputDevice(GamepadId);
		if (!UserId.IsValid())
		{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 6
			UserId = AllocateDeviceToDefaultUser
				? IPlatformInputDeviceMapper::Get().GetPrimaryPlatformUser()
				: IPlatformInputDeviceMapper::Get().AllocateNewUserId();

#else
			UserId = IPlatformInputDeviceMapper::Get().GetPlatformUserForNewlyConnectedDevice();
#endif
		}

		IPlatformInputDeviceMapper::Get().Internal_MapInputDeviceToUser(Context.UniqueInputDeviceId,
		                                                                UserId,
		                                                                EInputDeviceConnectionState::Connected);
	}


	if (ActiveConnectionWatchers.Contains(Context.UniqueInputDeviceId.GetId()))
	{
		ActiveConnectionWatchers.Remove(Context.UniqueInputDeviceId.GetId());
	}

	auto WatcherRunnable = MakeUnique<FHIDPollingRunnable>(
		MoveTemp(Context.Handle),
		std::chrono::milliseconds(150)
	);
	if (WatcherRunnable)
	{
		WatcherRunnable->StartThread();
		ActiveConnectionWatchers.Add(Context.UniqueInputDeviceId.GetId(), MoveTemp(WatcherRunnable));
	}
}

void FDeviceRegistry::RemoveAllLibraryInstance()
{
	for (const auto& LibraryInstance : LibraryInstances)
	{
		RemoveLibraryInstance(LibraryInstance.Key.GetId());
	}
	LibraryInstances.Reset();
}


int32 FDeviceRegistry::GetAllocatedDevices()
{
	return LibraryInstances.Num();
}

TMap<FInputDeviceId, ISonyGamepadInterface*> FDeviceRegistry::GetAllocatedDevicesMap()
{
	return LibraryInstances;
}
