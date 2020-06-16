// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

using UnrealBuildTool;
using System.Collections.Generic;


[SupportedPlatforms(UnrealPlatformClass.Server)]
public class AccelByteUe4SdkDemoServerTarget : TargetRules
{
    public AccelByteUe4SdkDemoServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        ExtraModuleNames.AddRange( new string[] { "AccelByteUe4SdkDemo", "AccelByteUe4Sdk", "AccelByteCustomWebBrowserWidget" } );
    }
}
