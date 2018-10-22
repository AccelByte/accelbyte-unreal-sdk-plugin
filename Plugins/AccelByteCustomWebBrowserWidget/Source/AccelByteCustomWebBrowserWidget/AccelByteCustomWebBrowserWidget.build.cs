// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

namespace UnrealBuildTool.Rules
{
	public class AccelByteCustomWebBrowserWidget : ModuleRules
	{
		public AccelByteCustomWebBrowserWidget(ReadOnlyTargetRules Target) : base(Target)
		{
            PrivateDependencyModuleNames.AddRange(new string[] { "AccelByteUe4Sdk" });

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
                    "AccelByteUe4Sdk",
                }
			);
		}
	}
}
