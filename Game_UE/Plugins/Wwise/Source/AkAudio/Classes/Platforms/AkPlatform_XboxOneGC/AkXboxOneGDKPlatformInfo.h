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

#include "Platforms/AkPlatformInfo.h"
#include "AkXboxOneGDKPlatformInfo.generated.h"

UCLASS()
class UAkXboxOneGDKPlatformInfo : public UAkPlatformInfo
{
	GENERATED_BODY()

public:
	UAkXboxOneGDKPlatformInfo()
	{
		WwisePlatform = "XboxOneGDK";

#ifdef AK_XBOXONEGC_VS_VERSION
		Architecture = "XboxOneGC_" AK_XBOXONEGC_VS_VERSION;
#else
		Architecture = "XboxOneGC_vc160";
#endif

		LibraryFileNameFormat = "{0}.dll";
		DebugFileNameFormat = "{0}.pdb";

#if WITH_EDITORONLY_DATA
		UAkPlatformInfo::UnrealNameToPlatformInfo.Add("XboxOneGDK", this);
#endif
	}

#if WITH_EDITORONLY_DATA
	virtual FString GetWwiseBankPlatformName(const TArray<FString>& AvailableWwisePlatforms) const override
	{
		const FString FoundWwiseName = Super::GetWwiseBankPlatformName(AvailableWwisePlatforms);
		if (FoundWwiseName.IsEmpty())
		{
			// XboxOne GDK is compatible with XboxOne banks. In fact, we used to ask customers to use the XboxOne platform
			// for XboxOneGDK banks. If the asset does not contain "XboxOneGDK" asset data, the "XboxOne" asset data is
			// compatible.
			if (AvailableWwisePlatforms.Contains("XboxOne"))
			{
				return "XboxOne";
			}
		}

		return FoundWwiseName;
	}
#endif
};

UCLASS()
class UAkXboxOneAnvilPlatformInfo : public UAkXboxOneGDKPlatformInfo
{
	GENERATED_BODY()
	UAkXboxOneAnvilPlatformInfo()
	{
#if WITH_EDITORONLY_DATA
		UAkPlatformInfo::UnrealNameToPlatformInfo.Add("XboxOneAnvil", this);
#endif
	}
};

UCLASS()
class UAkXB1PlatformInfo : public UAkXboxOneGDKPlatformInfo
{
	GENERATED_BODY()
	UAkXB1PlatformInfo()
	{
#if WITH_EDITORONLY_DATA
		UAkPlatformInfo::UnrealNameToPlatformInfo.Add("XB1", this);
#endif
	}
};
