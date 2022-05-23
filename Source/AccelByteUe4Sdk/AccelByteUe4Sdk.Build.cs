// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

using UnrealBuildTool;
using System.IO;
using System;
using System.Collections.Generic;

namespace UnrealBuildTool.Rules
{

public class AccelByteUe4Sdk : ModuleRules
{
    public AccelByteUe4Sdk(ReadOnlyTargetRules Target) : base(Target)
    {
#if UE_4_24_OR_LATER || UE_5_0_OR_LATER
        bLegacyPublicIncludePaths = false;
        DefaultBuildSettings = BuildSettingsVersion.V2;
#else
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
#endif

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Sockets",
            "Json",
            "JsonUtilities",
            "HTTP",
            "WebSockets",
            "Networking",
            "SSL",
            "Icmp"
        });

        if (System.Environment.GetEnvironmentVariable("SQLITE3_ENABLED") == "1")
        {
            PublicDependencyModuleNames.Add("CISQLite3");
        }

        PrivateDependencyModuleNames.AddRange(new []
        {
            "OpenSSL",
            "Projects"
        });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new[]
            {
                "Settings"
            });
        }
    }

}
}
