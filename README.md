<h1 align="center">Dualsense Unreal Plugin</h1>

<p align="center">
Integrate all the features of Sony's DualSense™ and DualShock 4® controllers into your Unreal Engine project.
<br />
<br />
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/issues">Report Bug</a>
·
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/issues">Suggest a Feature
</a>
·
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/wiki">Documentation
</a>
</p>

<p align="center">
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/releases">
    <img src="https://img.shields.io/github/v/release/rafaelvaloto/Unreal-Dualsense?style=for-the-badge&logo=github" alt="Latest Release">
</a>
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/rafaelvaloto/Unreal-Dualsense?style=for-the-badge" alt="License">
</a>
<img src="https://img.shields.io/badge/Unreal%20Engine-5.2+-blue?style=for-the-badge&logo=unrealengine" alt="Unreal Engine 5.x">
<br/>
<img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Platform: Windows">
<img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" alt="Platform: Linux">
<img src="https://img.shields.io/badge/macOS-000000?style=for-the-badge&logo=apple&logoColor=white" alt="Platform: macOS">
<img src="https://img.shields.io/badge/PlayStation-0070D1?style=for-the-badge&logo=playstation&logoColor=white" alt="Platform: PlayStation">
</p>

<p align="center">
  <a href="#-getting-started">Getting Started</a> •
  <a href="#-basic-usage">Basic Usage</a> •
  <a href="#-example-project-arena-shooter-ue-56">Example Project</a> •
<a href="#-live-haptic-prototyping-console-to-blueprint">Prototyping</a> •
  <a href="#extending-for-other-platforms-eg-playstation">Extending</a> •
<a href="#-injecting-custom-device-logic-custom-devicemanager">Injecting Custom Device</a>
  <br>
  <br>
  <a href="#-core-maintainers">💖 Why should your Studio sponsor this project? 💖</a>
	<br>
</p>

