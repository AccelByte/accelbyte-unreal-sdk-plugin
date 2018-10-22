// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

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
			});


        PrivateIncludePaths.AddRange(
            new string[] {
                "AccelByteUe4Sdk/Private",
				// ... add other private include paths required here ...
			});

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Sockets",
                "Json",
                "JsonUtilities",
                "Http",
                "WebSockets",
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Sockets",
                "Json",
                "JsonUtilities",
                "Http",
                "WebSockets",
            });


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            });
    }
}
