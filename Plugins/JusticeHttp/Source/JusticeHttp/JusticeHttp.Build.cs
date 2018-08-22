// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JusticeHttp : ModuleRules
{
	public JusticeHttp(ReadOnlyTargetRules Target) : base(Target)
	{
		Definitions.Add("HTTP_PACKAGE=1");
        //Definitions.Add("WITH_LIBCURL=1");
        
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(
			new string[] {
				"JusticeHttp/Private",
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Win32 ||
			Target.Platform == UnrealTargetPlatform.Win64)
		{
			AddEngineThirdPartyPrivateStaticDependencies(Target, "WinInet");
			AddEngineThirdPartyPrivateStaticDependencies(Target, "WinHttp");
			AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");

			PrivateDependencyModuleNames.Add("SSL");
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux ||
			Target.Platform == UnrealTargetPlatform.Android ||
			Target.Platform == UnrealTargetPlatform.Switch)
		{
			AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
			PrivateDependencyModuleNames.Add("SSL");
		}
		else
		{
			Definitions.Add("WITH_SSL=0");
			Definitions.Add("WITH_LIBCURL=0");
		}

		if (Target.Platform == UnrealTargetPlatform.HTML5)
		{
			PrivateDependencyModuleNames.Add("HTML5JS");
		}

		if (Target.Platform == UnrealTargetPlatform.IOS || Target.Platform == UnrealTargetPlatform.TVOS || Target.Platform == UnrealTargetPlatform.Mac)
		{
			PublicFrameworks.Add("Security");
		}
	}
}
