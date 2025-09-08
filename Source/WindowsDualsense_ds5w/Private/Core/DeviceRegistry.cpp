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
#include "Runtime/ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "Core/Structs/FDeviceContext.h"
#include "Core/Structs/FOutputContext.h"
#include "Core/Interfaces/SonyGamepadInterface.h"

UDeviceRegistry* UDeviceRegistry::Instance;
TMap<FString, FInputDeviceId> UDeviceRegistry::KnownDevicePaths;
TMap<FInputDeviceId, ISonyGamepadInterface*> UDeviceRegistry::LibraryInstances;
TMap<FString, TPair<FInputDeviceId, FPlatformUserId>> UDeviceRegistry::HistoryDevices;
TMap<int32, TUniquePtr<FHIDPollingRunnable>> UDeviceRegistry::ActiveConnectionWatchers;

float UDeviceRegistry::AccumulatorDelta = 0.0f;
bool UDeviceRegistry::bIsDeviceDetectionInProgress = false;

bool PrimaryTick = true;
float AccumulateSecurity = 0;
void UDeviceRegistry::DetectedChangeConnections(float DeltaTime)
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

	TWeakObjectPtr<UDeviceRegistry> WeakManager = Get();
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [WeakManager]()
	{
			const TUniquePtr<FHIDDeviceInfo> DeviceInfo = MakeUnique<FHIDDeviceInfo>();
		
			TArray<FDeviceContext> DetectedDevices;
			DetectedDevices.Reset();

			DeviceInfo->Detect(DetectedDevices);
			
			AsyncTask(ENamedThreads::GameThread, [WeakManager, DetectedDevices = MoveTemp(DetectedDevices)]() mutable
			{
				if (!WeakManager.IsValid())
				{
					return;
				}

				const UDeviceRegistry* Manager = WeakManager.Get();
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

UDeviceRegistry* UDeviceRegistry::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UDeviceRegistry>();
		Instance->AddToRoot();
	}
	return Instance;
}

UDeviceRegistry::~UDeviceRegistry()
{
	TArray<int32> WatcherKeys;
	ActiveConnectionWatchers.GetKeys(WatcherKeys);

	for (const int32 ControllerId : WatcherKeys)
	{
		RemoveLibraryInstance(ControllerId);
	}
}

ISonyGamepadInterface* UDeviceRegistry::GetLibraryInstance(int32 ControllerId)
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

void UDeviceRegistry::RemoveLibraryInstance(int32 ControllerId)
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

void UDeviceRegistry::CreateLibraryInstance(FDeviceContext& Context)
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

	const FName UniqueNamespace = TEXT("DeviceManager.WindowsDualsense");
	const FHardwareDeviceIdentifier HardwareId(UniqueNamespace, Context.Path);
	if (HistoryDevices.Contains(Context.Path))
	{
		Context.UniqueInputDeviceId = HistoryDevices[Context.Path].Key;
		Context.UniquePlatformUserId = HistoryDevices[Context.Path].Value;
	}
	else
	{
		const FInputDeviceId NewDeviceId = IPlatformInputDeviceMapper::Get().AllocateNewInputDeviceId();
		const FPlatformUserId NewUserId = IPlatformInputDeviceMapper::Get().AllocateNewUserId();
		Context.UniqueInputDeviceId = NewDeviceId;
		Context.UniquePlatformUserId = NewUserId;

		HistoryDevices.Add(Context.Path, TPair<FInputDeviceId, FPlatformUserId>(NewDeviceId, NewUserId));
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
		IPlatformInputDeviceMapper::Get().Internal_MapInputDeviceToUser(Context.UniqueInputDeviceId,
		                                                                Context.UniquePlatformUserId,
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

void UDeviceRegistry::RemoveAllLibraryInstance()
{
	for (const auto& LibraryInstance : LibraryInstances)
	{
		RemoveLibraryInstance(LibraryInstance.Key.GetId());
	}
	LibraryInstances.Reset();
}


int32 UDeviceRegistry::GetAllocatedDevices()
{
	return LibraryInstances.Num();
}

TMap<FInputDeviceId, ISonyGamepadInterface*> UDeviceRegistry::GetAllocatedDevicesMap()
{
	return LibraryInstances;
}
