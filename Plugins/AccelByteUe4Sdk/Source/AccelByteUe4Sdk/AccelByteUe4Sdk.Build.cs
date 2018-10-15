// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AccelByteUe4Sdk : ModuleRules
{
	public AccelByteUe4Sdk(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                "AccelByteUe4Sdk/Public",
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"AccelByteUe4Sdk/Private",
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
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

            }
			);
	}
}
