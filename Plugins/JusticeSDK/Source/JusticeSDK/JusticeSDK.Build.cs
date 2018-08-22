// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JusticeSDK : ModuleRules
{
	public JusticeSDK(ReadOnlyTargetRules Target) : base(Target)
	{        
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"JusticeSDK/Public",
                "JusticeHttp/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"JusticeSDK/Private",
                "JusticeHttp/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Engine",
                "Json",
                "JsonUtilities",
                "Http",
                "OnlineSubsystem",
                "OnlineSubsystemUtils",
                "WebBrowser",
                "JusticeHttp"
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "Sockets",
                "Json",
                "JsonUtilities",
                "Http",
                "OnlineSubsystem",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                "OnlineSubsystemSteam",
            }
			);
	}
}
