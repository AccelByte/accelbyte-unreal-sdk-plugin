// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteCloudSaveApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::CloudSave;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCloudSaveTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCloudSaveTest);

const int32 AutomationFlagMaskCloudSave = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

Credentials user2Creds;
TSharedPtr<Api::User> user2;
TSharedPtr<Api::CloudSave> cloudSave2;
FString keyUserTest = "UE4SDKKeyUserTest";
FString keyPublicUserTest = "UE4SDKKeyPublicUserTest";
FString unexistKeyUserTest = "UnexistUnitySDKKeyUserTest";
FString keyGameTest = "UE4SDKKeyGameTest";
FString unexistKeyGameTest = "UnexistUnitySDKKeyGameTest";
FJsonObject record1Test;
FJsonObject record2Test;
FJsonObject newRecord1Test;

const auto CloudSaveErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteCloudSaveTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveSetup, "AccelByte.Tests.CloudSave.A.Setup", AutomationFlagMaskCloudSave);
bool CloudSaveSetup::RunTest(const FString& Parameters)
{
	// record1Test
	{
		record1Test.SetNumberField("numRegion", 6);
		record1Test.SetNumberField("oilsReserve", 125.10);
		record1Test.SetStringField("islandName", "tartar friendly land");
		TArray<TSharedPtr<FJsonValue>> buildings{
			MakeShareable(new FJsonValueString("oilRefinery")),
			MakeShareable(new FJsonValueString("oilWell")),
			MakeShareable(new FJsonValueString("watchTower")),
			MakeShareable(new FJsonValueString("defendsTower"))
		};
		record1Test.SetArrayField("buildings", buildings);
		TSharedPtr<FJsonObject> resources = MakeShareable(new FJsonObject);
		resources->SetNumberField("gas", 20);
		resources->SetNumberField("water", 100);
		resources->SetNumberField("gold", 10);
		record1Test.SetObjectField("resources", resources);
	}
	// record2Test
	{
		record2Test.SetNumberField("numIsland", 2);
		record2Test.SetNumberField("waterReserve", 199.66);
		record2Test.SetStringField("countryName", "happyland");
		TArray<TSharedPtr<FJsonValue>> islands{
			MakeShareable(new FJsonValueString("smile island")),
			MakeShareable(new FJsonValueString("dance island"))
		};
		record2Test.SetArrayField("islands", islands);
		TSharedPtr<FJsonObject> population = MakeShareable(new FJsonObject);
		population->SetNumberField("smile island", 198);
		population->SetNumberField("dance island", 77);
		record2Test.SetObjectField("population", population);
	}
	// newRecord1Test
	{
		newRecord1Test.SetNumberField("numRegion", 10);
		newRecord1Test.SetNumberField("oilsReserve", 100);
		newRecord1Test.SetStringField("islandName", "salad friendly land");
		TArray<TSharedPtr<FJsonValue>> buildings{
			MakeShareable(new FJsonValueString("gasRefinery")),
			MakeShareable(new FJsonValueString("gasWell")),
			MakeShareable(new FJsonValueString("waterTower")),
			MakeShareable(new FJsonValueString("mainTower"))
		};
		newRecord1Test.SetArrayField("buildings", buildings);
		TSharedPtr<FJsonObject> resources = MakeShareable(new FJsonObject);
		resources->SetNumberField("gas", 50);
		resources->SetNumberField("water", 70);
		resources->SetNumberField("gold", 30);
		newRecord1Test.SetObjectField("resources", resources);
	}

	// user1 preps
	bool bUserLoginSuccess= false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("User Login Success"));
		bUserLoginSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bUserLoginSuccess, "Waiting for Login...");

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(keyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	bool bDeleteUserRecord1Success = false;
	FRegistry::CloudSave.DeleteUserRecord(unexistKeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecord1Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserRecord1Success, "Waiting for deleting user record ...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(keyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteGameRecord1Success = false;
	FRegistry::CloudSave.DeleteGameRecord(unexistKeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecord1Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteGameRecord1Success, "Waiting for deleting game record ...");

	bool bDeleteUserPublicRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(keyPublicUserTest, FVoidHandler::CreateLambda([&bDeleteUserPublicRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserPublicRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserPublicRecordSuccess, "Waiting for deleting user record ...");

	// user2 preps
	user2 = MakeShared<Api::User>(user2Creds, FRegistry::Settings);
	FString Email = FString::Printf(TEXT("cloudsaveUE4Test@example.com"));
	Email.ToLowerInline();
	FString Password = TEXT("123Password123");
	FString DisplayName = FString::Printf(TEXT("cloudsaveUE4"));
	FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bUserCreationSuccess = false;
	user2->Register(
		Email,
		Password,
		DisplayName,
		Country,
		format,
		THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
	{
		bUserCreationSuccess = true;
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Test CloudSave User2 is Created"));

	}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Code=%d"), Code);
		if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException || (ErrorCodes)Code == ErrorCodes::UserDisplayNameAlreadyUsedException) //email already used
		{
			bUserCreationSuccess = true;
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Test CloudSave User2 is already"));
		}
		else
		{
			UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Test CloudSave User can't be created"));
		}
	}));

	FlushHttpRequests();
	Waiting(bUserCreationSuccess, "Waiting for user created...");

	bool bUser2LoginSuccess = false;
	user2->LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&]()
	{
		bUser2LoginSuccess = true;
	}),
		CloudSaveErrorHandler);

	FlushHttpRequests();
	Waiting(bUserLoginSuccess, "Waiting for Login...");

	cloudSave2 = MakeShared<Api::CloudSave>(user2Creds, FRegistry::Settings);

	check(bUserLoginSuccess);
	check(bUserCreationSuccess);
	check(bUser2LoginSuccess);
	check(bDeleteUserRecordSuccess);
	check(bDeleteUserRecord1Success);
	check(bDeleteGameRecordSuccess);
	check(bDeleteGameRecord1Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveSaveUserRecord, "AccelByte.Tests.CloudSave.B.SaveUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveSaveUserRecord::RunTest(const FString& Parameters)
{
	bool bSaveUserRecord1Success = false;
	FRegistry::CloudSave.SaveUserRecord(keyUserTest, record1Test, false, FVoidHandler::CreateLambda([&bSaveUserRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record1 success"));
		bSaveUserRecord1Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSaveUserRecord1Success, "Waiting for saving user record1 ...");
	
	bool bSaveUserRecord2Success = false;
	FRegistry::CloudSave.SaveUserRecord(keyUserTest, record2Test, false, FVoidHandler::CreateLambda([&bSaveUserRecord2Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record2 success"));
		bSaveUserRecord2Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSaveUserRecord2Success, "Waiting for saving user record2 ...");

	check(bSaveUserRecord1Success);
	check(bSaveUserRecord2Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveSavePublicUserRecord, "AccelByte.Tests.CloudSave.B.SavePublicUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveSavePublicUserRecord::RunTest(const FString& Parameters)
{
	bool bSaveUserRecord1Success = false;
	FRegistry::CloudSave.SaveUserRecord(keyPublicUserTest, record1Test, true, FVoidHandler::CreateLambda([&bSaveUserRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record1 success"));
		bSaveUserRecord1Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSaveUserRecord1Success, "Waiting for saving user record1 ...");

	bool bSaveUserRecord2Success = false;
	FRegistry::CloudSave.SaveUserRecord(keyPublicUserTest, record2Test, true, FVoidHandler::CreateLambda([&bSaveUserRecord2Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record2 success"));
		bSaveUserRecord2Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSaveUserRecord2Success, "Waiting for saving user record2 ...");

	check(bSaveUserRecord1Success);
	check(bSaveUserRecord2Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetUserRecord, "AccelByte.Tests.CloudSave.C.GetUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveGetUserRecord::RunTest(const FString& Parameters)
{
	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetUserRecord(keyUserTest, THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");
	
	check(bGetUserRecordSuccess);
	check(getUserRecordResult.Key == keyUserTest);
	check(getUserRecordResult.UserId == *FRegistry::Credentials.GetUserId());
	check(getUserRecordResult.Value.GetNumberField("numRegion") == record1Test.GetNumberField("numRegion"));
	check(getUserRecordResult.Value.GetNumberField("oilsReserve") == record1Test.GetNumberField("oilsReserve"));
	check(getUserRecordResult.Value.GetStringField("islandName") == record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	check(getUserRecordResult.Value.GetNumberField("numIsland") == record2Test.GetNumberField("numIsland"));
	check(getUserRecordResult.Value.GetNumberField("waterReserve") == record2Test.GetNumberField("waterReserve"));
	check(getUserRecordResult.Value.GetStringField("countryName") == record2Test.GetStringField("countryName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("islands"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record2Test.GetArrayField("islands"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("smile island") == record2Test.GetObjectField("population").Get()->GetNumberField("smile island"));
	check(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("dance island") == record2Test.GetObjectField("population").Get()->GetNumberField("dance island"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetOwnPublicUserRecord, "AccelByte.Tests.CloudSave.C.GetOwnPublicUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveGetOwnPublicUserRecord::RunTest(const FString& Parameters)
{
	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetPublicUserRecord(keyPublicUserTest, FRegistry::Credentials.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	check(bGetUserRecordSuccess);
	check(getUserRecordResult.Key == keyPublicUserTest);
	check(getUserRecordResult.UserId == *FRegistry::Credentials.GetUserId());
	check(getUserRecordResult.Value.GetNumberField("numRegion") == record1Test.GetNumberField("numRegion"));
	check(getUserRecordResult.Value.GetNumberField("oilsReserve") == record1Test.GetNumberField("oilsReserve"));
	check(getUserRecordResult.Value.GetStringField("islandName") == record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	check(getUserRecordResult.Value.GetNumberField("numIsland") == record2Test.GetNumberField("numIsland"));
	check(getUserRecordResult.Value.GetNumberField("waterReserve") == record2Test.GetNumberField("waterReserve"));
	check(getUserRecordResult.Value.GetStringField("countryName") == record2Test.GetStringField("countryName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("islands"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record2Test.GetArrayField("islands"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("smile island") == record2Test.GetObjectField("population").Get()->GetNumberField("smile island"));
	check(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("dance island") == record2Test.GetObjectField("population").Get()->GetNumberField("dance island"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetOtherPublicUserRecord, "AccelByte.Tests.CloudSave.C.GetOtherPublicUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveGetOtherPublicUserRecord::RunTest(const FString& Parameters)
{
	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	cloudSave2->GetPublicUserRecord(keyPublicUserTest, FRegistry::Credentials.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	check(bGetUserRecordSuccess);
	check(getUserRecordResult.Key == keyPublicUserTest);
	check(getUserRecordResult.UserId == *FRegistry::Credentials.GetUserId());
	check(getUserRecordResult.Value.GetNumberField("numRegion") == record1Test.GetNumberField("numRegion"));
	check(getUserRecordResult.Value.GetNumberField("oilsReserve") == record1Test.GetNumberField("oilsReserve"));
	check(getUserRecordResult.Value.GetStringField("islandName") == record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	check(getUserRecordResult.Value.GetNumberField("numIsland") == record2Test.GetNumberField("numIsland"));
	check(getUserRecordResult.Value.GetNumberField("waterReserve") == record2Test.GetNumberField("waterReserve"));
	check(getUserRecordResult.Value.GetStringField("countryName") == record2Test.GetStringField("countryName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("islands"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record2Test.GetArrayField("islands"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("smile island") == record2Test.GetObjectField("population").Get()->GetNumberField("smile island"));
	check(getUserRecordResult.Value.GetObjectField("population").Get()->GetNumberField("dance island") == record2Test.GetObjectField("population").Get()->GetNumberField("dance island"));
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
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetUserRecordDone, "Waiting for saving user record invalid key ...");

	check(!bGetUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceUserRecord, "AccelByte.Tests.CloudSave.D.ReplaceUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceUserRecord::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecordSuccess = false;
	FRegistry::CloudSave.ReplaceUserRecord(keyUserTest, newRecord1Test, false, FVoidHandler::CreateLambda([&bReplaceUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record success"));
		bReplaceUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bReplaceUserRecordSuccess, "Waiting for replacing user record ...");

	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetUserRecord(keyUserTest, THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record1 success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	check(bGetUserRecordSuccess);
	check(getUserRecordResult.Key == keyUserTest);
	check(getUserRecordResult.UserId == *FRegistry::Credentials.GetUserId());
	check(getUserRecordResult.Value.GetNumberField("numRegion") == newRecord1Test.GetNumberField("numRegion"));
	check(getUserRecordResult.Value.GetNumberField("oilsReserve") == newRecord1Test.GetNumberField("oilsReserve"));
	check(getUserRecordResult.Value.GetStringField("islandName") == newRecord1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : newRecord1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplacePublicUserRecord, "AccelByte.Tests.CloudSave.D.ReplacePublicUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplacePublicUserRecord::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecordSuccess = false;
	FRegistry::CloudSave.ReplaceUserRecord(keyPublicUserTest, newRecord1Test, true, FVoidHandler::CreateLambda([&bReplaceUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record success"));
		bReplaceUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bReplaceUserRecordSuccess, "Waiting for replacing user record ...");

	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetPublicUserRecord(keyPublicUserTest, FRegistry::Credentials.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record1 success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	check(bGetUserRecordSuccess);
	check(getUserRecordResult.Key == keyPublicUserTest);
	check(getUserRecordResult.UserId == *FRegistry::Credentials.GetUserId());
	check(getUserRecordResult.Value.GetNumberField("numRegion") == newRecord1Test.GetNumberField("numRegion"));
	check(getUserRecordResult.Value.GetNumberField("oilsReserve") == newRecord1Test.GetNumberField("oilsReserve"));
	check(getUserRecordResult.Value.GetStringField("islandName") == newRecord1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : newRecord1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveDeleteUserRecord, "AccelByte.Tests.CloudSave.E.DeleteUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveDeleteUserRecord::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(keyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	check(bDeleteUserRecordSuccess);
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
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	check(bDeleteUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceUserRecordUnexistKey, "AccelByte.Tests.CloudSave.F.ReplaceUserRecordUnexistKey", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceUserRecordUnexistKey::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecordSuccess = false;
	FRegistry::CloudSave.ReplaceUserRecord(unexistKeyUserTest, record1Test, false, FVoidHandler::CreateLambda([&bReplaceUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace user record success"));
		bReplaceUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bReplaceUserRecordSuccess, "Waiting for replacing user record ...");

	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetUserRecord(unexistKeyUserTest, THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(unexistKeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	check(bReplaceUserRecordSuccess);
	check(bGetUserRecordSuccess);
	check(getUserRecordResult.Key == unexistKeyUserTest);
	check(getUserRecordResult.UserId == *FRegistry::Credentials.GetUserId());
	check(getUserRecordResult.Value.GetNumberField("numRegion") == record1Test.GetNumberField("numRegion"));
	check(getUserRecordResult.Value.GetNumberField("oilsReserve") == record1Test.GetNumberField("oilsReserve"));
	check(getUserRecordResult.Value.GetStringField("islandName") == record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	check(bDeleteUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceWithSaveUserRecord, "AccelByte.Tests.CloudSave.F.ReplaceWithSaveUserRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceWithSaveUserRecord::RunTest(const FString& Parameters)
{
	FString key = "UnexistUnitySDKKeyUserTest";
	bool bSaveUserRecordSuccess = false;
	FRegistry::CloudSave.SaveUserRecord(keyUserTest, record1Test, false, FVoidHandler::CreateLambda([&bSaveUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save user record1 success"));
		bSaveUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSaveUserRecordSuccess, "Waiting for saving user record ...");

	bool bGetUserRecordSuccess = false;
	FAccelByteModelsUserRecord getUserRecordResult;
	FRegistry::CloudSave.GetUserRecord(keyUserTest, THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetUserRecordSuccess, &getUserRecordResult](FAccelByteModelsUserRecord userRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get user record success"));
		getUserRecordResult = userRecord;
		bGetUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetUserRecordSuccess, "Waiting for getting user record ...");

	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(keyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	check(bSaveUserRecordSuccess);
	check(bGetUserRecordSuccess);
	check(getUserRecordResult.Key == keyUserTest);
	check(getUserRecordResult.UserId == *FRegistry::Credentials.GetUserId());
	check(getUserRecordResult.Value.GetNumberField("numRegion") == record1Test.GetNumberField("numRegion"));
	check(getUserRecordResult.Value.GetNumberField("oilsReserve") == record1Test.GetNumberField("oilsReserve"));
	check(getUserRecordResult.Value.GetStringField("islandName") == record1Test.GetStringField("islandName"));
	for (auto buildingResult : getUserRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getUserRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	check(bDeleteUserRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveSaveGameRecord, "AccelByte.Tests.CloudSave.G.SaveGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveSaveGameRecord::RunTest(const FString& Parameters)
{
	bool bSaveGameRecord1Success = false;
	FRegistry::CloudSave.SaveGameRecord(keyGameTest, record1Test, FVoidHandler::CreateLambda([&bSaveGameRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save Game record1 success"));
		bSaveGameRecord1Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSaveGameRecord1Success, "Waiting for saving game record1 ...");

	bool bSaveGameRecord2Success = false;
	FRegistry::CloudSave.SaveGameRecord(keyGameTest, record2Test, FVoidHandler::CreateLambda([&bSaveGameRecord2Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save game record2 success"));
		bSaveGameRecord2Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSaveGameRecord2Success, "Waiting for saving game record2 ...");

	check(bSaveGameRecord1Success);
	check(bSaveGameRecord2Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveGetGameRecord, "AccelByte.Tests.CloudSave.H.GetGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveGetGameRecord::RunTest(const FString& Parameters)
{
	bool bGetGameRecordSuccess = false;
	FAccelByteModelsGameRecord getGameRecordResult;
	FRegistry::CloudSave.GetGameRecord(keyGameTest, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &getGameRecordResult](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record success"));
		getGameRecordResult = gameRecord;
		bGetGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetGameRecordSuccess, "Waiting for getting game record ...");

	check(bGetGameRecordSuccess);
	check(getGameRecordResult.Key == keyGameTest);
	check(getGameRecordResult.Value.GetNumberField("numRegion") == record1Test.GetNumberField("numRegion"));
	check(getGameRecordResult.Value.GetNumberField("oilsReserve") == record1Test.GetNumberField("oilsReserve"));
	check(getGameRecordResult.Value.GetStringField("islandName") == record1Test.GetStringField("islandName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	check(getGameRecordResult.Value.GetNumberField("numIsland") == record2Test.GetNumberField("numIsland"));
	check(getGameRecordResult.Value.GetNumberField("waterReserve") == record2Test.GetNumberField("waterReserve"));
	check(getGameRecordResult.Value.GetStringField("countryName") == record2Test.GetStringField("countryName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("islands"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record2Test.GetArrayField("islands"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getGameRecordResult.Value.GetObjectField("population").Get()->GetNumberField("smile island") == record2Test.GetObjectField("population").Get()->GetNumberField("smile island"));
	check(getGameRecordResult.Value.GetObjectField("population").Get()->GetNumberField("dance island") == record2Test.GetObjectField("population").Get()->GetNumberField("dance island"));
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
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetGameRecordDone, "Waiting for saving game record invalid key ...");

	check(!bGetGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceGameRecord, "AccelByte.Tests.CloudSave.J.ReplaceGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceGameRecord::RunTest(const FString& Parameters)
{
	bool bReplaceGameRecordSuccess = false;
	FRegistry::CloudSave.ReplaceGameRecord(keyGameTest, newRecord1Test, FVoidHandler::CreateLambda([&bReplaceGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record success"));
		bReplaceGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bReplaceGameRecordSuccess, "Waiting for replacing game record ...");

	bool bGetGameRecordSuccess = false;
	FAccelByteModelsGameRecord getGameRecordResult;
	FRegistry::CloudSave.GetGameRecord(keyGameTest, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &getGameRecordResult](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record1 success"));
		getGameRecordResult = gameRecord;
		bGetGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetGameRecordSuccess, "Waiting for getting game record ...");

	check(bGetGameRecordSuccess);
	check(getGameRecordResult.Key == keyGameTest);
	check(getGameRecordResult.Value.GetNumberField("numRegion") == newRecord1Test.GetNumberField("numRegion"));
	check(getGameRecordResult.Value.GetNumberField("oilsReserve") == newRecord1Test.GetNumberField("oilsReserve"));
	check(getGameRecordResult.Value.GetStringField("islandName") == newRecord1Test.GetStringField("islandName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : newRecord1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == newRecord1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveDeleteGameRecord, "AccelByte.Tests.CloudSave.K.DeleteGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveDeleteGameRecord::RunTest(const FString& Parameters)
{
	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(keyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	check(bDeleteGameRecordSuccess);
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
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	check(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceGameRecordUnexistKey, "AccelByte.Tests.CloudSave.L.ReplaceGameRecordUnexistKey", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceGameRecordUnexistKey::RunTest(const FString& Parameters)
{
	bool bReplaceGameRecordSuccess = false;
	FRegistry::CloudSave.ReplaceGameRecord(unexistKeyGameTest, record1Test, FVoidHandler::CreateLambda([&bReplaceGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Replace game record success"));
		bReplaceGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bReplaceGameRecordSuccess, "Waiting for replacing game record ...");

	bool bGetGameRecordSuccess = false;
	FAccelByteModelsGameRecord getGameRecordResult;
	FRegistry::CloudSave.GetGameRecord(unexistKeyGameTest, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &getGameRecordResult](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record success"));
		getGameRecordResult = gameRecord;
		bGetGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetGameRecordSuccess, "Waiting for getting game record ...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(unexistKeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	check(bReplaceGameRecordSuccess);
	check(bGetGameRecordSuccess);
	check(getGameRecordResult.Key == unexistKeyGameTest);
	check(getGameRecordResult.Value.GetNumberField("numRegion") == record1Test.GetNumberField("numRegion"));
	check(getGameRecordResult.Value.GetNumberField("oilsReserve") == record1Test.GetNumberField("oilsReserve"));
	check(getGameRecordResult.Value.GetStringField("islandName") == record1Test.GetStringField("islandName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	check(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveReplaceWithSaveGameRecord, "AccelByte.Tests.CloudSave.L.ReplaceWithSaveGameRecord", AutomationFlagMaskCloudSave);
bool CloudSaveReplaceWithSaveGameRecord::RunTest(const FString& Parameters)
{
	FString key = "UnexistUnitySDKKeyGameTest";
	bool bSaveGameRecordSuccess = false;
	FRegistry::CloudSave.SaveGameRecord(keyGameTest, record1Test, FVoidHandler::CreateLambda([&bSaveGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Save game record1 success"));
		bSaveGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSaveGameRecordSuccess, "Waiting for saving game record ...");

	bool bGetGameRecordSuccess = false;
	FAccelByteModelsGameRecord getGameRecordResult;
	FRegistry::CloudSave.GetGameRecord(keyGameTest, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &getGameRecordResult](FAccelByteModelsGameRecord gameRecord)
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Get game record success"));
		getGameRecordResult = gameRecord;
		bGetGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetGameRecordSuccess, "Waiting for getting game record ...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(keyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	check(bSaveGameRecordSuccess);
	check(bGetGameRecordSuccess);
	check(getGameRecordResult.Key == keyGameTest);
	check(getGameRecordResult.Value.GetNumberField("numRegion") == record1Test.GetNumberField("numRegion"));
	check(getGameRecordResult.Value.GetNumberField("oilsReserve") == record1Test.GetNumberField("oilsReserve"));
	check(getGameRecordResult.Value.GetStringField("islandName") == record1Test.GetStringField("islandName"));
	for (auto buildingResult : getGameRecordResult.Value.GetArrayField("buildings"))
	{
		bool bItemFound = false;
		for (auto buildingRecord : record1Test.GetArrayField("buildings"))
		{
			if (buildingResult.Get()->AsString() == buildingRecord.Get()->AsString())
			{
				bItemFound = true;
				break;
			}
		}
		check(bItemFound);
	}
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gas") == record1Test.GetObjectField("resources").Get()->GetNumberField("gas"));
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("gold") == record1Test.GetObjectField("resources").Get()->GetNumberField("gold"));
	check(getGameRecordResult.Value.GetObjectField("resources").Get()->GetNumberField("water") == record1Test.GetObjectField("resources").Get()->GetNumberField("water"));
	check(bDeleteGameRecordSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudSaveTearDown, "AccelByte.Tests.CloudSave.Z.TearDown", AutomationFlagMaskCloudSave);
bool CloudSaveTearDown::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecordSuccess = false;
	FRegistry::CloudSave.DeleteUserRecord(keyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	bool bDeleteUserRecord1Success = false;
	FRegistry::CloudSave.DeleteUserRecord(unexistKeyUserTest, FVoidHandler::CreateLambda([&bDeleteUserRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user record success"));
		bDeleteUserRecord1Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteUserRecordSuccess, "Waiting for deleting user record ...");

	bool bDeleteGameRecordSuccess = false;
	FRegistry::CloudSave.DeleteGameRecord(keyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteGameRecord1Success = false;
	FRegistry::CloudSave.DeleteGameRecord(unexistKeyGameTest, FVoidHandler::CreateLambda([&bDeleteGameRecord1Success]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecord1Success = true;
	}), CloudSaveErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteSuccess = false;
	UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteSuccess]()
	{
		UE_LOG(LogAccelByteCloudSaveTest, Log, TEXT("Delete user by id success"));
		bDeleteSuccess = true;
	}), CloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteSuccess, "Waiting for user deletion...");

	check(bDeleteUserRecordSuccess);
	check(bDeleteUserRecord1Success);
	check(bDeleteGameRecordSuccess);
	check(bDeleteGameRecord1Success);
	check(bDeleteSuccess);
	return true;
}
