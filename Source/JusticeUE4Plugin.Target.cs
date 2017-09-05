// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class JusticeUE4PluginTarget : TargetRules
{
	public JusticeUE4PluginTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "JusticeUE4Plugin", "OnlineSubsystemJustice" } );
	}
}
