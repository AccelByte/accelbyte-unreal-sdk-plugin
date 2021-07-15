// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "TestModels.h"
#include "TestUtilities.h"
#include "Core/AccelByteUtilities.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCoreTest, Log, All);

DEFINE_LOG_CATEGORY(LogAccelByteCoreTest);

const int32 AutomationFlagMaskCore = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter |
	EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CoreConfigTest, "AccelByte.Tests.A.Core.A.Config", AutomationFlagMaskCore);

bool CoreConfigTest::RunTest(const FString& Parameters)
{
	const FString EmptyString = "";
	TestNotEqual("BaseUrl", FRegistry::Settings.BaseUrl, EmptyString);
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

DEFINE_SPEC(
	RemoveEmptyStringsFromJsonSpec,
	"AccelByte.Tests.Core.RemoveEmptyStringsFromJson",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

void RemoveEmptyStringsFromJsonSpec::Define()
{
	Describe("Given a UStruct", [this]()
	{
		It("When some of its root fields are empty string, they aren't serialized", [this]()
		{
			FStringFields StringFields;

			StringFields.FieldA = "notEmpty";
			StringFields.FieldC = "alsoNotEmpty";
			TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(StringFields);
			FAccelByteUtilities::RemoveEmptyStrings(Json);
			FString String;
			TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&String);
			FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

			AB_TEST_TRUE(String.Contains("notEmpty"));
			AB_TEST_TRUE(String.Contains("alsoNotEmpty"));

			AB_TEST_TRUE(String.Contains("FieldA"));
			AB_TEST_FALSE(String.Contains("FieldB"));
			AB_TEST_FALSE(String.Contains("FieldD"));
			AB_TEST_TRUE(String.Contains("FieldC"));
			AB_TEST_FALSE(String.Contains("FieldE"));
			AB_TEST_FALSE(String.Contains("FieldF"));

			return true;
		});

		It("When some of its children fields are empty string, they aren't serialized", [this]()
		{
			FStringFields StringFields;

			StringFields.FieldA = "notEmpty";
			StringFields.FieldC = "alsoNotEmpty";
			FParent Parent;
			Parent.ChildA = StringFields;
			Parent.ChildB = StringFields;
			TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(Parent);
			FAccelByteUtilities::RemoveEmptyStrings(Json);
			FString String;
			TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&String);
			FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

			AB_TEST_TRUE(String.Contains("ChildA"));
			AB_TEST_TRUE(String.Contains("ChildB"));

			AB_TEST_TRUE(String.Contains("notEmpty"));
			AB_TEST_TRUE(String.Contains("alsoNotEmpty"));

			AB_TEST_TRUE(String.Contains("FieldA"));
			AB_TEST_FALSE(String.Contains("FieldB"));
			AB_TEST_FALSE(String.Contains("FieldD"));
			AB_TEST_TRUE(String.Contains("FieldC"));
			AB_TEST_FALSE(String.Contains("FieldE"));
			AB_TEST_FALSE(String.Contains("FieldF"));

			return true;
		});

		It("When some of its grandchildren fields are empty string, they aren't serialized", [this]()
		{
			FStringFields StringFields;
			StringFields.FieldA = "notEmpty";
			StringFields.FieldC = "alsoNotEmpty";

			FParent Parent;
			Parent.ChildA = StringFields;
			Parent.ChildB = StringFields;


			FStringFields OtherStringFields;
			OtherStringFields.FieldE = "shouldNotEmpty";

			FGrandParent GrandParent;
			GrandParent.ParentA = Parent;
			GrandParent.ParentB = Parent;
			GrandParent.ParentB.ChildB = OtherStringFields;
			
			TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(GrandParent);
			FAccelByteUtilities::RemoveEmptyStrings(Json);
			FString String;
			TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&String);
			FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

			AB_TEST_TRUE(String.Contains("ParentA"));
			AB_TEST_TRUE(String.Contains("ParentB"));
			AB_TEST_TRUE(String.Contains("ChildA"));
			AB_TEST_TRUE(String.Contains("ChildB"));

			AB_TEST_TRUE(String.Contains("notEmpty"));
			AB_TEST_TRUE(String.Contains("alsoNotEmpty"));
			AB_TEST_TRUE(String.Contains("shouldNotEmpty"));

			AB_TEST_TRUE(String.Contains("FieldA"));
			AB_TEST_FALSE(String.Contains("FieldB"));
			AB_TEST_TRUE(String.Contains("FieldC"));
			AB_TEST_FALSE(String.Contains("FieldD"));
			AB_TEST_TRUE(String.Contains("FieldE"));
			AB_TEST_FALSE(String.Contains("FieldF"));

			return true;
		});
	});
}
