// Copyright (c) 2026 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#if WITH_EDITOR

namespace AccelByte
{
namespace Editor
{

/**
 * Register the "AccelByte" top-level editor menu with a "Setup Build Processors" entry.
 * @param Owner Pointer used as the UToolMenus owner (typically the module instance).
 */
void RegisterMenu(void* Owner);

/**
 * Download PreBuildProcessor.bat / PostBuildProcessor.bat from a GitHub release
 * and patch AccelByteUe4Sdk.uplugin with the corresponding build step entries.
 */
void ExecuteSetupBuildProcessors();

} // namespace Editor
} // namespace AccelByte

#endif // WITH_EDITOR
