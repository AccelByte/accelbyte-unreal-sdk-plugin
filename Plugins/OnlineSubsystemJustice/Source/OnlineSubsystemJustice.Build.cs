// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

using UnrealBuildTool;
using System.IO;

public class OnlineSubsystemJustice : ModuleRules
{
	public OnlineSubsystemJustice(ReadOnlyTargetRules Target) : base(Target)
    {        
        Definitions.Add("ONLINESUBSYSTEMJUSTICE_PACKAGE=1");
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                 "Core",
                 "CoreUObject",
                 "Engine",
                 "OnlineSubsystem",
                 "OnlineSubsystemUtils",
                 "Json",
                 "JsonUtilities",
            }
        );

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core", 
				"CoreUObject", 
				"Engine", 
				"Sockets", 
				"OnlineSubsystem", 
				"OnlineSubsystemUtils",
				"Json",
                "JsonUtilities",
                "Http"
			}
			);
	}
}