> [!IMPORTANT]
> **v2 is now available!**
> 
> You can now extend the plugin to implement custom logic for the **Gyroscope, Accelerometer, and Touchpad**, or even integrate native Unreal Engine assets. 
> - **Extensible Architecture:** The plugin features a pre-configured IMU filter that can be fully overridden.
> - **Custom Implementation:** Tailor the device behavior to your project's needs in the [Customization Section](#-injecting-custom-device-logic-custom-devicemanager).
> 
> 🔄 **Upgrading from v1.x?** Please read our [Migration Guide](https://github.com/rafaelvaloto/Unreal-Dualsense/wiki/Migration-Guide:-Unreal%E2%80%90Dualsense-v1.x-to-v2.0).


## 📖 About the Project

Built with a cross-platform architecture, this Unreal Engine plugin provides a unified solution for integrating DualSense™ (PlayStation 5) and DualShock 4® (PlayStation 4) controllers. It delivers native support on PC while being architected for easy compilation across other Unreal Engine platforms, including PlayStation®. This asset provides direct API access to the complete feature set of each controller, including the revolutionary Haptic Feedback and Adaptive Triggers of the DualSense™. All features are exposed through a clean and well-documented function library for both Blueprint and C++.

Designed to bridge the gap left by generic controller support, this asset empowers developers to implement the high-fidelity, immersive feedback that makes Sony's controllers unique.

## ✨ Features
* 🏗️ **Extensible Multi-Platform Architecture**: Its flexible design enables compilation across all Unreal Engine platforms. Supporting new hardware is as simple as implementing the connection interface.
* 🔌 **Dynamic Connection (Hot-Swap)**: Automatically detects controller connection and disconnection, even during gameplay.
* ⚡ **Optimized for Multiplayer**: High-performance architecture with minimal impact on network games.
* 🎮 **Seamless Input Integration**: Coexists perfectly with Unreal Engine's native input managers (like Enhanced Input) and other gamepad plugins, preventing conflicts between devices.
* 🎧 **Audio Haptics (USB & Wireless)**: Haptic feedback based on in-game audio.
* 🎯 **Adaptive Triggers**: Full control over resistance, effect, and vibration on R2/L2 triggers.
* 💡 **Lightbar Control**: Dynamically change the controller's LED color.
* 🎤 **Smart Mute Detection** — Automatic mute LED control, no coding required
* ⚙️ **Force Feedback**: Native integration with Unreal Engine's Force Feedback system for standard motor vibration.
* 🎮 **Multi-Controller Support**: Manage up to 4 controllers simultaneously.

## 🚀 Getting Started

### Prerequisites

* **Unreal Engine**: 5.2 or higher (Plugin uses C++20 features).
* **Operating System**: Windows 10 or 11.
* **Controller**: DualSense™ or DualShock 4®.

### Quick Installation

1.  Go to the official plugin page on the Unreal Engine Marketplace (FAB): [Plugin Page - FAB](https://www.fab.com/listings/cbc8b9df-12ea-4c52-949c-7ab79aa93928)
2.  Click **Install** or **Add to Project** and select your Unreal Engine project.
3.  Activate the plugin in Unreal Engine:
    * Open your project.
    * Go to `Edit > Plugins`.
    * Search for **Dualsense** and check the box.
4.  Restart Unreal Engine when prompted.

### Manual Installation

To ensure the plugin compiles correctly within Unreal Engine, you must configure the [GamepadCore](https://github.com/rafaelvaloto/GamepadCore_) submodule.

Please run the following commands in your terminal (Git Bash, PowerShell, or CMD):
```bash
# Clone the repository at version (tag)
git clone -b [tag] --single-branch https://github.com/rafaelvaloto/Unreal-Dualsense.git
```
```bash
# Clone the repository at master branch
git clone https://github.com/rafaelvaloto/Unreal-Dualsense.git

# Enter the repository folder
cd Unreal-Dualsense

# Init the submodule to the latest version
git submodule update --init

# Init the submodule miniaudio
git -C Source/WindowsDualsense_ds5w/Private/GamepadCore submodule update --init Libs/miniaudio
```
    
## 💻 Basic Usage

The plugin exposes all functionality through static Blueprint function libraries, meaning you can call methods from anywhere without needing to add components.

> [!IMPORTANT]
> **New in v2: Update Output Node**
>
> In version 2.x, after configuring any controller effects (such as Lightbar, Adaptive Triggers, or Mic LED), you **must** call the **Update Output** node to apply these changes to the controller. This optimization allows you to batch multiple effect changes and send them in a single update.
>
> **Example:** Set Lightbar Color ⮕ Set Weapon Effect (Trigger) ⮕ **Update Output**.

### Blueprint Function Libraries

The functions are divided into two main categories for easy access:

* **Sony Gamepad**: Contains management methods common to Sony controllers (DualShock and DualSense), such as LED control, gyroscope, battery level, etc.
* **DualSense Effects**: Contains methods specific to DualSense exclusive features, such as Adaptive Triggers configuration.

Call functions directly to control DualSense features. Some available effects include:

* 🐎 **Galloping**: Simulates a horse's trot.
* 💪 **Resistance**: Applies constant opposing force when pressing the trigger.
* 🔫 **Weapon**: Creates a recoil effect for semi-automatic weapons.
* 🔥 **Automatic Gun**: Vibrates rapidly to simulate an automatic weapon.

### 📚 For the full documentation, please see the **[Wiki](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki)**.


## 🎮 Example Project: Arena Shooter UE 5.6

To demonstrate the practical use of the **Dualsense Unreal Plugin**, a sample project has been developed using the [*Arena Shooter* template](https://www.unrealengine.com/marketplace/en-US/product/arena-shooter-template) and upgraded to Unreal Engine 5.6. This project integrates key features of the DualSense controller to enhance the player's experience.

🎬 **Gyroscope Demo:** Watch the new aiming mechanics in action on **[YouTube](https://youtu.be/r_NhoODeELU?si=IdqRFx8_aimeIToD)**.
  
![Arena Shooter UE 5.6](Images/ArenaBanner.jpg)

### Implemented Features

In this sample, the following DualSense functionalities were integrated to provide a more immersive gameplay experience:

* 🔫 **Automatic Gun**: Experience fully automatic firing with appropriate haptic feedback, simulating realistic weapon recoil and vibration.
* 🎯 **Semi-Automatic Gun**: Engage in precise shooting with semi-automatic weapons, where each shot provides distinct haptic sensations and trigger resistance.
* 💥 **Vibration on Player Hit & Visual Feedback**: Feel the impact! The controller vibrates dynamically when the player takes damage, complemented by on-screen visual feedback for enhanced immersion and immediate awareness.
* 💡 **LED Color Change on Player Hit**: The DualSense controller's LED light dynamically changes color when the player is hit, offering an immediate and intuitive visual cue of damage taken, enhancing situational awareness.

### Where to Download

You can download the *Arena Shooter UE 5.6* with the DualSense integration directly from link.

- [**Download the example project v2.0.0 for the editor here**](https://drive.google.com/file/d/11iUQuWwA4zkFI_eP0roYbTDh0ss8614m/view?usp=drive_link)

### 🎓 Hands-On Tutorial

We've created a detailed, step-by-step tutorial that breaks down the entire implementation within the example project. It's the perfect guide to get you started.

* 🎯 **Gyroscope Aiming**: A complete tutorial on how to implement a precise and responsive Aim Down Sights (ADS) gyro aiming system using the Arena Shooter template.
    ➡️ **[[Read the Gyroscope Aiming Tutorial]](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/Example-Project:-Gyroscope-Aiming)**

* 🔫 **Arena Shooter**: An example using the Arena Shooter template that implements adaptive triggers for automatic/semi-automatic weapons and haptic feedback for player damage.
    ➡️ **[[Read the Arena Shooter Tutorial]](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/Example-Project:-Arena-Shooter-Tutorial)**


## 🎮 Example Project: Audio Haptics (USB & Wireless)

Take your immersion to the next level! This update enhances the advanced Audio Haptics feature, allowing it to work seamlessly via both USB and wirelessly via Bluetooth (previously USB-only). This new example project demonstrates how to harness this power, featuring a complete implementation for real-time haptic feedback based on in-game audio.

![Audio Haptics (USB & Wireless)](Images/audio-haptics-bluetooth-and-usb-ds.png)

### Implemented Features

This sample project serves as a practical guide and includes:

* **🎧 Flexible Submix Listener:** Learn how to register a listener (RegisterSubmixForDevice) on an Unreal Engine Sound Submix that processes audio for both wired and wireless connections.

* **⚡️ Real-time Audio Processing:** We capture the audio data directly from the submix, resample it, and send it to the DualSense controller over your active connection (USB or Bluetooth) in real-time.

* **🎛️ Haptic FX Menu Widget:** A new sample UI (UMG Widget) is provided that allows you to:

Select different Sound Classes to be routed to the haptics system.

Play various "playback albums" (sets of Sound Cues) to test and feel a wide variety of haptic effects based on different sounds.

🛠️ New Developer/Debug Tools: For advanced users, new console commands have been added to test and fine-tune trigger vibrations and frequencies directly over wireless and wired connections (e.g., ds.SetAudioLR). 

- [**Download the example project for the editor here**](https://drive.google.com/file/d/1lGUBfz42gnh2bSIJTptC_LqOdFHsPuSi/view?usp=sharing)


## 🚀 Live Haptic Prototyping (Console to Blueprint)

You can now discover, test, and implement advanced trigger effects with a new, highly efficient workflow.

**1. Test Live in Console:** Fine-tune adaptive trigger effects directly in the Unreal Engine console. This is the fastest way to prototype and debug haptic sensations without recompiling. Use the `ds.SetTrigL` and `ds.SetTrigR` commands to send raw 10-byte HEX arrays until you discover the perfect effect.

**2. Store and Reuse:** Once you have your ideal HEX values, don't hard-code them! Store them in a reusable **Data Table** to be called from any Blueprint using the `Custom Trigger` node.

This complete workflow—from live console discovery to clean Blueprint implementation—is covered in our new Wiki guides:

* ➡️ **[Wiki Page: Console Commands & HEX Reference](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/%F0%9F%8E%AE-DualSense-Trigger-Effects:-Console-Commands-&-HEX-Reference)**
    * (Learn to *find and test* effects using the console)
* ➡️ **[Wiki Page: Tutorial: Creating a Reusable Trigger Effect Data Table](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/%F0%9F%8E%93-Tutorial:-Creating-a-Reusable-Trigger-Effect-Data-Table)**
    * (Learn to *store and use* your effects in Blueprints)


## 💉 Injecting Custom Device Logic (Custom DeviceManager)
Since version 2.0.0, you can also inject a custom implementation of the `DeviceManager`. This is useful if you want to implement your own input buffering, custom button mapping, or specialized haptic logic without modifying the plugin source.

> [!TIP]
> To ensure that your custom implementation works with native Unreal Engine assets (like **Haptic Feedback Effects**, **Force Feedback Assets**, and **Device Properties**), your class must correctly implement or override the methods from `IInputDevice` and `IHapticDevice`.

### Required Interfaces for Native Assets

If you want your custom manager to support native Unreal features, ensure it implements/overrides:

*   **`IHapticDevice`**: haptic assets.
    *   `SetHapticFeedbackValues`: Processes frequency and amplitude values from assets.
    *   `GetHapticFrequencyRange`: Determines the valid frequency range supported by the device.
    *   `GetHapticAmplitudeScale`: Returns the scaling factor for amplitude mapping.
*   **`IInputDevice`**: Required for standard vibration, light color, and properties.
    *   `SetChannelValues` / `SetChannelValue`: Essential for `UForceFeedbackEffect` assets.
    *   `SetLightColor` / `ResetLightColor`: Controls the controller's LED.
    *   `SetDeviceProperty`: Handles `UInputDeviceProperty` (e.g., Adaptive Triggers via Unreal 5.1+ system).
    *   `GetHapticDevice`: Returns the `IHapticDevice*` interface (usually `return this;`).
    *   `IsGamepadAttached`: Returns whether the device is currently connected.

### Custom implementation example:

1. Create your custom class inheriting from `DeviceManager`:
```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "DeviceManager.h"
#include "Modules/ModuleManager.h"
#include "WindowsDualsense_ds5w.h"

class FMyCustomDeviceManager : public DeviceManager
{
public:
    using DeviceManager::DeviceManager;
	/** * Map to track the previous frame's touch state per DeviceID.
	 * Marked as 'mutable' so it can be updated within const methods.
	 */
	mutable TMap<int32, bool> DeviceTouchStates;
	    
	virtual void TouchpadImpl(FDeviceContext* Context, FInputContext& FrameInput, const FPlatformUserId UserId,
	                          const FInputDeviceId InputDeviceId, float DeltaTime) const override
	{
	    // --- 1. Basic Touch Mapping (Unreal Message Handler) ---
	    if (Context->bEnableTouch)
	    {
	        bool& bWasTouchDown = DeviceTouchStates.FindOrAdd(InputDeviceId.GetId(), false);
	
	        if (FrameInput.bIsTouching && !bWasTouchDown)
	        {
	            MessageHandler->OnTouchStarted(nullptr, FrameInput.TouchPosition, 1.0f, FrameInput.TouchId, UserId, InputDeviceId);
	        }
	        else if (FrameInput.bIsTouching && bWasTouchDown)
	        {
	            MessageHandler->OnTouchMoved(FrameInput.TouchPosition, 1.0f, FrameInput.TouchId, UserId, InputDeviceId);
	        }
	        else if (!FrameInput.bIsTouching && bWasTouchDown)
	        {
	            MessageHandler->OnTouchEnded(FrameInput.TouchPosition, FrameInput.TouchId, UserId, InputDeviceId);
	        }
	
	        bWasTouchDown = FrameInput.bIsTouching;
	    }
	
	    // --- 2. Gesture Mapping (Two-Finger Scroll) ---
	    if (Context->bEnableGesture)
	    {
	        // Check if exactly 2 fingers are touching the pad
	        if (FrameInput.bIsTouching && FrameInput.TouchFingerCount == 2)
	        {
	            MessageHandler->OnTouchGesture(
	                EGestureEvent::Scroll,
	                ScrollDelta,
	                0.0f,   /* Value / Total movement if needed */
	                false   /* IsInverted */
	            );
	        }
	    }
	}

};

```

2. Register your custom factory in your Game Module:
```cpp
class FMyProject : public FDefaultGameModuleImpl {
public:
    virtual bool IsGameModule() const override { return true; }

    virtual void StartupModule() override {
        FWindowsDualsense_ds5wModule::SetCustomInputDeviceFactory([](const TSharedRef<FGenericApplicationMessageHandler>& InHandler)
        {
            UE_LOG(LogTemp, Log, TEXT("MyProject Game Module: Init FMyCustomDeviceManager."));
            return MakeShared<FMyCustomDeviceManager>(InHandler);
        });
    }
};

IMPLEMENT_PRIMARY_GAME_MODULE( FMyProject, MyProject, "MyProject" );
```


3. Build Configuration
   Ensure your project's Build.cs includes the plugin module and enables C++20 support:
```csharp
public class NewDeveloper : ModuleRules
{
    public NewDeveloper(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // Required for Concepts and Policy-Based architecture
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { 
            ...
            "WindowsDualsense_ds5w" 
        });
    }
}
```

# Extending for Other Platforms (e.g., PlayStation)

The plugin features a decoupled architecture using Policy-Based Design, allowing developers to integrate other platform SDKs (such as the official Sony PlayStation® SDK) or custom HID wrappers directly from their Game Project.

The primary advantage is that you do not need to modify the plugin's source code. You can inject your implementation during the application startup.

1. Implementation via Hardware Policy
   Low-level hardware communication is abstracted through a Template-Policy system. To add a new platform, you create a simple C++ struct in your project that implements the required hardware methods (Read, Write, Detect, etc.).
```cpp
#pragma once
#include "CoreMinimal.h"

namespace SonyPlatformPolicy 
{
    struct FSonyHardware 
    {
        FSonyHardware() = default;

        // Implementation of the required Hardware Policy methods
        void Read(FDeviceContext* Context) { /* Your SDK Read */ }
        void Write(FDeviceContext* Context) { /* Your SDK Write */ }
        void Detect(TArray<FDeviceContext>& Devices) { /* Your SDK Detect */ }
        bool CreateHandle(FDeviceContext* Context) { return true; }
        void InvalidateHandle(FDeviceContext* Context) { /* Cleanup */ }
        void ProcessAudioHaptic(FDeviceContext* Context) { /* Haptics logic */ }
    };
}
```
2. Injection via Game Module
      Instead of modifying a singleton inside the plugin, you "inject" your custom hardware platform during your Game Module's startup. This ensures your project-specific logic takes precedence over the default HID implementation.
   Example Implementation in your Game Module (NewDeveloper.cpp):
```cpp
#include "NewDeveloper.h"
#include "Modules/ModuleManager.h"
#include "Implementations/Platforms/Others/GamepadHardwareBridge.h"
#include "Platforms/SonyPlatformPolicy.h"
#include <memory>

class FNewDeveloper : public FDefaultGameModuleImpl {
public:
    virtual bool IsGameModule() const override { return true; }

    virtual void StartupModule() override {
        // Injecting the custom hardware platform into the Plugin Bridge
        auto CustomPlatform = std::make_unique<SonyPlatformPolicy::FSonyHardware>();
        FGamepadHardwareBridge::InjectHardwarePlatform(std::move(CustomPlatform));
        
        UE_LOG(LogTemp, Log, TEXT("NewDeveloper Game Module: Custom Hardware Policy Injected."));
    }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FNewDeveloper, NewDeveloper, "NewDeveloper");
```

3. Build Configuration
   Ensure your project's Build.cs includes the plugin module and enables C++20 support:
```csharp
public class NewDeveloper : ModuleRules
{
    public NewDeveloper(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // Required for Concepts and Policy-Based architecture
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { 
            ...
            "WindowsDualsense_ds5w" 
        });

        // Add your custom SDK libraries here
        // PublicSystemLibraries.Add("MySDK.lib");
    }
}
```
---
## 🤝 Core Maintainers

This plugin is actively maintained by:

* **[Rafael Valoto](https://github.com/rafaelvaloto)** – Original author and lead developer
* **[Marat Radchenko (@slonopotamus)](https://github.com/slonopotamus)** – Senior Engineer (CTO), enterprise-level build systems, CI/CD, and cross-platform compilation

---
## 💖 Sponsor the Project
<div align="center">

<a href="https://github.com/sponsors/rafaelvaloto">
  <img src="https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&color=ea4aaa&style=for-the-badge" height="45" alt="Sponsor Button">
</a>

<p><b>Help sustain open-source "AAA" tools for developers.</b></p>

</div>
<br>
This is a professional-grade, open-source tool built to bring advanced controller features to all developers—from indies to AAA studios.

---

## 📉 Massive Engineering Savings
Maintaining separate hardware implementations for different platforms is a silent budget killer. This plugin eliminates that overhead:
* **Unified Codebase:** Write your haptic and trigger logic once. A single code path for all platforms.
* **Hardware Abstraction:** Save **hundreds of engineering hours** otherwise spent on low-level HID maintenance and firmware edge cases.
* **Zero-Overhead Maintenance:** When a platform updates its SDK, you only update the bridge, not your entire game’s input system.

---

### 🔒 NDA-Safe Architecture (PlayStation SDK Compatible)
**Inject licensed/proprietary SDKs without modifying the plugin's source code.**

The plugin uses a Policy-Based Design that acts as a "bridge": your NDA-covered platform code stays in **your project**.
- ✅ No NDA violations or license conflicts.
- ✅ Official Sony SDKs can be integrated safely.
- ✅ Your proprietary code remains under your control.

### Why Studios Choose This Plugin:
- **🌍 Cross-Platform API** – Unified input logic for Windows, Linux, macOS, and PlayStation.
- **⚡ Production-Ready** – C++20 zero-overhead architecture.
- **🚀 Faster Development** – Real-time haptic prototyping via console commands.


---
## 📄 License

This project is distributed under the MIT License. See the `LICENSE` file for more details.

## 🤝 How to Contribute

Contributions are welcome! If you have ideas, suggestions, or bug fixes, feel free to open an *Issue* or submit a *Pull Request*.

---
## ⭐ Credits and Acknowledgments

The foundation of this plugin was built upon the research and code from several amazing projects in the community:

* [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows) - Initial DS5 implementation logic.
* [Nielk1 on GIST](https://gist.github.com/Nielk1/6d54cc2c00d2201ccb8c2720ad7538db) - HID report structures.
* [DualSenseAPI](https://github.com/BadMagic100/DualSenseAPI/tree/master) - Hardware communication references.
* [flok pydualsense](https://github.com/flok/pydualsense) - Feature report research.
* [SAxense](https://github.com/egormanga/SAxense) - Base for Bluetooth Audio Haptics.
* [miniaudio](https://github.com/mackron/miniaudio) - Audio playback and conversion library.
* [Ryochan7/DS4Windows](https://github.com/Ryochan7/DS4Windows) - Industry standard for DualShock/DualSense on Windows.
* [linux/drivers/hid/hid-playstation.c](https://github.com/torvalds/linux/blob/master/drivers/hid/hid-playstation.c#L1709) - Reference for calibration, gyroscope, and Linux driver standards.

Special thanks to the community members who helped improve this plugin:

* **[yncat](https://github.com/yncat)**: For the extensive research and implementation logic regarding **USB Audio Haptics**, which was crucial for supporting high-fidelity haptics via USB ([Issue #105](https://github.com/rafaelvaloto/Unreal-Dualsense/issues/105)).

A special thanks to the Unreal Engine team for providing the Arena Shooter templates, which served as an excellent foundation for the example project demonstrating this plugin's features.

---

## ⚖️ Disclaimer and Trademarks

This software is an independent and unofficial project. It is **not** affiliated, associated, authorized, endorsed by, or in any way officially connected with Sony Interactive Entertainment Inc., Microsoft Corporation, Apple Inc., Epic Games, Unity Technologies, the Godot Engine project, or the Open 3D Foundation.

**Trademarks belong to their respective owners:**

* **Sony:** "PlayStation", "PlayStation Family Mark", "PS5 logo", "PS5", "DualSense", and "DualShock" are registered trademarks or trademarks of Sony Interactive Entertainment Inc. "SONY" is a registered trademark of Sony Corporation.
* **Microsoft:** "Windows" and "Xbox" are registered trademarks of Microsoft Corporation.
* **Apple:** "Mac" and "macOS" are registered trademarks of Apple Inc.
* **Linux:** "Linux" is the registered trademark of Linus Torvalds in the U.S. and other countries.
* **Epic Games:** "Unreal" and "Unreal Engine" are trademarks or registered trademarks of Epic Games, Inc. in the United States of America and elsewhere.
* **Unity:** "Unity", Unity logos, and other Unity trademarks are trademarks or registered trademarks of Unity Technologies or its affiliates in the U.S. and elsewhere.
* **Godot:** "Godot" and the Godot logo are trademarks of the Godot Engine project.
* **O3DE:** "O3DE" and the O3DE logo are trademarks of the Open 3D Foundation.
