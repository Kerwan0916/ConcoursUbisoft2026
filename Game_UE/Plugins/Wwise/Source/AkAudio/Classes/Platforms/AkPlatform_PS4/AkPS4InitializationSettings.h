/*******************************************************************************
The content of this file includes portions of the proprietary AUDIOKINETIC Wwise
Technology released in source code form as part of the game integration package.
The content of this file may not be used without valid licenses to the
AUDIOKINETIC Wwise Technology.
Note that the use of the game engine is subject to the Unreal(R) Engine End User
License Agreement at https://www.unrealengine.com/en-US/eula/unreal
 
License Usage
 
Licensees holding valid licenses to the AUDIOKINETIC Wwise Technology may use
this file in accordance with the end user license agreement provided with the
software or, alternatively, in accordance with the terms contained
in a written agreement between you and Audiokinetic Inc.
Copyright (c) 2025 Audiokinetic Inc.
*******************************************************************************/

#pragma once

#include "AkInclude.h"
#include "InitializationSettings/AkInitializationSettings.h"
#include "InitializationSettings/AkPlatformInitialisationSettingsBase.h"

#include "AkPS4InitializationSettings.generated.h"


USTRUCT()
struct FAkPS4AdvancedInitializationSettings : public FAkAdvancedInitializationSettingsWithMultiCoreRendering
{
	GENERATED_BODY()

	FAkPS4AdvancedInitializationSettings();

	UPROPERTY(EditAnywhere, Category = "Ak Initialization Settings", meta = (ToolTip = "ACP batch buffer size used for ATRAC9 decoding."))
	uint32 ACPBatchBufferSize = (90 * 1024);

	UPROPERTY(EditAnywhere, Category = "Ak Initialization Settings", meta = (ToolTip = "Use low latency mode for ATRAC9 (default is false). If true, decoding jobs are submitted at the beginning of the Wwise update and it will be necessary to wait for the result."))
	bool UseHardwareCodecLowLatencyMode = false;

	UPROPERTY(EditAnywhere, Category = "Ak Initialization Settings", meta = (ToolTip = "If enabled, will constantly output the current context queue level as a plot to Razor CPU, to visualize audio starvation limits."))
	bool bPlotQueueLevel = true;

	void FillInitializationStructure(FAkInitializationStructure& InitializationStructure) const;
};


UCLASS(config = Game, defaultconfig)
class AKAUDIO_API UAkPS4InitializationSettings : public UObject, public IAkPlatformInitialisationSettingsBase
{
	GENERATED_BODY()

public:
	virtual const TCHAR* GetConfigOverridePlatform() const override
	{
		return TEXT("PS4");
	}

	UAkPS4InitializationSettings(const FObjectInitializer& ObjectInitializer);
	void FillInitializationStructure(FAkInitializationStructure& InitializationStructure) const;

	UPROPERTY(Config, EditAnywhere, Category = "Initialization")
	FAkCommonInitializationSettings CommonSettings;

	UPROPERTY(Config, EditAnywhere, Category = "Initialization")
	FAkCommunicationSettingsWithSystemInitialization CommunicationSettings;

	UPROPERTY(Config, EditAnywhere, Category = "Initialization", AdvancedDisplay)
	FAkPS4AdvancedInitializationSettings AdvancedSettings;

	UFUNCTION()
	void MigrateMultiCoreRendering(bool NewValue)
	{
		AdvancedSettings.EnableMultiCoreRendering = NewValue;
	}
};
