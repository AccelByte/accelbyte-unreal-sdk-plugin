// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

using UnrealBuildTool;
using System.IO;
using System;

public class AccelByteUe4Sdk : ModuleRules
{
    public AccelByteUe4Sdk(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        bool AgonesPluginFound = Directory.Exists(Path.Combine(PluginDirectory, "..", "Agones"));
        PublicDefinitions.Add("AGONES_PLUGIN_FOUND=" + (AgonesPluginFound ? "1" : "0"));
        if (AgonesPluginFound)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "Agones" });
        }

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
