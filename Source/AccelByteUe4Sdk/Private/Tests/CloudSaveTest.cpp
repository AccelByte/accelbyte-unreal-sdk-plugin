// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteCloudSaveApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteCredentials.h"
#include "TestUtilities.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::CloudSave;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCloudSaveTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCloudSaveTest);

const int32 AutomationFlagMaskCloudSave = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

static Credentials User2Creds;
static TSharedPtr<Api::User> User2;
static TSharedPtr<Api::CloudSave> CloudSave2;
static FString KeyUserTest = "UE4SDKKeyUserTest";
static FString KeyPublicUserTest = "UE4SDKKeyPublicUserTest";
static FString UnexistKeyUserTest = "UnexistUnitySDKKeyUserTest";
static FString KeyGameTest = "UE4SDKKeyGameTest";
static FString UnexistKeyGameTest = "UnexistUnitySDKKeyGameTest";
static FJsonObject Record1Test;
static FJsonObject Record2Test;
static FJsonObject NewRecord1Test;

const auto CloudSaveErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteCloudSaveTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveSetup, "AccelByte.Tests.CloudSave.A.Setup", AutomationFlagMaskCloudSave);
bool CloudSaveSetup::RunTest(const FString& Parameters)
{
	// record1Test
	{
		Record1Test.SetNumberField("numRegion", 6);
		Record1Test.SetNumberField("oilsReserve", 125.10);
		Record1Test.SetStringField("islandName", "tartar friendly land");
		TArray<TSharedPtr<FJsonValue>> const Buildings{
			MakeShareable(new FJsonValueString("oilRefinery")),
			MakeShareable(new FJsonValueString("oilWell")),
			MakeShareable(new FJsonValueString("watchTower")),
			MakeShareable(new FJsonValueString("defendsTower"))
		};
		Record1Test.SetArrayField("buildings", Buildings);
		TSharedPtr<FJsonObject> Resources = MakeShareable(new FJsonObject);
		Resources->SetNumberField("gas", 20);
		Resources->SetNumberField("water", 100);
		Resources->SetNumberField("gold", 10);
		Record1Test.SetObjectField("resources", Resources);
	}
	// record2Test
	{
		Record2Test.SetNumberField("numIsland", 2);
		Record2Test.SetNumberField("waterReserve", 199.66);
		Record2Test.SetStringField("countryName", "happyland");
		TArray<TSharedPtr<FJsonValue>> const Islands{
			MakeShareable(new FJsonValueString("smile island")),
			MakeShareable(new FJsonValueString("dance island"))
		};
		Record2Test.SetArrayField("islands", Islands);
		TSharedPtr<FJsonObject> population = MakeShareable(new FJsonObject);
		population->SetNumberField("smile island", 198);
		population->SetNumberField("dance island", 77);
		Record2Test.SetObjectField("population", population);
	}
	// newRecord1Test
	{
		NewRecord1Test.SetNumberField("numRegion", 10);
		NewRecord1Test.SetNumberField("oilsReserve", 100);
		NewRecord1Test.SetStringField("islandName", "salad friendly land");
		TArray<TSharedPtr<FJsonValue>> const Buildings{
			MakeShareable(new FJsonValueString("gasRefinery")),
			MakeShareable(new FJsonValueString("gasWell")),
			MakeShareable(new FJsonValueString("waterTower")),
			MakeShareable(new FJsonValueString("mainTower"))
		};
		NewRecord1Test.SetArrayField("buildings", Buildings);
		TSharedPtr<FJsonObject> resources = MakeShareable(new FJsonObject);
		resources->SetNumberField("gas", 50);
		resources->SetNumberField("water", 70);
		resources->SetNumberField("gold", 30);
		NewRecord1Test.SetObjectField("resources", resources);
	}

	// user1 preps
	bool bUserLoginSuccess= false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("User Login Success"));
		bUserLoginSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bUserLoginSuccess, "Waiting for Login...");

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(KeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	bool bDeleteUserRecord1Success = false;
	FRegistry::CloudSave.DeleteUserRecord(UnexistKeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecord1Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecord1Success, "Waiting for deleting user record ...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(KeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteGameRecord1Success = false;
	FRegistry::CloudSave.DeleteGameRecord(UnexistKeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecord1Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecord1Success, "Waiting for deleting game record ...");

	bool bDeleteUserPublicRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(KeyPublicUserTest, FVoidHandler::CreateLambda([&bDeleteUserPublicRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserPublicRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserPublicRecordSuccess, "Waiting for deleting user record ...");

	// user2 preps
	User2 = MakeShared<Api::User>(User2Creds, FRegistry::Settings);
	FString Email = FString::Printf(TEXT("cloudsaveUE4Test@example.com"));
	Email.ToLowerInline();
	FString const Password = TEXT("123Password123");
	FString const DisplayName = FString::Printf(TEXT("cloudsaveUE4"));
	FString const Country = "US";
	FDateTime const DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
	FString const Format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bUserCreationSuccess = false;
	User2->Register(
		Email,
		Password,
		DisplayName,
		Country,
		Format,
		THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
	{
		bUserCreationSuccess = true;
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Test CloudSave User2 is Created"));

	}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Code=%d"), Code);
		if (static_cast<ErrorCodes>(Code) == ErrorCodes::UserEmailAlreadyUsedException || static_cast<ErrorCodes>(Code) == ErrorCodes::UserDisplayNameAlreadyUsedException) //email already used
		{
			bUserCreationSuccess = true;
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Test CloudSave User2 is already"));
		}
		else
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Test CloudSave User can't be created"));
		}
	}));

	Waiting(bUserCreationSuccess, "Waiting for user created...");

	bool bUser2LoginSuccess = false;
	User2->LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&]()
	{
		bUser2LoginSuccess = true;
	}),
		CloudSaveErrorHandler);

	Waiting(bUser2LoginSuccess, "Waiting for Login...");

	CloudSave2 = MakeShared<Api::CloudSave>(User2Creds, FRegistry::Settings);

	AB_TEST_TRUE(bUserLoginSuccess);
	AB_TEST_TRUE(bUserCreationSuccess);
	AB_TEST_TRUE(bUser2LoginSuccess);
	AB_TEST_TRUE(bDeleteUserRecordSuccess);
	AB_TEST_TRUE(bDeleteUserRecord1Success);
	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	AB_TEST_TRUE(bDeleteGameRecord1Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveSaveUserRecord, "AccelByte.Tests.CloudSave.B.SaveUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveSaveUserRecord::RunTest(const FString& Parameters)
{
	bool bSaveUserRecord1Success = false;
	FRegistry::CloudSave.SaveUserRecord(KeyUserTest, Record1Test, false, FVoidHandler::CreateLambda([&bSaveUserRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record1 success"));
		bSaveUserRecord1Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bSaveUserRecord1Success, "Waiting for saving user record1 ...");
	
	bool bSaveUserRecord2Success = false;
	FRegistry::CloudSave.SaveUserRecord(KeyUserTest, Record2Test, false, FVoidHandler::CreateLambda([&bSaveUserRecord2Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record2 success"));
		bSaveUserRecord2Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bSaveUserRecord2Success, "Waiting for saving user record2 ...");

	AB_TEST_TRUE(bSaveUserRecord1Success);
	AB_TEST_TRUE(bSaveUserRecord2Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveSavePublicUserRecord, "AccelByte.Tests.CloudSave.B.SavePublicUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveSavePublicUserRecord::RunTest(const FString& Parameters)
{
	bool bSaveUserRecord1Success = false;
	FRegistry::CloudSave.SaveUserRecord(KeyPublicUserTest, Record1Test, true, FVoidHandler::CreateLambda([&bSaveUserRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record1 success"));
		bSaveUserRecord1Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bSaveUserRecord1Success, "Waiting for saving user record1 ...");

	bool bSaveUserRecord2Success = false;
	FRegistry::CloudSave.SaveUserRecord(KeyPublicUserTest, Record2Test, true, FVoidHandler::CreateLambda([&bSaveUserRecord2Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record2 success"));
		bSaveUserRecord2Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bSaveUserRecord2Success, "Waiting for saving user record2 ...");

	AB_TEST_TRUE(bSaveUserRecord1Success);
	AB_TEST_TRUE(bSaveUserRecord2Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetUserRecord, "AccelByte.Tests.CloudSave.C.GetUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveGetUserRecord::RunTest(const FString& Parameters)
{
	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetUserRecord(KeyUserTest, THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");
	
	AB_TEST_TRUE(bGetUserRecordSuccess);
	AB_TEST_EQUAL(getUserRecordResult.Key, KeyUserTest);
	AB_TEST_EQUAL(getUserRecordResult.UserId, *FRegistry::Credentials.GetUserId());
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numRegion"), Record1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("oilsReserve"), Record1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("islandName"), Record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), Record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numIsland"), Record2Test.GetNumberField("numIsland"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("waterReserve"), Record2Test.GetNumberField("waterReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("countryName"), Record2Test.GetStringField("countryName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("islands"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record2Test.GetArrayField("islands"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("smile island"), Record2Test.GetObjectField("population").Get()->GetNumberField("smile island"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("dance island"), Record2Test.GetObjectField("population").Get()->GetNumberField("dance island"));
	AB_TEST_FALSE(getUserRecordResult.IsPublic);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetOwnPublicUserRecord, "AccelByte.Tests.CloudSave.C.GetOwnPublicUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveGetOwnPublicUserRecord::RunTest(const FString& Parameters)
{
	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetPublicUserRecord(KeyPublicUserTest, FRegistry::Credentials.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	AB_TEST_TRUE(bGetUserRecordSuccess);
	AB_TEST_EQUAL(getUserRecordResult.Key, KeyPublicUserTest);
	AB_TEST_EQUAL(getUserRecordResult.UserId, *FRegistry::Credentials.GetUserId());
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numRegion"), Record1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("oilsReserve"), Record1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("islandName"), Record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), Record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numIsland"), Record2Test.GetNumberField("numIsland"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("waterReserve"), Record2Test.GetNumberField("waterReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("countryName"), Record2Test.GetStringField("countryName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("islands"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record2Test.GetArrayField("islands"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("smile island"), Record2Test.GetObjectField("population").Get()->GetNumberField("smile island"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("dance island"), Record2Test.GetObjectField("population").Get()->GetNumberField("dance island"));
	AB_TEST_TRUE(getUserRecordResult.IsPublic);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetOtherPublicUserRecord, "AccelByte.Tests.CloudSave.C.GetOtherPublicUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveGetOtherPublicUserRecord::RunTest(const FString& Parameters)
{
	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	CloudSave2->GetPublicUserRecord(KeyPublicUserTest, FRegistry::Credentials.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	AB_TEST_TRUE(bGetUserRecordSuccess);
	AB_TEST_EQUAL(getUserRecordResult.Key, KeyPublicUserTest);
	AB_TEST_EQUAL(getUserRecordResult.UserId, *FRegistry::Credentials.GetUserId());
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numRegion"), Record1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("oilsReserve"), Record1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("islandName"), Record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), Record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numIsland"), Record2Test.GetNumberField("numIsland"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("waterReserve"), Record2Test.GetNumberField("waterReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("countryName"), Record2Test.GetStringField("countryName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("islands"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record2Test.GetArrayField("islands"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("smile island"), Record2Test.GetObjectField("population").Get()->GetNumberField("smile island"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("dance island"), Record2Test.GetObjectField("population").Get()->GetNumberField("dance island"));
	AB_TEST_TRUE(getUserRecordResult.IsPublic);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetUserRecordInvalidKey, "AccelByte.Tests.CloudSave.C.GetUserRecordInvalidKey", AutomationFlagMaskCloudSave);
bool CloudSaveGetUserRecordInvalidKey::RunTest(const FString& Parameters)
{
	bool bGetUserRecordSuccess = true;
	bool bGetUserRecordDone = false;
	FRegistry::CloudSave.GetUserRecord("Invalid", THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordDone](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record invalid key success"));
		bGetUserRecordDone = true;
	}), FErrorHandler::CreateLambda([&bGetUserRecordSuccess, &bGetUserRecordDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record invalid key failed"));
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bGetUserRecordSuccess = false;
		bGetUserRecordDone = true;
	}));
	Waiting(bGetUserRecordDone, "Waiting for get user record invalid key ...");

	AB_TEST_FALSE(bGetUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceUserRecord, "AccelByte.Tests.CloudSave.D.ReplaceUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceUserRecord::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecordSuccess = false;
	FRegistry::CloudSave.ReplaceUserRecord(KeyUserTest, NewRecord1Test, false, FVoidHandler::CreateLambda([&bReplaceUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record success"));
		bReplaceUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bReplaceUserRecordSuccess, "Waiting for replacing user record ...");

	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetUserRecord(KeyUserTest, THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record1 success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	AB_TEST_TRUE(bGetUserRecordSuccess);
	AB_TEST_EQUAL(getUserRecordResult.Key, KeyUserTest);
	AB_TEST_EQUAL(getUserRecordResult.UserId, *FRegistry::Credentials.GetUserId());
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numRegion"), NewRecord1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("oilsReserve"), NewRecord1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("islandName"), NewRecord1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : NewRecord1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplacePublicUserRecord, "AccelByte.Tests.CloudSave.D.ReplacePublicUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplacePublicUserRecord::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecordSuccess = false;
	FRegistry::CloudSave.ReplaceUserRecord(KeyPublicUserTest, NewRecord1Test, true, FVoidHandler::CreateLambda([&bReplaceUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record success"));
		bReplaceUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bReplaceUserRecordSuccess, "Waiting for replacing user record ...");

	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetPublicUserRecord(KeyPublicUserTest, FRegistry::Credentials.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record1 success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	AB_TEST_TRUE(bGetUserRecordSuccess);
	AB_TEST_EQUAL(getUserRecordResult.Key, KeyPublicUserTest);
	AB_TEST_EQUAL(getUserRecordResult.UserId, *FRegistry::Credentials.GetUserId());
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numRegion"), NewRecord1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("oilsReserve"), NewRecord1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("islandName"), NewRecord1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : NewRecord1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveDeleteUserRecord, "AccelByte.Tests.CloudSave.E.DeleteUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveDeleteUserRecord::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(KeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	AB_TEST_TRUE(bDeleteUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveDeleteUserRecordInvalidKey, "AccelByte.Tests.CloudSave.E.DeleteUserRecordInvalidKey", AutomationFlagMaskCloudSave);
bool CloudSaveDeleteUserRecordInvalidKey::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord("Invalid", FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record invalid key success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	AB_TEST_TRUE(bDeleteUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceUserRecordUnexistKey, "AccelByte.Tests.CloudSave.F.ReplaceUserRecordUnexistKey", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceUserRecordUnexistKey::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecordSuccess = false;
	FRegistry::CloudSave.ReplaceUserRecord(UnexistKeyUserTest, Record1Test, false, FVoidHandler::CreateLambda([&bReplaceUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record success"));
		bReplaceUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bReplaceUserRecordSuccess, "Waiting for replacing user record ...");

	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetUserRecord(UnexistKeyUserTest, THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(UnexistKeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	AB_TEST_TRUE(bReplaceUserRecordSuccess);
	AB_TEST_TRUE(bGetUserRecordSuccess);
	AB_TEST_EQUAL(getUserRecordResult.Key, UnexistKeyUserTest);
	AB_TEST_EQUAL(getUserRecordResult.UserId, *FRegistry::Credentials.GetUserId());
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numRegion"), Record1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("oilsReserve"), Record1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("islandName"), Record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), Record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	AB_TEST_TRUE(bDeleteUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceWithSaveUserRecord, "AccelByte.Tests.CloudSave.F.ReplaceWithSaveUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceWithSaveUserRecord::RunTest(const FString& Parameters)
{
	FString Key = "UnexistUnitySDKKeyUserTest";
	bool bSaveUserRecordSuccess = false;
	FRegistry::CloudSave.SaveUserRecord(KeyUserTest, Record1Test, false, FVoidHandler::CreateLambda([&bSaveUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record1 success"));
		bSaveUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bSaveUserRecordSuccess, "Waiting for saving user record ...");

	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetUserRecord(KeyUserTest, THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(KeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	AB_TEST_TRUE(bSaveUserRecordSuccess);
	AB_TEST_TRUE(bGetUserRecordSuccess);
	AB_TEST_EQUAL(getUserRecordResult.Key, KeyUserTest);
	AB_TEST_EQUAL(getUserRecordResult.UserId, *FRegistry::Credentials.GetUserId());
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("numRegion"), Record1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetNumberField("oilsReserve"), Record1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetStringField("islandName"), Record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), Record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	AB_TEST_TRUE(bDeleteUserRecordSuccess);
	return true;
}

#if 0
// Turn on when Concurrent Bug in Back-end solved
IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceUserRecordRacingConditionManual, "AccelByte.Tests.CloudSave.F.ReplaceUserRecordRacingConditionManual", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceUserRecordRacingConditionManual::RunTest(const FString& Parameters)
{
	const FString ConcurrentKey = "UE4KeyConcurrentManualUserRecordTest";
	int DictIndex = 0;
	bool bReplaceRecordSuccess = false;
	bool bReplaceRecordDone = false;
	int32 ErrorCode;
	FString ErrorMessage;
	FJsonObject UpdatedRecord;
	UpdatedRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
	DictIndex++;
	FRegistry::CloudSave.ReplaceUserRecordCheckLatest(ConcurrentKey, FDateTime::Now(), UpdatedRecord, FVoidHandler::CreateLambda([&bReplaceRecordSuccess, &bReplaceRecordDone]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record success"));
		bReplaceRecordSuccess = true;
		bReplaceRecordDone = true;
	}), FErrorHandler::CreateLambda([&bReplaceRecordDone, &ErrorCode, &ErrorMessage](int32 Code, const FString& Message)
	{
		ErrorCode = Code;
		ErrorMessage = Message;
		bReplaceRecordDone = true;
	}));
	Waiting(bReplaceRecordDone, "Waiting for replace user record...");

	bool bReplaceRecord2Success = false;
	bool bReplaceRecord2Done = false;
	int32 ErrorCode2;
	FString ErrorMessage2;
	FJsonObject updatedRecord2;
	updatedRecord2.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
	DictIndex++;
	FRegistry::CloudSave.ReplaceUserRecordCheckLatest(ConcurrentKey, FDateTime::Now(), updatedRecord2, FVoidHandler::CreateLambda([&bReplaceRecord2Success, &bReplaceRecord2Done]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record success"));
		bReplaceRecord2Success = true;
		bReplaceRecord2Done = true;
	}), FErrorHandler::CreateLambda([&bReplaceRecord2Done, &ErrorCode2, &ErrorMessage2](int32 Code, const FString& Message)
	{
		ErrorCode2 = Code;
		ErrorMessage2 = Message;
		bReplaceRecord2Done = true;
	}));
	Waiting(bReplaceRecord2Done, "Waiting for replace user record...");

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(ConcurrentKey, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	AB_TEST_TRUE(bReplaceRecordSuccess || bReplaceRecordDone);
	if (bReplaceRecordDone && !bReplaceRecordSuccess) AB_TEST_EQUAL(ErrorCode, (int32)ErrorCodes::PlayerRecordPreconditionFailedException);
	AB_TEST_FALSE(bReplaceRecord2Success);
	AB_TEST_TRUE(bReplaceRecord2Done);
	AB_TEST_EQUAL(ErrorCode2, (int32)ErrorCodes::PlayerRecordPreconditionFailedException);
	AB_TEST_TRUE(bDeleteUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceUserRecordRacingConditionAuto, "AccelByte.Tests.CloudSave.F.ReplaceUserRecordRacingConditionAuto", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceUserRecordRacingConditionAuto::RunTest(const FString& Parameters)
{
	AB_TEST_SKIP_WHEN_DISABLED();
	// update user record
	int ConcurrentWriteCount = 5;
	int UpdateSuccessCount = 0;
	int UpdateDone = 0;
	const int TryAttempt = ConcurrentWriteCount*2;
	const FString ConcurrentKey = "UE4KeyConcurrentUserRecordTest";
	for (int i = 0; i < ConcurrentWriteCount; i++)
	{
		int DictIndex = i;
		FJsonObject UpdatedRecord;
		UpdatedRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
		FRegistry::CloudSave.ReplaceUserRecordCheckLatest(TryAttempt, ConcurrentKey, UpdatedRecord, THandlerPayloadModifier<FJsonObject, FJsonObject>::CreateLambda([DictIndex](FJsonObject LatestRecord)
		{
			if (!LatestRecord.HasField(FString::Printf(TEXT("key_%d"), DictIndex)))
			{
				LatestRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
			}
			float delay = FMath::RandRange(1.2f, 2.f);
			FPlatformProcess::Sleep(delay);
			return LatestRecord;
		}), FVoidHandler::CreateLambda([DictIndex, &UpdateDone, &UpdateSuccessCount]()
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record %d success"), DictIndex);
			UpdateSuccessCount++;
			UpdateDone++;
		}), FErrorHandler::CreateLambda([DictIndex, &UpdateDone](int32 Code, const FString& Message)
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record %d failed"), DictIndex);
			UpdateDone++;
		}));
		if (i == 0)
		{
			float delay = FMath::RandRange(1.5f, 2.5f);
			FPlatformProcess::Sleep(delay);
		}
	}
	WaitUntil([&UpdateDone, &ConcurrentWriteCount]() {return UpdateDone >= ConcurrentWriteCount;}, 60.0, "Waiting for Update Record done...");

	// get the latest record
	FAccelByteModelsUserRecord GetRecordResult;
	bool bGetRecordSuccess = false;
	FRegistry::CloudSave.GetUserRecord(ConcurrentKey, THandler<FAccelByteModelsUserRecord>::CreateLambda([&GetRecordResult, &bGetRecordSuccess](const FAccelByteModelsUserRecord& Result)
	{
		GetRecordResult = Result;
		bGetRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetRecordSuccess, "Waiting for get user record...");

	bool bAssertValueCount = GetRecordResult.Value.Values.Num() == UpdateSuccessCount;

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(ConcurrentKey, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	AB_TEST_TRUE(bAssertValueCount);
	AB_TEST_TRUE(bDeleteUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceUserRecordAutoExhaustAllRetries, "AccelByte.Tests.CloudSave.F.ReplaceUserRecordAutoExhaustAllRetries", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceUserRecordAutoExhaustAllRetries::RunTest(const FString& Parameters)
{
	AB_TEST_SKIP_WHEN_DISABLED();

	// update user record
	int ConcurrentWriteCount = 5;
	int UpdateSuccessCount = 0;
	int UpdateDone = 0;
	int RetriesExhausted = 0;
	const int TryAttempt = ConcurrentWriteCount/2;
	const FString ConcurrentKey = "UE4KeyConcurrentUserRecordTest";
	for (int i = 0; i < ConcurrentWriteCount; i++)
	{
		int DictIndex = i;
		FJsonObject UpdatedRecord;
		UpdatedRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
		FRegistry::CloudSave.ReplaceUserRecordCheckLatest(TryAttempt, ConcurrentKey, UpdatedRecord, THandlerPayloadModifier<FJsonObject, FJsonObject>::CreateLambda([DictIndex](FJsonObject LatestRecord)
		{
			if (!LatestRecord.HasField(FString::Printf(TEXT("key_%d"), DictIndex)))
			{
				LatestRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
			}
			float delay = FMath::RandRange(1.2f, 2.f);
			FPlatformProcess::Sleep(delay);
			return LatestRecord;
		}), FVoidHandler::CreateLambda([DictIndex, &UpdateDone, &UpdateSuccessCount]()
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record %d success"), DictIndex);
			UpdateSuccessCount++;
			UpdateDone++;
		}), FErrorHandler::CreateLambda([DictIndex, &UpdateDone, &RetriesExhausted](int32 Code, const FString& Message)
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record %d failed"), DictIndex);
			UpdateDone++;
			if (Code == (int32)ErrorCodes::PlayerRecordPreconditionFailedException)
			{
				RetriesExhausted++;
			}
		}));
		if (i == 0)
		{
			float delay = FMath::RandRange(1.5f, 2.5f);
			FPlatformProcess::Sleep(delay);
		}
	}
	WaitUntil([&UpdateDone, &ConcurrentWriteCount]() {return UpdateDone >= ConcurrentWriteCount;}, 60.0, "Waiting for Update Record done...");

	// get the latest record
	FAccelByteModelsUserRecord GetRecordResult;
	bool bGetRecordSuccess = false;
	FRegistry::CloudSave.GetUserRecord(ConcurrentKey, THandler<FAccelByteModelsUserRecord>::CreateLambda([&GetRecordResult, &bGetRecordSuccess](const FAccelByteModelsUserRecord& Result)
	{
		GetRecordResult = Result;
		bGetRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetRecordSuccess, "Waiting for get user record...");

	bool bAssertValueCount = GetRecordResult.Value.Values.Num() == UpdateSuccessCount;

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(ConcurrentKey, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	AB_TEST_TRUE(bAssertValueCount);
	AB_TEST_EQUAL(UpdateSuccessCount + RetriesExhausted, UpdateDone);
	AB_TEST_TRUE(bDeleteUserRecordSuccess);
	return true;
}
#endif

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveSaveGameRecord, "AccelByte.Tests.CloudSave.G.SaveGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveSaveGameRecord::RunTest(const FString& Parameters)
{
	bool bSaveGameRecord1Success = false;
	FRegistry::CloudSave.SaveGameRecord(KeyGameTest, Record1Test, FVoidHandler::CreateLambda([&bSaveGameRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save Game record1 success"));
		bSaveGameRecord1Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bSaveGameRecord1Success, "Waiting for saving game record1 ...");

	bool bSaveGameRecord2Success = false;
	FRegistry::CloudSave.SaveGameRecord(KeyGameTest, Record2Test, FVoidHandler::CreateLambda([&bSaveGameRecord2Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save game record2 success"));
		bSaveGameRecord2Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bSaveGameRecord2Success, "Waiting for saving game record2 ...");

	AB_TEST_TRUE(bSaveGameRecord1Success);
	AB_TEST_TRUE(bSaveGameRecord2Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetGameRecord, "AccelByte.Tests.CloudSave.H.GetGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveGetGameRecord::RunTest(const FString& Parameters)
{
	bool bGetGameRecordSuccess = false;
	FAccelByteModelsGameRecord getGameRecordResult;
	FRegistry::CloudSave.GetGameRecord(KeyGameTest, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &getGameRecordResult](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record success"));
		getGameRecordResult = gameRecord;
		bGetGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetGameRecordSuccess, "Waiting for getting game record ...");

	AB_TEST_TRUE(bGetGameRecordSuccess);
	AB_TEST_EQUAL(getGameRecordResult.Key, KeyGameTest);
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("numRegion"), Record1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("oilsReserve"), Record1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetStringField("islandName"), Record1Test.GetStringField("islandName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), Record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("numIsland"), Record2Test.GetNumberField("numIsland"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("waterReserve"), Record2Test.GetNumberField("waterReserve"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetStringField("countryName"), Record2Test.GetStringField("countryName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("islands"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record2Test.GetArrayField("islands"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("population").Get()->GetNumberField("smile island"), Record2Test.GetObjectField("population").Get()->GetNumberField("smile island"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("population").Get()->GetNumberField("dance island"), Record2Test.GetObjectField("population").Get()->GetNumberField("dance island"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetGameRecordInvalidKey, "AccelByte.Tests.CloudSave.I.GetGameRecordInvalidKey", AutomationFlagMaskCloudSave);
bool CloudSaveGetGameRecordInvalidKey::RunTest(const FString& Parameters)
{
	bool bGetGameRecordSuccess = true;
	bool bGetGameRecordDone = false;
	FRegistry::CloudSave.GetGameRecord("Invalid", THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordDone](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record invalid key success"));
		bGetGameRecordDone = true;
	}), FErrorHandler::CreateLambda([&bGetGameRecordSuccess, &bGetGameRecordDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record invalid key failed"));
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bGetGameRecordSuccess = false;
		bGetGameRecordDone = true;
	}));
	Waiting(bGetGameRecordDone, "Waiting for saving game record invalid key ...");

	AB_TEST_FALSE(bGetGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceGameRecord, "AccelByte.Tests.CloudSave.J.ReplaceGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceGameRecord::RunTest(const FString& Parameters)
{
	bool bReplaceGameRecordSuccess = false;
	FRegistry::CloudSave.ReplaceGameRecord(KeyGameTest, NewRecord1Test, FVoidHandler::CreateLambda([&bReplaceGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record success"));
		bReplaceGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bReplaceGameRecordSuccess, "Waiting for replacing game record ...");

	bool bGetGameRecordSuccess = false;
	FAccelByteModelsGameRecord getGameRecordResult;
	FRegistry::CloudSave.GetGameRecord(KeyGameTest, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &getGameRecordResult](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record1 success"));
		getGameRecordResult = gameRecord;
		bGetGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetGameRecordSuccess, "Waiting for getting game record ...");

	AB_TEST_TRUE(bGetGameRecordSuccess);
	AB_TEST_EQUAL(getGameRecordResult.Key, KeyGameTest);
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("numRegion"), NewRecord1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("oilsReserve"), NewRecord1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetStringField("islandName"), NewRecord1Test.GetStringField("islandName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : NewRecord1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), NewRecord1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	return true;
}

#if 0
// Turn on when Concurrent Bug in Back-end solved
IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceGameRecordRacingConditionManual, "AccelByte.Tests.CloudSave.J.ReplaceGameRecordRacingConditionManual", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceGameRecordRacingConditionManual::RunTest(const FString& Parameters)
{
	const FString ConcurrentKey = "UE4KeyConcurrentManualGameRecordTest";
	int DictIndex = 0;
	bool bReplaceRecordSuccess = false;
	bool bReplaceRecordDone = false;
	int32 ErrorCode;
	FString ErrorMessage;
	FJsonObject UpdatedRecord;
	UpdatedRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
	DictIndex++;
	FRegistry::CloudSave.ReplaceGameRecordCheckLatest(ConcurrentKey, FDateTime::Now(), UpdatedRecord, FVoidHandler::CreateLambda([&bReplaceRecordSuccess, &bReplaceRecordDone]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record success"));
		bReplaceRecordSuccess = true;
		bReplaceRecordDone = true;
	}), FErrorHandler::CreateLambda([&bReplaceRecordDone, &ErrorCode, &ErrorMessage](int32 Code, const FString& Message)
	{
		ErrorCode = Code;
		ErrorMessage = Message;
		bReplaceRecordDone = true;
	}));
	Waiting(bReplaceRecordDone, "Waiting for replace game record...");

	bool bReplaceRecord2Success = false;
	bool bReplaceRecord2Done = false;
	int32 ErrorCode2;
	FString ErrorMessage2;
	FJsonObject updatedRecord2;
	updatedRecord2.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
	DictIndex++;
	FRegistry::CloudSave.ReplaceGameRecordCheckLatest(ConcurrentKey, FDateTime::Now(), updatedRecord2, FVoidHandler::CreateLambda([&bReplaceRecord2Success, &bReplaceRecord2Done]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record success"));
		bReplaceRecord2Success = true;
		bReplaceRecord2Done = true;
	}), FErrorHandler::CreateLambda([&bReplaceRecord2Done, &ErrorCode2, &ErrorMessage2](int32 Code, const FString& Message)
	{
		ErrorCode2 = Code;
		ErrorMessage2 = Message;
		bReplaceRecord2Done = true;
	}));
	Waiting(bReplaceRecord2Done, "Waiting for replace game record...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(ConcurrentKey, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	AB_TEST_TRUE(bReplaceRecordSuccess || bReplaceRecordDone);
	if (bReplaceRecordDone && !bReplaceRecordSuccess) AB_TEST_EQUAL(ErrorCode, (int32)ErrorCodes::GameRecordPreconditionFailedException);
	AB_TEST_FALSE(bReplaceRecord2Success);
	AB_TEST_TRUE(bReplaceRecord2Done);
	AB_TEST_EQUAL(ErrorCode2, (int32)ErrorCodes::GameRecordPreconditionFailedException);
	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceGameRecordRacingConditionAuto, "AccelByte.Tests.CloudSave.J.ReplaceGameRecordRacingConditionAuto", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceGameRecordRacingConditionAuto::RunTest(const FString& Parameters)
{
	AB_TEST_SKIP_WHEN_DISABLED();
	// update game record
	int ConcurrentWriteCount = 5;
	int UpdateSuccessCount = 0;
	int UpdateDone = 0;
	const int TryAttempt = ConcurrentWriteCount * 2;
	const FString ConcurrentKey = "UE4KeyConcurrentGameRecordTest";
	for (int i = 0; i < ConcurrentWriteCount; i++)
	{
		int DictIndex = i;
		FJsonObject UpdatedRecord;
		UpdatedRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
		FRegistry::CloudSave.ReplaceGameRecordCheckLatest(TryAttempt, ConcurrentKey, UpdatedRecord, THandlerPayloadModifier<FJsonObject, FJsonObject>::CreateLambda([DictIndex](FJsonObject LatestRecord)
		{
			if (!LatestRecord.HasField(FString::Printf(TEXT("key_%d"), DictIndex)))
			{
				LatestRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
			}
			float delay = FMath::RandRange(1.2f, 2.f);
			FPlatformProcess::Sleep(delay);
			return LatestRecord;
		}), FVoidHandler::CreateLambda([DictIndex, &UpdateDone, &UpdateSuccessCount]()
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record %d success"), DictIndex);
			UpdateSuccessCount++;
			UpdateDone++;
		}), FErrorHandler::CreateLambda([DictIndex, &UpdateDone](int32 Code, const FString& Message)
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record %d failed"), DictIndex);
			UpdateDone++;
		}));
		if (i == 0)
		{
			float delay = FMath::RandRange(1.5f, 2.5f);
			FPlatformProcess::Sleep(delay);
		}
	}
	WaitUntil([&UpdateDone, &ConcurrentWriteCount]() {return UpdateDone >= ConcurrentWriteCount;}, 60.0, "Waiting for Update Record done...");

	// get the latest record
	FAccelByteModelsGameRecord GetRecordResult;
	bool bGetRecordSuccess = false;
	FRegistry::CloudSave.GetGameRecord(ConcurrentKey, THandler<FAccelByteModelsGameRecord>::CreateLambda([&GetRecordResult, &bGetRecordSuccess](const FAccelByteModelsGameRecord& Result)
	{
		GetRecordResult = Result;
		bGetRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetRecordSuccess, "Waiting for get game record...");

	bool bAssertValueCount = GetRecordResult.Value.Values.Num() == UpdateSuccessCount;

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(ConcurrentKey, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	AB_TEST_TRUE(bAssertValueCount);
	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceGameRecordAutoExhaustAllRetries, "AccelByte.Tests.CloudSave.J.ReplaceGameRecordAutoExhaustAllRetries", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceGameRecordAutoExhaustAllRetries::RunTest(const FString& Parameters)
{
	AB_TEST_SKIP_WHEN_DISABLED();
	// update game record
	int ConcurrentWriteCount = 5;
	int UpdateSuccessCount = 0;
	int UpdateDone = 0;
	int RetriesExhausted = 0;
	const int TryAttempt = ConcurrentWriteCount / 2;
	const FString ConcurrentKey = "UE4KeyConcurrentGameRecordTest";
	for (int i = 0; i < ConcurrentWriteCount; i++)
	{
		int DictIndex = i;
		FJsonObject UpdatedRecord;
		UpdatedRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
		FRegistry::CloudSave.ReplaceGameRecordCheckLatest(TryAttempt, ConcurrentKey, UpdatedRecord, THandlerPayloadModifier<FJsonObject, FJsonObject>::CreateLambda([DictIndex](FJsonObject LatestRecord)
		{
			if (!LatestRecord.HasField(FString::Printf(TEXT("key_%d"), DictIndex)))
			{
				LatestRecord.SetNumberField(FString::Printf(TEXT("key_%d"), DictIndex), DictIndex);
			}
			float delay = FMath::RandRange(1.2f, 2.f);
			FPlatformProcess::Sleep(delay);
			return LatestRecord;
		}), FVoidHandler::CreateLambda([DictIndex, &UpdateDone, &UpdateSuccessCount]()
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record %d success"), DictIndex);
			UpdateSuccessCount++;
			UpdateDone++;
		}), FErrorHandler::CreateLambda([DictIndex, &UpdateDone, &RetriesExhausted](int32 Code, const FString& Message)
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record %d failed"), DictIndex);
			UpdateDone++;
			if (Code == (int32)ErrorCodes::GameRecordPreconditionFailedException)
			{
				RetriesExhausted++;
			}
		}));
		if (i == 0)
		{
			float delay = FMath::RandRange(1.5f, 2.5f);
			FPlatformProcess::Sleep(delay);
		}
	}
	WaitUntil([&UpdateDone, &ConcurrentWriteCount]() {return UpdateDone >= ConcurrentWriteCount;}, 60.0, "Waiting for Update Record done...");

	// get the latest record
	FAccelByteModelsGameRecord GetRecordResult;
	bool bGetRecordSuccess = false;
	FRegistry::CloudSave.GetGameRecord(ConcurrentKey, THandler<FAccelByteModelsGameRecord>::CreateLambda([&GetRecordResult, &bGetRecordSuccess](const FAccelByteModelsGameRecord& Result)
	{
		GetRecordResult = Result;
		bGetRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetRecordSuccess, "Waiting for get game record...");

	bool bAssertValueCount = GetRecordResult.Value.Values.Num() == UpdateSuccessCount;

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(ConcurrentKey, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	AB_TEST_TRUE(bAssertValueCount);
	AB_TEST_EQUAL(UpdateSuccessCount + RetriesExhausted, UpdateDone);
	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	return true;
}
#endif

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveDeleteGameRecord, "AccelByte.Tests.CloudSave.K.DeleteGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveDeleteGameRecord::RunTest(const FString& Parameters)
{
	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(KeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveDeleteGameRecordInvalidKey, "AccelByte.Tests.CloudSave.K.DeleteGameRecordInvalidKey", AutomationFlagMaskCloudSave);
bool CloudSaveDeleteGameRecordInvalidKey::RunTest(const FString& Parameters)
{
	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord("Invalid", FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record invalid key success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceGameRecordUnexistKey, "AccelByte.Tests.CloudSave.L.ReplaceGameRecordUnexistKey", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceGameRecordUnexistKey::RunTest(const FString& Parameters)
{
	bool bReplaceGameRecordSuccess = false;
	FRegistry::CloudSave.ReplaceGameRecord(UnexistKeyGameTest, Record1Test, FVoidHandler::CreateLambda([&bReplaceGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record success"));
		bReplaceGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bReplaceGameRecordSuccess, "Waiting for replacing game record ...");

	bool bGetGameRecordSuccess = false;
	FAccelByteModelsGameRecord getGameRecordResult;
	FRegistry::CloudSave.GetGameRecord(UnexistKeyGameTest, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &getGameRecordResult](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record success"));
		getGameRecordResult = gameRecord;
		bGetGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetGameRecordSuccess, "Waiting for getting game record ...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(UnexistKeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	AB_TEST_TRUE(bReplaceGameRecordSuccess);
	AB_TEST_TRUE(bGetGameRecordSuccess);
	AB_TEST_EQUAL(getGameRecordResult.Key, UnexistKeyGameTest);
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("numRegion"), Record1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("oilsReserve"), Record1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetStringField("islandName"), Record1Test.GetStringField("islandName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), Record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceWithSaveGameRecord, "AccelByte.Tests.CloudSave.L.ReplaceWithSaveGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceWithSaveGameRecord::RunTest(const FString& Parameters)
{
	FString Key	= "UnexistUnitySDKKeyGameTest";
	bool bSaveGameRecordSuccess = false;
	FRegistry::CloudSave.SaveGameRecord(KeyGameTest, Record1Test, FVoidHandler::CreateLambda([&bSaveGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save game record1 success"));
		bSaveGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bSaveGameRecordSuccess, "Waiting for saving game record ...");

	bool bGetGameRecordSuccess = false;
	FAccelByteModelsGameRecord getGameRecordResult;
	FRegistry::CloudSave.GetGameRecord(KeyGameTest, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &getGameRecordResult](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record success"));
		getGameRecordResult = gameRecord;
		bGetGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bGetGameRecordSuccess, "Waiting for getting game record ...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(KeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	AB_TEST_TRUE(bSaveGameRecordSuccess);
	AB_TEST_TRUE(bGetGameRecordSuccess);
	AB_TEST_EQUAL(getGameRecordResult.Key, KeyGameTest);
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("numRegion"), Record1Test.GetNumberField("numRegion"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetNumberField("oilsReserve"), Record1Test.GetNumberField("oilsReserve"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetStringField("islandName"), Record1Test.GetStringField("islandName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : Record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		AB_TEST_TRUE(bItemFound);
	}
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold"), Record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	AB_TEST_EQUAL(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water"), Record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveTearDown, "AccelByte.Tests.CloudSave.Z.TearDown", AutomationFlagMaskCloudSave);
bool CloudSaveTearDown::RunTest(const FString& Parameters)
{
	bool bUserLoginSuccess= false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
    {
        UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("User Login Success"));
        bUserLoginSuccess = true;
    }), CloudSaveErrorHandler);
	Waiting(bUserLoginSuccess, "Waiting for Login...");

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(KeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	bool bDeleteUserRecord1Success = false;
	FRegistry::CloudSave.DeleteUserRecord(UnexistKeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecord1Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(KeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteGameRecord1Success = false;
	FRegistry::CloudSave.DeleteGameRecord(UnexistKeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecord1Success = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteSuccess = false;
	UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user by id success"));
		bDeleteSuccess = true;
	}), CloudSaveErrorHandler);
	Waiting(bDeleteSuccess, "Waiting for user deletion...");

	return true;
}