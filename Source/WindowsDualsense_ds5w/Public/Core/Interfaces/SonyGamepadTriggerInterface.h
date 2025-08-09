// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SonyGamepadTriggerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USonyGamepadTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINDOWSDUALSENSE_DS5W_API ISonyGamepadTriggerInterface
{
	GENERATED_BODY()

public:
	/**
	 * Configures the trigger effects on a gamepad using specified haptic feedback values.
	 *
	 * @param Values A reference to an FHapticFeedbackValues structure that contains the details
	 *               of the haptic feedback configuration for the triggers.
	 */
	virtual void SetTriggers(const FInputDeviceProperty* Values) = 0;
	/**
	 * Sets the haptic feedback for a specific hand on the gamepad.
	 *
	 * @param Hand The hand to which the haptic feedback will be applied.
	 *             Typically, 0 represents the left hand, and 1 represents the right hand.
	 * @param Values A pointer to an FHapticFeedbackValues structure containing the haptic feedback details.
	 */
	virtual void SetHapticFeedback(int32 Hand, const FHapticFeedbackValues* Values) = 0;
	/**
	 * Sets the resistance levels for the gamepad triggers across different stages
	 * and applies them to a specified controller hand.
	 *
	 * @param BeginStrength The resistance strength applied at the beginning point of the trigger pull.
	 * @param MiddleStrength The resistance strength applied during the middle point of the trigger pull.
	 * @param EndStrength The resistance strength applied at the end point of the trigger pull.
	 * @param Hand The controller hand (e.g., left or right) to which the resistance settings will be applied.
	 */
	virtual void SetResistance(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand) = 0;
	/**
	 * Sets the automatic gun trigger effects on a controller by defining resistance levels
	 * across multiple stages and optionally retaining the effect across actions.
	 *
	 * @param BeginStrength The resistance strength at the beginning stage of the trigger pull.
	 * @param MiddleStrength The resistance strength at the middle stage of the trigger pull.
	 * @param EndStrength The resistance strength at the end stage of the trigger pull.
	 * @param Hand The controller hand (e.g., left or right) where the effect will be applied.
	 * @param KeepEffect Indicates whether the applied effect should persist after the action.
	 */
	virtual void SetAutomaticGun(int32 BeginStrength, int32 MiddleStrength, int32 EndStrength, const EControllerHand& Hand, bool KeepEffect) = 0;
	/**
	 * Configures continuous resistance for a gamepad trigger at a specific position
	 * with a defined level of strength for a given controller hand.
	 *
	 * @param StartPosition The starting position on the trigger where the resistance begins to take effect.
	 * @param Strength The level of resistance to be applied, typically within a valid range for the device.
	 * @param Hand The controller hand (e.g., left or right) where the resistance effect will be applied.
	 */
	virtual void SetContinuousResistance(int32 StartPosition, int32 Strength, const EControllerHand& Hand) = 0;
	/**
	 * Configures the galloping effect on a gamepad by specifying the start and end positions,
	 * foot timings, frequency, and the hand to which the effect is applied.
	 *
	 * @param StartPosition The starting position of the trigger effect.
	 * @param EndPosition The ending position of the trigger effect.
	 * @param FirstFoot The timing or position representing the impact of the first foot.
	 * @param SecondFoot The timing or position representing the impact of the second foot.
	 * @param Frequency The frequency of the galloping effect to determine its speed.
	 * @param Hand The controller hand (e.g., left or right) where the effect will be applied.
	 */
	virtual void SetGalloping(int32 StartPosition, int32 EndPosition, int32 FirstFoot, int32 SecondFoot, float Frequency, const EControllerHand& Hand) = 0;
	/**
	 * Configures a custom machine effect on the gamepad triggers by defining start and end positions,
	 * amplitude levels, frequency, and duration specifics for a given controller hand.
	 *
	 * @param StartPosition The starting position of the trigger effect.
	 * @param EndPosition The ending position of the trigger effect.
	 * @param AmplitudeBegin The initial amplitude level for the trigger effect.
	 * @param AmplitudeEnd The final amplitude level for the trigger effect.
	 * @param Frequency The frequency of the effect to control its repetition rate.
	 * @param Period The duration of one cycle of the effect.
	 * @param Hand The controller hand (e.g., left or right) where the effect will be applied.
	 */
	virtual void SetMachine(int32 StartPosition, int32 EndPosition, int32 AmplitudeBegin, int32 AmplitudeEnd, float Frequency, float Period, const EControllerHand& Hand) = 0;
	/**
	 * Configures the weapon effect on a gamepad's adaptive triggers based on specified parameters.
	 *
	 * @param StartPosition The starting position for the trigger effect as an integer value.
	 * @param EndPosition The ending position for the trigger effect as an integer value.
	 * @param Strength The intensity of the trigger effect as an integer value.
	 * @param Hand A reference to the hand (left or right) where the effect should be applied, represented by EControllerHand.
	 */
	virtual void SetWeapon(int32 StartPosition, int32 EndPosition, int32 Strength, const EControllerHand& Hand) = 0;
	/**
	 * Configures the bow tension effects on a gamepad trigger for a specified hand.
	 *
	 * @param StartPosition The starting position of the bow tension effect on the trigger.
	 * @param EndPosition The ending position of the bow tension effect on the trigger.
	 * @param BegingStrength The intensity of the bow tension effect at the starting position.
	 * @param EndStrength The intensity of the bow tension effect at the ending position.
	 * @param Hand The controller hand to which the bow effect will be applied.
	 */
	virtual void SetBow(int32 StartPosition, int32 EndPosition, int32 BegingStrength, int32 EndStrength, const EControllerHand& Hand) = 0;
	/**
	 * Disables the trigger effects on a gamepad for the specified controller hand.
	 *
	 * @param Hand An enum indicating which controller hand's trigger effects should be stopped.
	 */
	virtual void StopTrigger(const EControllerHand& Hand) = 0;

	/**
	 * Configures the gamepad vibration based on audio feedback parameters.
	 *
	 * @param Vibration A reference to an FForceFeedbackValues structure containing the force feedback values for each motor.
	 * @param Threshold The minimum amplitude of the audio envelope that triggers vibration. Default is 0.015f.
	 * @param ExponentCurve The exponent applied to shape the vibration curve. Default is 2.f.
	 * @param BaseMultiplier A multiplier applied to the vibration intensity base level. Default is 1.5f.
	 */
	virtual void SetVibrationAudioBased(const FForceFeedbackValues& Vibration, const float Threshold = 0.015f, const float ExponentCurve = 2.f, const float BaseMultiplier = 1.5f) = 0;
};
