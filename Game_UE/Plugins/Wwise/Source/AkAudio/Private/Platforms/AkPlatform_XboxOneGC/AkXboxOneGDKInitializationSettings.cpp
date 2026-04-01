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

#include "Platforms/AkPlatform_XboxOneGC/AkXboxOneGDKInitializationSettings.h"
#include "Wwise/Stats/AkAudioMemory.h"
#include "AkAudioDevice.h"
#include "WwiseDefines.h"

#ifdef AK_XBOXONEGC
namespace AkInitializationSettings_Helpers
{
	// forward device calls to stock Wwise allocdevice calls
	// These device-memory calls are just a thin wrapper around ApuAlloc.
	// Note that these should not be called if XMA is disabled
	void* AkXb1GcMemAllocDevice(size_t size, size_t* extra)
	{
		ASYNC_INC_MEMORY_STAT_BY(STAT_WwiseMemorySoundEngineDevice, size);
		return AKPLATFORM::AllocDevice(size, extra);
	}

	void AkXb1GcMemFreeDevice(void* address, size_t size, size_t extra, size_t release)
	{
		if (release)
		{
			AKPLATFORM::FreeDevice(address, size, extra, release);
			ASYNC_DEC_MEMORY_STAT_BY(STAT_WwiseMemorySoundEngineDevice, release);
		}
	}
}
#endif

//////////////////////////////////////////////////////////////////////////
// FAkXboxOneGDKAdvancedInitializationSettings

void FAkXboxOneGDKAdvancedInitializationSettings::FillInitializationStructure(FAkInitializationStructure& InitializationStructure) const
{
	Super::FillInitializationStructure(InitializationStructure);

#ifdef AK_XBOXONEGC
	InitializationStructure.PlatformInitSettings.uMaxXMAVoices = MaximumNumberOfXMAVoices;
	InitializationStructure.PlatformInitSettings.bHwCodecLowLatencyMode = UseHardwareCodecLowLatencyMode;

	// if no XMA voices are active, we can disable the device heap in order to avoid APU heap creation
	if (InitializationStructure.PlatformInitSettings.uMaxXMAVoices == 0)
	{
#if WWISE_2023_1_OR_LATER
		InitializationStructure.MemSettings.pfAllocDevice = nullptr;
		InitializationStructure.MemSettings.pfFreeDevice = nullptr;
		InitializationStructure.MemSettings.bEnableSeparateDeviceHeap = false;
#else
		// on pre-23.1, we "disable" device memory by setting bUseDeviceMemAlways to true,
		// and direct the "device" alloc hook to the generic alloc hook
		InitializationStructure.MemSettings.pfAllocDevice = InitializationStructure.MemSettings.pfAllocVM;
		InitializationStructure.MemSettings.pfFreeDevice = InitializationStructure.MemSettings.pfFreeVM;
		InitializationStructure.MemSettings.bUseDeviceMemAlways = true;
#endif // WWISE_2023_1_OR_LATER
	}
	else
	{
		InitializationStructure.MemSettings.pfAllocDevice = AkInitializationSettings_Helpers::AkXb1GcMemAllocDevice;
		InitializationStructure.MemSettings.pfFreeDevice = AkInitializationSettings_Helpers::AkXb1GcMemFreeDevice;
	}
#endif // AK_XBOXONEGC
}

//////////////////////////////////////////////////////////////////////////
// FAkXboxOneGDKApuHeapInitializationSettings

void FAkXboxOneGDKApuHeapInitializationSettings::FillInitializationStructure(FAkInitializationStructure& InitializationStructure) const
{
#ifdef AK_XBOXONE_NEED_APU_ALLOC
	if (InitializationStructure.PlatformInitSettings.uMaxXMAVoices > 0)
	{
		// Perform this as early as possible to ensure that no other allocation calls are made before this!
		auto ApuCreateHeapResult = ApuCreateHeap(CachedSize, NonCachedSize);
		if (ApuCreateHeapResult == APU_E_HEAP_ALREADY_ALLOCATED)
		{
			UE_LOG(LogAkAudio, Warning, TEXT("APU heap has already been allocated."));
		}
		else if (ApuCreateHeapResult != S_OK)
		{
			UE_LOG(LogAkAudio, Warning, TEXT("APU heap could not be allocated."));
		}
	}
#endif // AK_XBOXONE_NEED_APU_ALLOC
}


//////////////////////////////////////////////////////////////////////////
// UAkXboxOneGDKInitializationSettings

UAkXboxOneGDKInitializationSettings::UAkXboxOneGDKInitializationSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CommunicationSettings.DiscoveryBroadcastPort = FAkCommunicationSettings::DefaultDiscoveryBroadcastPort;
	CommunicationSettings.CommandPort = FAkCommunicationSettings::DefaultDiscoveryBroadcastPort + 1;
}

void UAkXboxOneGDKInitializationSettings::FillInitializationStructure(FAkInitializationStructure& InitializationStructure) const
{
#ifdef AK_XBOXONEGC_VS_VERSION
	constexpr auto PlatformArchitecture = "XboxOneGC_" AK_XBOXONEGC_VS_VERSION;
#else
	constexpr auto PlatformArchitecture = "XboxOneGC_vc160";
#endif

	InitializationStructure.SetPluginDllPath(PlatformArchitecture);

	CommonSettings.FillInitializationStructure(InitializationStructure);
	AdvancedSettings.FillInitializationStructure(InitializationStructure);
	ApuHeapSettings.FillInitializationStructure(InitializationStructure);
	CommunicationSettings.FillInitializationStructure(InitializationStructure);
}
