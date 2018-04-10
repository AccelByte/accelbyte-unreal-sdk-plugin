// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

using UnrealBuildTool;

public class JusticeSDKDemo : ModuleRules
{
	public JusticeSDKDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "JusticeSDK",  "Json", "JsonUtilities", });



		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "JusticeSDK","OnlineSubsystem",
                "OnlineSubsystemUtils", "Steamworks" });


        PrivateDependencyModuleNames.AddRange(new string[] {  });


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                "OnlineSubsystemSteam",
            }
            );
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
