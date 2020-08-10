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
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
#if UE_4_24_OR_LATER
        bLegacyPublicIncludePaths = false;
#endif

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        bool isAgonesFound = Directory.Exists(Path.Combine(ModuleDirectory, "..", "..", "..", "Agones"));
#if UE_4_19_OR_LATER
        PublicDefinitions.Add(string.Format("AGONES_PLUGIN_FOUND={0}", (isAgonesFound ? 1 : 0)));
#else
        Definitions.Add(string.Format("AGONES_PLUGIN_FOUND={0}", (isAgonesFound ? 1 : 0)));
#endif
        if (isAgonesFound) { PublicDependencyModuleNames.AddRange(new string[] {"Agones"}); }

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Sockets",
            "Json",
            "JsonUtilities",
            "Http",
            "WebSockets",
            "Networking",
            "SSL",
            "Icmp"
        });

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "Settings"
            });
        }
    }

}
}
