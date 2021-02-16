// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCoreTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCoreTest);

const int32 AutomationFlagMaskCore = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CoreConfigTest, "AccelByte.Tests.A.Core.A.Config", AutomationFlagMaskCore);
bool CoreConfigTest::RunTest(const FString& Parameters)
{
    const FString EmptyString = "";
    TestNotEqual("BaseUrl", FRegistry::Settings.BaseUrl, EmptyString);
    TestNotEqual("NonApiBaseUrl", FRegistry::Settings.NonApiBaseUrl, EmptyString);
    TestNotEqual("IamServerUrl", FRegistry::Settings.IamServerUrl, EmptyString);
    TestNotEqual("PlatformServerUrl", FRegistry::Settings.PlatformServerUrl, EmptyString);
    TestNotEqual("LobbyServerUrl", FRegistry::Settings.LobbyServerUrl, EmptyString);
    TestNotEqual("BasicServerUrl", FRegistry::Settings.BasicServerUrl, EmptyString);
    TestNotEqual("CloudStorageServerUrl", FRegistry::Settings.CloudStorageServerUrl, EmptyString);
    TestNotEqual("GameProfileServerUrl", FRegistry::Settings.GameProfileServerUrl, EmptyString);
    TestNotEqual("StatisticServerUrl", FRegistry::Settings.StatisticServerUrl, EmptyString);
    TestNotEqual("QosManagerServerUrl", FRegistry::Settings.QosManagerServerUrl, EmptyString);
    TestNotEqual("LeaderboardServerUrl", FRegistry::Settings.LeaderboardServerUrl, EmptyString);
    TestNotEqual("CloudSaveServerUrl", FRegistry::Settings.CloudSaveServerUrl, EmptyString);
    TestNotEqual("GameTelemetryServerUrl", FRegistry::Settings.GameTelemetryServerUrl, EmptyString);
    TestNotEqual("AgreementServerUrl", FRegistry::Settings.AgreementServerUrl, EmptyString);
    TestNotEqual("AchievementServerUrl", FRegistry::Settings.AchievementServerUrl, EmptyString);

    TestNotEqual("Server.BaseUrl", FRegistry::ServerSettings.BaseUrl, EmptyString);
    TestNotEqual("Server.IamServerUrl", FRegistry::ServerSettings.IamServerUrl, EmptyString);
    TestNotEqual("Server.DSMControllerServerUrl", FRegistry::ServerSettings.DSMControllerServerUrl, EmptyString);
    TestNotEqual("Server.StatisticServerUrl", FRegistry::ServerSettings.StatisticServerUrl, EmptyString);
    TestNotEqual("Server.PlatformServerUrl", FRegistry::ServerSettings.PlatformServerUrl, EmptyString);
    TestNotEqual("Server.QosManagerServerUrl", FRegistry::ServerSettings.QosManagerServerUrl, EmptyString);
    TestNotEqual("Server.GameTelemetryServerUrl", FRegistry::ServerSettings.GameTelemetryServerUrl, EmptyString);
    TestNotEqual("Server.AchievementServerUrl", FRegistry::ServerSettings.AchievementServerUrl, EmptyString);

    return !HasAnyErrors();
}
