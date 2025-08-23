// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DeviceSettings.generated.h"

USTRUCT(BlueprintType)
struct FGamepadMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, config, Category="Input")
	int32 UserId = 0;

	UPROPERTY(EditAnywhere, config, Category="Input")
	int32 DeviceId = 0;
};


/**
 * 
 */
UCLASS(config=WindowsDualSense, defaultconfig)
class WINDOWSDUALSENSE_DS5W_API UDeviceSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = "Sony Gamepad Map")
	FGamepadMapping SonyGamepadOne;

	UPROPERTY(EditAnywhere, config, Category = "Sony Gamepad Map")
	FGamepadMapping SonyGamepadTwo;

	UPROPERTY(EditAnywhere, config, Category = "Sony Gamepad Map")
	FGamepadMapping SonyGamepadThree;

	UPROPERTY(EditAnywhere, config, Category = "Sony Gamepad Map")
	FGamepadMapping SonyGamepadFour;
};
