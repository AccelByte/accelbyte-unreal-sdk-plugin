// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "TestModels.h"
#include "TestUtilities.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteUtilities.h"
#include "Core/Version.h"

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



DEFINE_SPEC(
	FVersionSpec,
	"AccelByte.Tests.Core.VersionSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

void FVersionSpec::Define()
{
	Describe("Given an invalid version core string", [this]()
	{
		TArray<FString> InvalidVersions =
		{
			"00.0.0",
			"0.00.0",
			"0.0.00",
			"00.00.00",
			"01.0.0",
			"0.01.0",
			"0.0.01",
			"01.01.01",
			"001.0.0",
			"0.001.0",
			"0.0.001",
			"001.001.001",
			"A.0.0",
			"0.A.0",
			"0.0.A",
			"A.A.A",
			"0.0.0",
		};

		for (auto const& Version : InvalidVersions)
		{
			It(FString::Printf(TEXT("When create FVersion from \"%s\", Then invalid"), *Version), [this, Version]()
			{
				AB_TEST_FALSE(FVersion{Version}.IsValid());

				return true;
			});
		}
	});

	Describe("Given a valid version core string", [this]()
	{
		TMap<FString, FVersion> ValidVersions =
		{
			{"0.0.1", FVersion{0, 0, 1}},
			{"0.1.0", FVersion{0, 1, 0}},
			{"1.0.0", FVersion{1, 0, 0}},
			{"1.2.0", FVersion{1, 2, 0}},
			{"1.2.3", FVersion{1, 2, 3}},
			{"1.0.3", FVersion{1, 0, 3}},
		};

		for (auto const& Version : ValidVersions)
		{
			It(FString::Printf(TEXT("When create FVersion from \"%s\", Then valid"), *Version.Key), [this, Version]()
			{
				AB_TEST_EQUAL(FVersion{Version.Key}.Compare(Version.Value), 0);

				return true;
			});
		}
	});

	Describe("Given a list of valid version core strings", [this]()
	{
		TArray<FString> const OrderedVersions =
		{
			"0.0.1",
			"0.0.200",
			"0.1.0",
			"0.1.1",
			"0.200.0",
			"0.200.1",
			"0.200.200",
			"1.0.0",
			"1.0.1",
			"1.0.200",
			"1.1.0",
			"1.1.1",
			"1.1.200",
			"1.200.0",
			"1.200.1",
			"1.200.200",
			"200.0.0",
			"200.0.1",
			"200.0.200",
			"200.1.0",
			"200.1.1",
			"200.1.200",
		};

		for (int i = 0; i < OrderedVersions.Num() - 1; i++)
		{
			FString A = OrderedVersions[i];
			FString B = OrderedVersions[i + 1];

			It(FString::Printf(TEXT("When compare \"%s\" and \"%s\", Then it's less than 0"), *A, *B), [this, A, B]()
			{
				AB_TEST_TRUE(FVersion{A}.Compare(FVersion{B}) < 0);
				AB_TEST_TRUE(FVersion{B}.Compare(FVersion{A}) > 0);

				return true;
			});
		}
	});

	Describe("Given a invalid version string with prerelease and build metadata", [this]()
	{
		TArray<FString> InvalidVersions =
		{
			"1.2.3-",
			"1.2.3--",
			"1.2.3+",
			"1.2.3++",
			"1.2.3+-",
			"1.2.3-+",
			"1.2.3-a-b",
			"1.2.3+a+b",
			"1.2.3+a-b",
		};

		for (auto const& Version : InvalidVersions)
		{
			It(FString::Printf(TEXT("When create FVersion from \"%s\", Then invalid"), *Version), [this, Version]()
			{
				AB_TEST_FALSE(FVersion{Version}.IsValid());

				return true;
			});
		}
	});

	Describe("Given a valid version core string with invalid prerelease", [this]()
	{
		TArray<FString> InvalidVersions =
		{
			"0.0.1-.",
			"0.0.1-.aa",
			"0.0.1-.11",
			"0.0.1-aa.",
			"0.0.1-11.",
			"0.0.1-..",
			"0.0.1-.aa.",
			"0.0.1-.11.",
			"0.0.1-..aa",
			"0.0.1-..11",
			"0.0.1-00.0.0",
			"0.0.1-0.00.0",
			"0.0.1-0.0.00",
			"0.0.1-00.00.00",
			"0.0.1-01.0.0",
			"0.0.1-0.01.0",
			"0.0.1-0.0.01",
			"0.0.1-01.01.01",
			"0.0.1-001.0.0",
			"0.0.1-0.001.0",
			"0.0.1-0.0.001",
			"0.0.1-001.001.001",
		};

		for (auto const& Version : InvalidVersions)
		{
			It(FString::Printf(TEXT("When create FVersion from \"%s\", Then invalid"), *Version), [this, Version]()
			{
				AB_TEST_FALSE(FVersion{Version}.IsValid());

				return true;
			});
		}
	});

	Describe("Given a valid version string with prerelease", [this]()
	{
		TMap<FString, FVersion> ValidVersions =
		{
			{"0.0.1-alpha", FVersion{0, 0, 1, TArray<FString>{"alpha"}}},
			{"0.0.1-beta", FVersion{0, 0, 1, TArray<FString>{"beta"}}},
			{"0.0.1-beta.1", FVersion{0, 0, 1,TArray<FString>{"beta", "1"}}},
			{"0.0.1-1.pre", FVersion{0, 0, 1, TArray<FString>{"1", "pre"}}},
		};

		for (auto const& Version : ValidVersions)
		{
			It(FString::Printf(TEXT("When create FVersion from \"%s\", Then valid"), *Version.Key), [this, Version]()
			{
				AB_TEST_TRUE(FVersion{Version.Key}.IsValid());
				AB_TEST_EQUAL(FVersion{Version.Key}.Compare(Version.Value), 0);

				return true;
			});
		}
	});

	Describe("Given a valid version string with prerelease and build metadata", [this]()
	{
		TMap<FString, FVersion> ValidVersions =
		{
			{"0.0.1-alpha+x", FVersion{0, 0, 1, TArray<FString>{"alpha"}}},
			{"0.0.1-beta+123.456", FVersion{0, 0, 1, TArray<FString>{"beta"}}},
			{"0.0.1-beta.1+xyz", FVersion{0, 0, 1, TArray<FString>{"beta", "1"}}},
			{"0.0.1-1.pre+xxx.yyy.111", FVersion{0, 0, 1, TArray<FString>{"1", "pre"}}},
			{"0.0.1+xxx", FVersion{0, 0, 1}},
		};

		for (auto const& Version : ValidVersions)
		{
			It(FString::Printf(TEXT("When create FVersion from \"%s\", Then valid and build metadata ignored"), *Version.Key), [this, Version]()
			{
				AB_TEST_EQUAL(FVersion{Version.Key}.Compare(Version.Value), 0);

				return true;
			});
		}
	});

	Describe("Given a list of ordered valid versions with prerelease", [this]()
	{
		TArray<FString> const OrderedVersions =
		{
			"0.1.0",
			"1.0.0-2",
			"1.0.0-10",
			"1.0.0-alpha",
			"1.0.0-alpha.1",
			"1.0.0-alpha.beta",
			"1.0.0-alpha.delta",
			"1.0.0-beta",
			"1.0.0-beta.2",
			"1.0.0-beta.11",
			"1.0.0-rc.1",
			"1.0.0",
		};

		for (int i = 0; i < OrderedVersions.Num() - 1; i++)
		{
			FString A = OrderedVersions[i];
			FString B = OrderedVersions[i + 1];

			It(FString::Printf(TEXT("When compare \"%s\" and \"%s\", Then it's less than 0"), *A, *B), [this, A, B]()
			{
				AB_TEST_TRUE(FVersion{A}.Compare(FVersion{B}) < 0);
				AB_TEST_TRUE(FVersion{B}.Compare(FVersion{A}) > 0);

				return true;
			});
		}

	});
}



DEFINE_SPEC(
	FServiceVersionCompatibilitySpec,
	"AccelByte.Tests.Core.ServiceVersionCompatibilitySpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

void FServiceVersionCompatibilitySpec::Define()
{
	Describe("Given an invalid JSON string", [this]()
	{
		TArray<FString> InvalidJsons =
		{
			"",
			"null",
			"{}",
			"[]",
			"{ \"servicename\": null }",
			"{ \"servicename\": 123 }",
			"{ \"servicename\": true }",
			"{ \"servicename\": \"string\" }",
			"{ \"servicename\": {} }",
			"{ \"servicename\": [] }",
			"{ \"servicename\": [{}] }",
			"{ \"servicename\": { \"minVersion\": \"\" } }",
			"{ \"servicename\": { \"maxVersion\": \"\" } }",
		};

		for (auto const& Json : InvalidJsons)
		{
			It(FString::Printf(TEXT("When Json is %s, Then Map is empty"), *Json), [this, Json]()
			{
				FServiceCompatibilityMap const Map = FServiceCompatibilityMap::FromJson(Json);

				AB_TEST_EQUAL(Map.GetServices().Num(), 0);

				return true;
			});
		}
	});

	Describe("Given a valid JSON string with 1 service and invalid version string", [this]()
	{
		TArray<FString> JsonWithInvalidVersionStrings =
		{
			"{ \"servicename\": { \"minVersion\": null, \"maxVersion\": null } }",
			"{ \"servicename\": { \"minVersion\": true, \"maxVersion\": false } }",
			"{ \"servicename\": { \"minVersion\": 1.2, \"maxVersion\": 1.2 } }",
			"{ \"servicename\": { \"minVersion\": 1.2.3, \"maxVersion\": 1.2.3 } }",
			"{ \"servicename\": { \"minVersion\": 1.2.3.4, \"maxVersion\": 1.2.3.4 } }",
			"{ \"servicename\": { \"minVersion\": \"invalidString\", \"maxVersion\": \"invalidString\" } }",
			"{ \"servicename\": { \"minVersion\": \".\", \"maxVersion\": \".\" } }",
			"{ \"servicename\": { \"minVersion\": \"..\", \"maxVersion\": \"..\" } }",
			"{ \"servicename\": { \"minVersion\": \"...\", \"maxVersion\": \"...\" } }",
			"{ \"servicename\": { \"minVersion\": \"-\", \"maxVersion\": \"-\" } }",
			"{ \"servicename\": { \"minVersion\": \"--\", \"maxVersion\": \"--\" } }",
			"{ \"servicename\": { \"minVersion\": \"---\", \"maxVersion\": \"---\" } }",
			"{ \"servicename\": { \"minVersion\": \"X\", \"maxVersion\": \"X\" } }",
			"{ \"servicename\": { \"minVersion\": \"X.X\", \"maxVersion\": \"X.X\" } }",
			"{ \"servicename\": { \"minVersion\": \"X.X.X\", \"maxVersion\": \"X.X.X\" } }",
			"{ \"servicename\": { \"minVersion\": \"1\", \"maxVersion\": \"1\" } }",
			"{ \"servicename\": { \"minVersion\": \"1.X\", \"maxVersion\": \"1.X\" } }",
			"{ \"servicename\": { \"minVersion\": \"1.2\", \"maxVersion\": \"1.2\" } }",
			"{ \"servicename\": { \"minVersion\": \"1.2.X\", \"maxVersion\": \"1.2.X\" } }",
			"{ \"servicename\": { \"minVersion\": \"1.2.3.X\", \"maxVersion\": \"1.2.3.X\" } }",
			"{ \"servicename\": { \"minVersion\": \"1.2.3.4\", \"maxVersion\": \"1.2.3.4\" } }",
		};

		for (auto const& Json : JsonWithInvalidVersionStrings)
		{
			It(FString::Printf(TEXT("When Json is %s, Then Map is empty"), *Json), [this, Json]()
			{
				FServiceCompatibilityMap const Map = FServiceCompatibilityMap::FromJson(Json);

				AB_TEST_EQUAL(Map.GetServices().Num(), 0);

				return true;
			});
		}
	});

	Describe("Given a valid JSON with 1 service and valid version string", [this]()
	{
		FString const Json = "{ \"servicename\": { \"minVersion\": \"1.0.0\", \"maxVersion\": \"2.0.0\" } }";
		FServiceCompatibilityMap const Map = FServiceCompatibilityMap::FromJson(Json);

		It("When constructed, Then Map has 1 service", [this, Json, Map]()
		{

			AB_TEST_EQUAL(Map.GetServices().Num(), 1);
			AB_TEST_EQUAL(Map.GetServices()[0], FString{"servicename"});

			return true;
		});

		It("When check with version within range, Then result is error", [this, Map]()
		{
			AB_TEST_FALSE(Map.Check("servicename", "1.0.0").bIsError);
			AB_TEST_FALSE(Map.Check("servicename", "1.2.0").bIsError);
			AB_TEST_FALSE(Map.Check("servicename", "2.0.0-rc.1").bIsError);
			AB_TEST_FALSE(Map.Check("servicename", "2.0.0").bIsError);

			return true;
		});

		It("When check with non existent service, Then result is not error", [this, Map]()
		{
			AB_TEST_TRUE(Map.Check("nothing", "1.0.0").bIsError);

			return true;
		});

		It("When check with version outside range, Then result is error", [this, Map]()
		{
			AB_TEST_TRUE(Map.Check("servicename", "1.0.0-rc1").bIsError);
			AB_TEST_TRUE(Map.Check("servicename", "2.0.1").bIsError);

			return true;
		});
	});
}
