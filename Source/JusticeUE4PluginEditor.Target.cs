// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class JusticeUE4PluginEditorTarget : TargetRules
{
	public JusticeUE4PluginEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "JusticeUE4Plugin", "OnlineSubsystemJustice" } );
	}
}
