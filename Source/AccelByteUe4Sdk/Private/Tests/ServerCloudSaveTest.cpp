// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteUserApi.h"
#include "Misc/AutomationTest.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "GameServerApi/AccelByteServerCloudSaveApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerCloudSaveTest, Log, All);

DEFINE_LOG_CATEGORY(LogAccelByteServerCloudSaveTest);

const int32 AutomationFlagMaskServerCloudSave = (EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext
);

const auto ServerCloudSaveErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteServerCloudSaveTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

Credentials ServerCloudUserCreds;
TSharedPtr<Api::User> ServerCloudUser;
FString ServerCloudGameRecordKey = "UE4SDKGameRecordTest";
FString ServerCloudGameRecordKeyUnExist = "UE4SDKGameRecordTestUnExist";
FString ServerCloudPlayerRecordKey = "UE4SDKPlayerRecordTest";
FString ServerCloudPlayerRecordKeyPublic = "UE4SDKPlayerRecordTestPublic";
FString ServerCloudPlayerRecordKeyUnExist = "UE4SDKPlayerRecordTestUnExist";
FJsonObject ServerCloudRecord;
FJsonObject ServerCloudRecord2;

bool ServerCloudSaveCompareJsonObject(const FJsonObject& Compare1, const FJsonObject& Compare2)
{
	if (Compare1.GetStringField("map") != Compare2.GetStringField("map")) { return false; }
	if (Compare1.GetNumberField("maxTeamMember") != Compare2.GetNumberField("maxTeamMember")) { return false; }
	auto Eq1 = Compare2.GetArrayField("equipment");
	auto Eq2 = Compare2.GetArrayField("equipment");
	for (int i = 0; i < Eq1.Num(); i++)
	{
		bool bIsFound = false;
		for (int j = 0; j < Eq2.Num(); j++)
		{
			if (Eq1[i].Get()->AsString() == Eq2[j].Get()->AsString())
			{
				bIsFound = true;
				break;
			}
		}

		if (!bIsFound) { return false; }
	}

	if (Compare1.GetObjectField("consumable").Get()->GetNumberField("chocolate") != Compare2.GetObjectField("consumable").Get()->GetNumberField("chocolate")) { return false; }
	if (Compare1.GetObjectField("consumable").Get()->GetNumberField("water") != Compare2.GetObjectField("consumable").Get()->GetNumberField("water")) { return false; }
	if (Compare1.GetObjectField("consumable").Get()->GetNumberField("apple") != Compare2.GetObjectField("consumable").Get()->GetNumberField("apple")) { return false; }

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudSaveSetup, "AccelByte.Tests.ServerCloudSave.A.Setup", AutomationFlagMaskServerCloudSave);
bool ServerCloudSaveSetup::RunTest(const FString& Parameters)
{
	ServerCloudRecord.SetStringField("map", "Oslo");
	ServerCloudRecord.SetNumberField("maxTeamMember", 1);
	const TArray<TSharedPtr<FJsonValue>> EquipmentDrops{
		MakeShareable(new FJsonValueString("shield")),
		MakeShareable(new FJsonValueString("gun")),
		MakeShareable(new FJsonValueString("excalibur")),
		MakeShareable(new FJsonValueString("bow"))
	};
	ServerCloudRecord.SetArrayField("equipment", EquipmentDrops);
	TSharedPtr<FJsonObject> ConsumableDrops = MakeShareable(new FJsonObject);
	ConsumableDrops->SetNumberField("chocolate", 10);
	ConsumableDrops->SetNumberField("water", 200);
	ConsumableDrops->SetNumberField("apple", 30);
	ServerCloudRecord.SetObjectField("consumable", ConsumableDrops);

	ServerCloudRecord2.SetStringField("map", "Denver");
	ServerCloudRecord2.SetNumberField("maxTeamMember", 2);
	const TArray<TSharedPtr<FJsonValue>> EquipmentDrops2{
		MakeShareable(new FJsonValueString("shotgun")),
		MakeShareable(new FJsonValueString("rifle")),
		MakeShareable(new FJsonValueString("pistol")),
		MakeShareable(new FJsonValueString("sniper"))
	};
	ServerCloudRecord2.SetArrayField("equipment", EquipmentDrops2);
	TSharedPtr<FJsonObject> ConsumableDrops2 = MakeShareable(new FJsonObject);
	ConsumableDrops2->SetNumberField("chocolate", 3);
	ConsumableDrops2->SetNumberField("water", 600);
	ConsumableDrops2->SetNumberField("apple", 42);
	ServerCloudRecord2.SetObjectField("consumable", ConsumableDrops2);

	FRegistry::ServerOauth2.ForgetAllCredentials();

	ServerCloudUser = MakeShared<Api::User>(ServerCloudUserCreds, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	FString Email = FString::Printf(TEXT("cloudsaveUE4Test@example.com"));
	Email.ToLowerInline();
	FString const Password = TEXT("123Password123");
	FString const DisplayName = FString::Printf(TEXT("cloudsaveUE4"));
	FString const Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bUserCreationSuccess = false;
	ServerCloudUser->Register(
		Email,
		Password,
		DisplayName,
		Country,
		format,
		THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
	{
		bUserCreationSuccess = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Test ServerCloudSave User2 is Created"));

	}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Code=%d"), Code);
		if (static_cast<ErrorCodes>(Code) == ErrorCodes::UserEmailAlreadyUsedException || static_cast<ErrorCodes>(Code) == ErrorCodes::UserDisplayNameAlreadyUsedException) //email already used
		{
			bUserCreationSuccess = true;
			UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Test ServerCloudSave User2 is already"));
		}
		else
		{
			UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Test ServerCloudSave ServerCloudUser can't be created"));
		}
	}));

	WaitUntil(bUserCreationSuccess, "Waiting for ServerCloudUser created...");

	bool bUserLoginSuccess = false;
	ServerCloudUser->LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&]()
	{
		bUserLoginSuccess = true;
	}),
		ServerCloudSaveErrorHandler);

	WaitUntil(bUserLoginSuccess, "Waiting for Login...");

	bool bClientTokenObtained = false;

	UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), ServerCloudSaveErrorHandler);

	WaitUntil(bClientTokenObtained, "Waiting for Login...");

	/*
	bool bDeleteGameRecordSuccess = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKey, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), ServerCloudSaveErrorHandler);
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteGameRecordUnExistSuccess = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKeyUnExist, FVoidHandler::CreateLambda([&bDeleteGameRecordUnExistSuccess]()
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordUnExistSuccess = true;
	}), ServerCloudSaveErrorHandler);
	Waiting(bDeleteGameRecordUnExistSuccess, "Waiting for deleting game record ...");
	*/

	AB_TEST_TRUE(bUserLoginSuccess);
	AB_TEST_TRUE(bClientTokenObtained);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudSaveTearDown, "AccelByte.Tests.ServerCloudSave.Z.TearDown", AutomationFlagMaskServerCloudSave);
