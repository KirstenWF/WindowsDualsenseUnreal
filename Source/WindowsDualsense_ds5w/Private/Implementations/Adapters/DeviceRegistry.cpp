// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Implementations/Adapters/DeviceRegistry.h"
#include "GCore/Interfaces/ISonyGamepad.h"

TUniquePtr<FDeviceRegistry> FDeviceRegistry::Instance;
std::unique_ptr<FDeviceRegistry::FRegistryLogic> FDeviceRegistry::RegistryImplementation = nullptr;

FDeviceRegistry::~FDeviceRegistry()
{
}

FDeviceRegistry::FDeviceRegistry()
{
	RegistryImplementation = std::make_unique<FRegistryLogic>();
}

void FDeviceRegistry::Initialize()
{
	if (!Instance)
	{
		check(IsInGameThread());
		Instance.Reset(new FDeviceRegistry());
	}
}

void FDeviceRegistry::Shutdown()
{
	Instance.Reset();
}

FDeviceRegistry* FDeviceRegistry::Get()
{
	check(IsInGameThread());
	return Instance.Get();
}

void FDeviceRegistry::DiscoverDevices(float DeltaTime)
{
	if (RegistryImplementation)
	{
		return RegistryImplementation->PlugAndPlay(DeltaTime);
	}
}

ISonyGamepad* FDeviceRegistry::GetLibraryInstance(FInputDeviceId DeviceId)
{
	if (RegistryImplementation)
	{
		if (ISonyGamepad* Lib = RegistryImplementation->GetLibrary(DeviceId))
		{
			return Lib;
		}
		RegistryImplementation->RemoveLibraryInstance(DeviceId);
	}
	return nullptr;
}

void FDeviceRegistry::RequestImmediateDetection()
{
	if (RegistryImplementation)
	{
		RegistryImplementation->RequestImmediateDetection();
	}
}
