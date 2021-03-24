// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

using System;
using UnrealBuildTool;

public class AccelByteUe4SdkDemo : ModuleRules
{
	public AccelByteUe4SdkDemo(ReadOnlyTargetRules Target) : base(Target)
	{
#if UE_4_24_OR_LATER
		bLegacyPublicIncludePaths = false;
		DefaultBuildSettings = BuildSettingsVersion.V2;
#else
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
#endif
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AccelByteUe4Sdk", "Json", "JsonUtilities", "Http", "WebSockets",});

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using steam
        // DynamicallyLoadedModuleNames.AddRange(new string[] { "OnlineSubsystemSteam",});

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
