// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class JusticeUE4PluginTarget : TargetRules
{
	public JusticeUE4PluginTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "JusticeUE4Plugin" } );
	}
}
