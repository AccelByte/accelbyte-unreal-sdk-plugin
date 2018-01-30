// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

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