bool ServerCloudSaveTearDown::RunTest(const FString& Parameters)
{
	bool bDeleteGameRecordSuccess = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKey, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordSuccess = true;
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteGameRecordUnExistSuccess = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKeyUnExist, FVoidHandler::CreateLambda([&bDeleteGameRecordUnExistSuccess]()
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete game record success"));
		bDeleteGameRecordUnExistSuccess = true;
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteGameRecordUnExistSuccess, "Waiting for deleting game record ...");

	bool bDeleteSuccess = false;
	UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(ServerCloudUserCreds.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteSuccess]()
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete user by id success"));
		bDeleteSuccess = true;
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteSuccess, "Waiting for user deletion...");

	AB_TEST_TRUE(bDeleteGameRecordSuccess);
	AB_TEST_TRUE(bDeleteGameRecordUnExistSuccess);
	AB_TEST_TRUE(bDeleteSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudSaveTestAdminGameRecord, "AccelByte.Tests.ServerCloudSave.B.SaveGameRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudSaveTestAdminGameRecord::RunTest(const FString& Parameters)
{
	bool SaveGameRecord = false;
	FRegistry::ServerCloudSave.SaveGameRecord(ServerCloudGameRecordKey, ServerCloudRecord, FVoidHandler::CreateLambda([&SaveGameRecord]()
	{
		SaveGameRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Save Game ServerCloudRecord success"));
	}), ServerCloudSaveErrorHandler);
	WaitUntil(SaveGameRecord, "Waiting for saving game record UnExist key ...");

	AB_TEST_TRUE(SaveGameRecord)

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestGetAdminGameRecord, "AccelByte.Tests.ServerCloudSave.C.GetGameRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestGetAdminGameRecord::RunTest(const FString& Parameters)
{
	bool bGetGameRecordSuccess = false;
	bool bGetGameRecordDone = false;
	FRegistry::ServerCloudSave.GetGameRecord(ServerCloudGameRecordKey, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordSuccess, &bGetGameRecordDone](FAccelByteModelsGameRecord Rec)
	{
		bGetGameRecordSuccess = true;
		bGetGameRecordDone = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("GET Game ServerCloudRecord success"));
	}), FErrorHandler::CreateLambda(
	[&bGetGameRecordDone](int32 Code, FString Message)
	{
		bGetGameRecordDone = true;

		UE_LOG(LogAccelByteServerCloudSaveTest, Error, TEXT("Failed to GET Game ServerCloudRecord. Error code: %d\nError message:%s"), Code, *Message);
	}
	));

	WaitUntil(bGetGameRecordDone, "Waiting for Get Game ServerCloudRecord ...");

	AB_TEST_TRUE(bGetGameRecordSuccess)

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestGetAdminGameRecordUnExistKey, "AccelByte.Tests.ServerCloudSave.C.GetGameRecordUnExistKey", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestGetAdminGameRecordUnExistKey::RunTest(const FString& Parameters)
{
	bool bGetGameRecordSuccess = true;
	bool bGetGameRecordDone = false;
	FRegistry::ServerCloudSave.GetGameRecord(ServerCloudGameRecordKeyUnExist, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordDone](FAccelByteModelsGameRecord Rec)
	{
		bGetGameRecordDone = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("GET Game ServerCloudRecord success"));
	}), FErrorHandler::CreateLambda([&bGetGameRecordSuccess, &bGetGameRecordDone](int32 Code, FString Message)
	{
		bGetGameRecordDone = true;
		bGetGameRecordSuccess = false;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Failed to GET Game ServerCloudRecord. Error code: %d\nError message:%s"), Code, *Message);
	}));

	WaitUntil(bGetGameRecordDone, "Waiting for Get Game ServerCloudRecord with UnExist Key ...");

	AB_TEST_FALSE(bGetGameRecordSuccess)

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestReplaceAdminGameRecord, "AccelByte.Tests.ServerCloudSave.D.ReplaceGameRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestReplaceAdminGameRecord::RunTest(const FString& Parameters)
{
	bool bUpdateGameRecord = false;
	FRegistry::ServerCloudSave.ReplaceGameRecord(ServerCloudGameRecordKey, ServerCloudRecord2, FVoidHandler::CreateLambda([&bUpdateGameRecord]()
	{
		bUpdateGameRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success Update Game ServerCloudRecord"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bUpdateGameRecord, "Waiting for Updating ServerCloudRecord value ... ");

	AB_TEST_TRUE(bUpdateGameRecord)

	bool bGetGameRecord = false;
	FAccelByteModelsGameRecord Compare;
	FRegistry::ServerCloudSave.GetGameRecord(ServerCloudGameRecordKey, THandler<FAccelByteModelsGameRecord>::CreateLambda([&Compare, &bGetGameRecord](FAccelByteModelsGameRecord Result)
	{
		Compare = Result;
		bGetGameRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success GetGameRecord to compare with ServerCloudRecord2"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bGetGameRecord, "Waiting for GetGameRecord ...");

	AB_TEST_TRUE(bGetGameRecord)
	AB_TEST_EQUAL(Compare.Key, ServerCloudGameRecordKey);

	return ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord2);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestReplaceGameRecordUnexistKey, "AccelByte.Tests.ServerCloudSave.D.ReplaceGameRecordUnexistKey", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestReplaceGameRecordUnexistKey::RunTest(const FString& Parameters)
{
	bool bReplaceGameRecordUnexistKey = false;
	FString const UnexistKey = "UE4UnexistKeyGameRecordTest";
	FRegistry::ServerCloudSave.ReplaceGameRecord(UnexistKey, ServerCloudRecord, FVoidHandler::CreateLambda([&bReplaceGameRecordUnexistKey]()
	{
		bReplaceGameRecordUnexistKey = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success replacing unexist key Game ServerCloudRecord"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bReplaceGameRecordUnexistKey, "Waiting replace game record on unexist key ...");

	AB_TEST_TRUE(bReplaceGameRecordUnexistKey)

	bool bGetGameRecordUnexistKey = false;
	FAccelByteModelsGameRecord Compare;
	FRegistry::ServerCloudSave.GetGameRecord(UnexistKey, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordUnexistKey, &Compare](FAccelByteModelsGameRecord Result)
	{
		bGetGameRecordUnexistKey = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success getting Game ServerCloudRecord using unexist key (submited using replace game record)"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bGetGameRecordUnexistKey, "Waiting get game record on unexist key ...");

	AB_TEST_TRUE(bGetGameRecordUnexistKey)

	AB_TEST_EQUAL(Compare.Key, UnexistKey);
	ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord);

	bool bDeleteGameRecordUnexistKey = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(UnexistKey, FVoidHandler::CreateLambda([&bDeleteGameRecordUnexistKey]()
	{
		bDeleteGameRecordUnexistKey = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success deleting game record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteGameRecordUnexistKey, "Waiting to delete game record on unexist key ...");

	AB_TEST_TRUE(bDeleteGameRecordUnexistKey)

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestDeleteAdminGameRecord, "AccelByte.Tests.ServerCloudSave.E.DeleteGameRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestDeleteAdminGameRecord::RunTest(const FString& Parameters)
{
	bool bDeleteGameRecord = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKey, FVoidHandler::CreateLambda([&bDeleteGameRecord]()
	{
		bDeleteGameRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success Delete Game ServerCloudRecord"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteGameRecord, "Waiting for Deleting ServerCloudRecord Value ... ");

	AB_TEST_TRUE(bDeleteGameRecord)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestDeleteGameRecordUnExistKey, "AccelByte.Tests.ServerCloudSave.E.DeleteGameRecordUnExistKey", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestDeleteGameRecordUnExistKey::RunTest(const FString& Parameters)
{
	bool bDeleteGameRecord = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKeyUnExist, FVoidHandler::CreateLambda([&bDeleteGameRecord]()
	{
		bDeleteGameRecord = true;
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteGameRecord, "Waiting Delete game record ...");

	AB_TEST_TRUE(bDeleteGameRecord)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestSavePlayerRecord, "AccelByte.Tests.ServerCloudSave.F.SavePlayerRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestSavePlayerRecord::RunTest(const FString& Parameters)
{
	bool bSavePlayerRecord = false;
	FRegistry::ServerCloudSave.SaveUserRecord(ServerCloudPlayerRecordKey, ServerCloudUserCreds.GetUserId(), ServerCloudRecord, false, FVoidHandler::CreateLambda([&bSavePlayerRecord]()
	{
		bSavePlayerRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success Saving new user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bSavePlayerRecord, "Waiting saving user record ...");

	AB_TEST_TRUE(bSavePlayerRecord)

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestSavePlayerRecordPublic, "AccelByte.Tests.ServerCloudSave.F.SavePlayerRecordPublic", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestSavePlayerRecordPublic::RunTest(const FString& Parameters)
{
	bool bSavePlayerRecord = false;
	FRegistry::ServerCloudSave.SaveUserRecord(ServerCloudPlayerRecordKeyPublic, ServerCloudUserCreds.GetUserId(), ServerCloudRecord, true, FVoidHandler::CreateLambda([&bSavePlayerRecord]()
	{
		bSavePlayerRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success Saving new user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bSavePlayerRecord, "Waiting saving user record ...");

	AB_TEST_TRUE(bSavePlayerRecord)

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestGetPlayerRecord, "AccelByte.Tests.ServerCloudSave.G.GetPlayerRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestGetPlayerRecord::RunTest(const FString& Parameters)
{
	bool bGetPlayerRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetUserRecord(ServerCloudPlayerRecordKey, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetPlayerRecord, &Compare](FAccelByteModelsUserRecord Result)
	{
		bGetPlayerRecord = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success get user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bGetPlayerRecord, "Waiting getting the user record ...");

	AB_TEST_TRUE(bGetPlayerRecord)
	AB_TEST_EQUAL(Compare.Key, ServerCloudPlayerRecordKey);
	return ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestGetPlayerRecordPublic, "AccelByte.Tests.ServerCloudSave.G.GetPlayerRecordPublic", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestGetPlayerRecordPublic::RunTest(const FString& Parameters)
{
	bool bGetPlayerRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetPublicUserRecord(ServerCloudPlayerRecordKeyPublic, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetPlayerRecord, &Compare](FAccelByteModelsUserRecord Result)
	{
		bGetPlayerRecord = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success get user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bGetPlayerRecord, "Waiting getting the user record ...");

	AB_TEST_TRUE(bGetPlayerRecord)
	AB_TEST_EQUAL(Compare.Key, ServerCloudPlayerRecordKeyPublic);
	return ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestGetPlayerRecordUnexistKey, "AccelByte.Tests.ServerCloudSave.G.GetPlayerRecordUnexistKey", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestGetPlayerRecordUnexistKey::RunTest(const FString& Parameters)
{
	bool bGetRecordUnexistKey = true;
	bool bGetRecordDone = false;
	FRegistry::ServerCloudSave.GetUserRecord(ServerCloudPlayerRecordKeyUnExist, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetRecordDone](FAccelByteModelsUserRecord Result)
	{
		bGetRecordDone = true;
	}), FErrorHandler::CreateLambda([&bGetRecordUnexistKey, &bGetRecordDone](int32 Code, const FString& Message)
	{
		bGetRecordDone = true;
		bGetRecordUnexistKey = false;

		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Failed get user record (expected behaviour)"))
	}));
	WaitUntil(bGetRecordDone, "Waiting getting user record unexist key ...");

	AB_TEST_FALSE(bGetRecordUnexistKey)

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestReplaceExistingPlayerRecord, "AccelByte.Tests.ServerCloudSave.H.ReplaceExistingUserRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestReplaceExistingPlayerRecord::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecord = false;
	FRegistry::ServerCloudSave.ReplaceUserRecord(ServerCloudPlayerRecordKey, ServerCloudUserCreds.GetUserId(), ServerCloudRecord2, false, FVoidHandler::CreateLambda([&bReplaceUserRecord]()
	{
		bReplaceUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Replace user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bReplaceUserRecord, "Waiting to replace user record");

	AB_TEST_TRUE(bReplaceUserRecord)

	bool bGetReplacedUserRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetUserRecord(ServerCloudPlayerRecordKey, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetReplacedUserRecord, &Compare](FAccelByteModelsUserRecord Result)
	{
		bGetReplacedUserRecord = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Successfuly get replaced user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bGetReplacedUserRecord, "Waiting to get user record");

	AB_TEST_TRUE(bGetReplacedUserRecord)
	AB_TEST_EQUAL(Compare.Key, ServerCloudPlayerRecordKey);

	return ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord2);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestReplaceExistingPlayerRecordPublic, "AccelByte.Tests.ServerCloudSave.H.ReplaceExistingUserRecordPublic", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestReplaceExistingPlayerRecordPublic::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecord = false;
	FRegistry::ServerCloudSave.ReplaceUserRecord(ServerCloudPlayerRecordKeyPublic, ServerCloudUserCreds.GetUserId(), ServerCloudRecord2, true, FVoidHandler::CreateLambda([&bReplaceUserRecord]()
	{
		bReplaceUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Replace user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bReplaceUserRecord, "Waiting to replace user record");

	AB_TEST_TRUE(bReplaceUserRecord)

	bool bGetReplacedUserRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetPublicUserRecord(ServerCloudPlayerRecordKeyPublic, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetReplacedUserRecord, &Compare](FAccelByteModelsUserRecord Result)
	{
		bGetReplacedUserRecord = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Successfuly get replaced user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bGetReplacedUserRecord, "Waiting to get user record");

	AB_TEST_TRUE(bGetReplacedUserRecord)
	AB_TEST_EQUAL(Compare.Key, ServerCloudPlayerRecordKeyPublic);

	return ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord2);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestReplaceUnExistingPlayerRecord, "AccelByte.Tests.ServerCloudSave.H.ReplaceUnExistingUserRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestReplaceUnExistingPlayerRecord::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecord = false;
	const FString UnexistKey = "UE4UnexistKeyPlayerRecord";
	FRegistry::ServerCloudSave.ReplaceUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), ServerCloudRecord2, false, FVoidHandler::CreateLambda([&bReplaceUserRecord]()
	{
		bReplaceUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Replace user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bReplaceUserRecord, "Waiting to replace user record");

	AB_TEST_TRUE(bReplaceUserRecord)

	bool bGetReplacedUserRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetReplacedUserRecord, &Compare](FAccelByteModelsUserRecord Result)
	{
		bGetReplacedUserRecord = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Successfuly get replaced user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bGetReplacedUserRecord, "Waiting to get user record");

	AB_TEST_TRUE(bGetReplacedUserRecord)
	AB_TEST_EQUAL(Compare.Key, UnexistKey);

	ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord2);

	bool bDeleteUserRecord = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), false, FVoidHandler::CreateLambda([&bDeleteUserRecord]()
	{
		bDeleteUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Succefully delete user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteUserRecord, "Waiting to delete user record");

	AB_TEST_TRUE(bDeleteUserRecord)

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestReplaceUnExistingPlayerRecordPublic, "AccelByte.Tests.ServerCloudSave.H.ReplaceUnExistingUserRecordPublic", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestReplaceUnExistingPlayerRecordPublic::RunTest(const FString& Parameters)
{
	bool bReplaceUserRecord = false;
	const FString UnexistKey = "UE4UnexistKeyPlayerRecord2";
	FRegistry::ServerCloudSave.ReplaceUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), ServerCloudRecord2, true, FVoidHandler::CreateLambda([&bReplaceUserRecord]()
	{
		bReplaceUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Replace user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bReplaceUserRecord, "Waiting to replace user record");

	AB_TEST_TRUE(bReplaceUserRecord)

	bool bGetReplacedUserRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetPublicUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetReplacedUserRecord, &Compare](FAccelByteModelsUserRecord Result)
	{
		bGetReplacedUserRecord = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Successfuly get replaced user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bGetReplacedUserRecord, "Waiting to get user record");

	AB_TEST_TRUE(bGetReplacedUserRecord)
	AB_TEST_EQUAL(Compare.Key, UnexistKey);

	ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord2);

	bool bDeleteUserRecord = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), true, FVoidHandler::CreateLambda([&bDeleteUserRecord]()
	{
		bDeleteUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Succefully delete user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteUserRecord, "Waiting to delete user record");

	AB_TEST_TRUE(bDeleteUserRecord);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestDeletePlayerRecord, "AccelByte.Tests.ServerCloudSave.I.DeleteUserRecord", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestDeletePlayerRecord::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecord = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(ServerCloudPlayerRecordKey, ServerCloudUserCreds.GetUserId(), false, FVoidHandler::CreateLambda([&bDeleteUserRecord]()
	{
		bDeleteUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteUserRecord, "Waiting deleting user record");

	AB_TEST_TRUE(bDeleteUserRecord);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestDeletePlayerRecordPublic, "AccelByte.Tests.ServerCloudSave.H.DeleteUserRecordPublic", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestDeletePlayerRecordPublic::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecord = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(ServerCloudPlayerRecordKeyPublic, ServerCloudUserCreds.GetUserId(), true, FVoidHandler::CreateLambda([&bDeleteUserRecord]()
	{
		bDeleteUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete user record"))
	}), ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteUserRecord, "Waiting deleting user record");

	AB_TEST_TRUE(bDeleteUserRecord);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestDeletePlayerRecordUnExist, "AccelByte.Tests.ServerCloudSave.H.DeleteUserRecordUnExist", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestDeletePlayerRecordUnExist::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecord = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(ServerCloudPlayerRecordKeyUnExist, ServerCloudUserCreds.GetUserId(), false, FVoidHandler::CreateLambda([&bDeleteUserRecord]()
	{
		bDeleteUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete user record"))
	}),ServerCloudSaveErrorHandler);
	WaitUntil(bDeleteUserRecord, "Waiting deleting user record");

	AB_TEST_TRUE(bDeleteUserRecord);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestDeletePlayerRecordUnExistPublic, "AccelByte.Tests.ServerCloudSave.H.DeleteUserRecordUnExistPublic", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestDeletePlayerRecordUnExistPublic::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecordDone = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(ServerCloudPlayerRecordKeyUnExist, ServerCloudUserCreds.GetUserId(), true, FVoidHandler::CreateLambda([&bDeleteUserRecordDone]()
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete user record"))
		bDeleteUserRecordDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteUserRecordDone](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bDeleteUserRecordDone = true;
	}));
	WaitUntil(bDeleteUserRecordDone, "Waiting deleting user record");

	AB_TEST_TRUE(bDeleteUserRecordDone);

	return true;
}
