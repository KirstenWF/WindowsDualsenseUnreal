// Copyright (c) 2025 Rafael Valoto. All Rights Reserved.
// Project: GamepadCore
// Description: Cross-platform library for DualSense and generic gamepad input support.
// Targets: Windows, Linux, macOS.
#pragma once
#include "GCore/Templates/TGenericHardwareInfo.h"
#include "WindowsDeviceInfo.h"
#include <set>
#ifdef _WIN32
namespace FWindowsPlatform
{
	struct FWindowsHardwarePolicy;
	using FWindowsHardware = GamepadCore::TGenericHardwareInfo<FWindowsHardwarePolicy>;

	/**
	 * @brief Global registry to track which audio devices are currently in use
	 * to ensure multiple DualSense controllers get separate audio channels.
	 */
	struct FAudioDeviceRegistry
	{
		static FAudioDeviceRegistry& Get()
		{
			static FAudioDeviceRegistry Instance;
			return Instance;
		}

		void RegisterDevice(const ma_device_id& DeviceId)
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			UsedDeviceIds.insert(DeviceId);
		}

		void UnregisterDevice(const ma_device_id& DeviceId)
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			UsedDeviceIds.erase(DeviceId);
		}

		bool IsDeviceInUse(const ma_device_id& DeviceId)
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			return UsedDeviceIds.find(DeviceId) != UsedDeviceIds.end();
		}

	private:
		std::mutex Mutex;
		struct DeviceIdCompare
		{
			bool operator()(const ma_device_id& lhs, const ma_device_id& rhs) const
			{
				return std::memcmp(&lhs, &rhs, sizeof(ma_device_id)) < 0;
			}
		};
		std::set<ma_device_id, DeviceIdCompare> UsedDeviceIds;
	};

	struct FWindowsHardwarePolicy
	{
	public:
		void Read(FDeviceContext* Context)
		{
			FWindowsDeviceInfo::Read(Context);
		}

		void Write(FDeviceContext* Context)
		{
			FWindowsDeviceInfo::Write(Context);
		}

		void Detect(std::vector<FDeviceContext>& Devices)
		{
			FWindowsDeviceInfo::Detect(Devices);
		}

		bool CreateHandle(FDeviceContext* Context)
		{
			return FWindowsDeviceInfo::CreateHandle(Context);
		}

		void InvalidateHandle(FDeviceContext* Context)
		{
			if (Context && Context->AudioContext)
			{
				if (Context->AudioContext->bHasDeviceId)
				{
					FAudioDeviceRegistry::Get().UnregisterDevice(Context->AudioContext->DeviceId);
				}
			}
			FWindowsDeviceInfo::InvalidateHandle(Context);
		}

		void ProcessAudioHaptic(FDeviceContext* Context)
		{
			FWindowsDeviceInfo::ProcessAudioHaptic(Context);
		}

		/**
		 * @brief Initializes the audio device for a DualSense controller.
		 *
		 * Enumerates available audio playback devices, searches for one matching
		 * the DualSense controller (by name containing "DualSense" or "Wireless Controller"),
		 * and initializes the FAudioDeviceContext with the found device.
		 *
		 * @param Context The device context to store the audio device in
		 */
		void InitializeAudioDevice(FDeviceContext* Context)
		{
			if (!Context)
			{
				return;
			}

			// Initialize miniaudio context for device enumeration
			ma_context maContext;
			if (ma_context_init(nullptr, 0, nullptr, &maContext) != MA_SUCCESS)
			{
				return;
			}

			// Get playback devices
			ma_device_info* pPlaybackInfos;
			ma_uint32 playbackCount;
			ma_device_info* pCaptureInfos;
			ma_uint32 captureCount;

			if (ma_context_get_devices(&maContext, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount) != MA_SUCCESS)
			{
				ma_context_uninit(&maContext);
				return;
			}

			// Get Gamepad Container ID
			std::string GamepadContainerId = FWindowsDeviceInfo::GetContainerId(Context->Path);

			// Search for DualSense audio device
			ma_device_id* pFoundDeviceId = nullptr;
			ma_device_id foundDeviceId;

			for (ma_uint32 i = 0; i < playbackCount; i++)
			{
				std::string deviceName(pPlaybackInfos[i].name);

				// Check if device name contains DualSense identifiers
				// DualSense appears as "Wireless Controller" or "DualSense Wireless Controller"
				if (deviceName.find("DualSense") != std::string::npos ||
				    deviceName.find("Wireless Controller") != std::string::npos)
				{
					// Match by Container ID if possible
					if (!GamepadContainerId.empty())
					{
						std::string AudioContainerId = FWindowsDeviceInfo::GetAudioContainerId(pPlaybackInfos[i].id.wasapi);
						if (AudioContainerId != GamepadContainerId)
						{
							continue;
						}
					}

					// Check if this device is already in use by another controller
					if (!FAudioDeviceRegistry::Get().IsDeviceInUse(pPlaybackInfos[i].id))
					{
						// Verify it has 4 channels (stereo + haptic L/R)
						// DualSense audio device typically has 4 channels for haptics
						foundDeviceId = pPlaybackInfos[i].id;
						pFoundDeviceId = &foundDeviceId;

						// Mark as used
						FAudioDeviceRegistry::Get().RegisterDevice(foundDeviceId);
						break;
					}
				}
			}

			Context->AudioContext = std::make_shared<FAudioDeviceContext>();
			if (pFoundDeviceId)
			{
				Context->AudioContext->InitializeWithDeviceId(pFoundDeviceId, 48000, 4);
			}

			ma_context_uninit(&maContext);
		}
	};
} // namespace FWindowsPlatform
#endif
