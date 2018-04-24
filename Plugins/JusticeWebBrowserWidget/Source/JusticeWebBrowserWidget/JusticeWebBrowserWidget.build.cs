// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

namespace UnrealBuildTool.Rules
{
	public class JusticeWebBrowserWidget : ModuleRules
	{
		public JusticeWebBrowserWidget(ReadOnlyTargetRules Target) : base(Target)
		{
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
				    "CoreUObject",
					"WebBrowser",
                    "Slate",
                    "SlateCore",
					"UMG",
                    "Json",
                    "JsonUtilities",
                    "JusticeSDK",
                }
			);
		}
	}
}
