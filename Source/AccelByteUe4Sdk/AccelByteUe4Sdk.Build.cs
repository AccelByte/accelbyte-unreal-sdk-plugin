// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

using UnrealBuildTool;
using System.IO;
using System;
using System.Collections.Generic;
using System.Diagnostics;

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

		if (string.IsNullOrEmpty(Environment.GetEnvironmentVariable("BuildDocs"))) return;

		if (BuildDocs(ModuleDirectory))
		{
			Console.WriteLine("AccelByteDocsBuilder: documentation built successfully");
		}
	}

	private static bool BuildDocs(string ModuleDirectory)
	{
		try
		{
			using (var Process = new Process())
			{
				string ScriptPath = Path.Combine(ModuleDirectory, @"..\..\Doxygen\docs-builder\accelbyte_docs_builder.py");
				Process.StartInfo.FileName = "python.exe";
				Process.StartInfo.Arguments = string.Format("{0} internal", ScriptPath);
				Process.StartInfo.UseShellExecute = false;

				Process.Start();
				if (!Process.HasExited) Process.WaitForExit();

				return Process.ExitCode == 0;
			}
		}
		catch (Exception E)
		{
			Console.WriteLine("Docs Builder Error: {0}", E.Message);
			return false;
		}
	}
}

}