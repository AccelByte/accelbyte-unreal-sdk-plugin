// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

using UnrealBuildTool;
using System.Collections.Generic;

public class AccelByteUe4SdkDemoTarget : TargetRules
{
    public AccelByteUe4SdkDemoTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        ExtraModuleNames.AddRange( new string[] { "AccelByteUe4SdkDemo", "AccelByteUe4Sdk", "AccelByteCustomWebBrowserWidget" } );
    }
}
