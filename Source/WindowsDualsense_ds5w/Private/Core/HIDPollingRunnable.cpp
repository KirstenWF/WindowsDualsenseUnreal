// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Core/HIDPollingRunnable.h"
#include "Core/HIDDeviceInfo.h"
#include "HAL/PlatformProcess.h"
#include <thread>

FHIDPollingRunnable::FHIDPollingRunnable(HANDLE InDeviceHandle, const std::chrono::milliseconds InInterval):
	DeviceHandle(InDeviceHandle),
	Thread(nullptr),
	Interval(InInterval)
{
}

FHIDPollingRunnable::~FHIDPollingRunnable()
{
	Stop();
	
	if (Thread)
	{
		Thread->WaitForCompletion();
		
		delete Thread;
		Thread = nullptr;
	}
}

bool FHIDPollingRunnable::Init()
{
	return DeviceHandle != INVALID_HANDLE_VALUE;
}

uint32 FHIDPollingRunnable::Run()
{
	using FClock = std::chrono::steady_clock;
	auto NextPingTime = FClock::now() + Interval;

	while (!bStopRequested.load(std::memory_order_relaxed))
	{
		DWORD LastError = ERROR_SUCCESS;
		if (!FHIDDeviceInfo::PingOnce(DeviceHandle, &LastError) && FHIDDeviceInfo::ShouldTreatAsDisconnected(LastError))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ping failed: device is no longer connected. Shutting down the ping"));
			return 1;
		}

		std::this_thread::sleep_until(NextPingTime);

		do
		{
			NextPingTime += Interval;
		} while (NextPingTime <= FClock::now());
	}

	return 0;
}

void FHIDPollingRunnable::Stop()
{
	bStopRequested.store(true, std::memory_order_relaxed);
}

void FHIDPollingRunnable::StartThread()
{
	const FString ThreadName = FString::Printf(TEXT("FHIDPollingRunnable_%p"), this);
	Thread = FRunnableThread::Create(this, *ThreadName);
}

void FHIDPollingRunnable::Exit()
{
	UE_LOG(LogTemp, Log, TEXT("Thread is exiting, cleaning up buffer."));
}
