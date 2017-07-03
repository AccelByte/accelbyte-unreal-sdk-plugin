// Fill out your copyright notice in the Description page of Project Settings.

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
