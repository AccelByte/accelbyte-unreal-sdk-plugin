// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "TestUtilities.h"
#include "Api/AccelByteUserApi.h"
#include "Misc/AutomationTest.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "GameServerApi/AccelByteServerCloudSaveApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerCloudSaveTest, Log, All);

DEFINE_LOG_CATEGORY(LogAccelByteServerCloudSaveTest);

const int32 AutomationFlagMaskServerCloudSave = (EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext
);
void FlushHttpRequests();

const auto ServerCloudSaveErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteServerCloudSaveTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
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
    check(Compare1.GetStringField("map") == Compare2.GetStringField("map"))
    check(Compare1.GetNumberField("maxTeamMember") == Compare2.GetNumberField("maxTeamMember"))
    auto Eq1 = Compare2.GetArrayField("equipment");
	auto Eq2 = Compare2.GetArrayField("equipment");
	for(int i = 0; i < Eq1.Num(); i++)
	{
		bool bIsFound = false;
		for(int j = 0; j < Eq2.Num(); j++)
		{
			if (Eq1[i].Get()->AsString() == Eq2[j].Get()->AsString())
			{
				bIsFound = true;
				break;
			}
		}
		check(bIsFound)
    }
	check(Compare1.GetObjectField("consumable").Get()->GetNumberField("chocolate") == Compare2.GetObjectField("consumable").Get()->GetNumberField("chocolate"))
    check(Compare1.GetObjectField("consumable").Get()->GetNumberField("water") == Compare2.GetObjectField("consumable").Get()->GetNumberField("water"))
    check(Compare1.GetObjectField("consumable").Get()->GetNumberField("apple") == Compare2.GetObjectField("consumable").Get()->GetNumberField("apple"))

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
	
	ServerCloudUser = MakeShared<Api::User>(ServerCloudUserCreds, FRegistry::Settings);
	FString Email = FString::Printf(TEXT("cloudsaveUE4Test@example.com"));
	Email.ToLowerInline();
	FString Password = TEXT("123Password123");
	FString DisplayName = FString::Printf(TEXT("cloudsaveUE4"));
	FString Country = "US";
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
		if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException || (ErrorCodes)Code == ErrorCodes::UserDisplayNameAlreadyUsedException) //email already used
		{
			bUserCreationSuccess = true;
			UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Test ServerCloudSave User2 is already"));
		}
		else
		{
			UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Test ServerCloudSave ServerCloudUser can't be created"));
		}
	}));

	FlushHttpRequests();
	Waiting(bUserCreationSuccess, "Waiting for ServerCloudUser created...");

	bool bUserLoginSuccess = false;
	ServerCloudUser->LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&]()
	{
		bUserLoginSuccess = true;
	}),
		ServerCloudSaveErrorHandler);

	FlushHttpRequests();
	Waiting(bUserLoginSuccess, "Waiting for Login...");

	bool bClientTokenObtained = false;

	UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("    Success"));
        bClientTokenObtained = true;
    }), ServerCloudSaveErrorHandler);

	FlushHttpRequests();
	Waiting(bClientTokenObtained, "Waiting for Login...");

	
	/*
	bool bDeleteGameRecordSuccess = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKey, FVoidHandler::CreateLambda([&bDeleteGameRecordSuccess]()
    {
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete game record success"));
        bDeleteGameRecordSuccess = true;
    }), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteGameRecordUnExistSuccess = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKeyUnExist, FVoidHandler::CreateLambda([&bDeleteGameRecordUnExistSuccess]()
    {
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete game record success"));
        bDeleteGameRecordUnExistSuccess = true;
    }), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteGameRecordUnExistSuccess, "Waiting for deleting game record ...");
	*/

	check(bUserLoginSuccess)
	check(bClientTokenObtained)

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
	FlushHttpRequests();
	Waiting(bDeleteGameRecordSuccess, "Waiting for deleting game record ...");

	bool bDeleteGameRecordUnExistSuccess = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(ServerCloudGameRecordKeyUnExist, FVoidHandler::CreateLambda([&bDeleteGameRecordUnExistSuccess]()
    {
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete game record success"));
        bDeleteGameRecordUnExistSuccess = true;
    }), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteGameRecordUnExistSuccess, "Waiting for deleting game record ...");

	bool bDeleteSuccess = false;
	UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(ServerCloudUserCreds.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteSuccess]()
	{
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete user by id success"));
		bDeleteSuccess = true;
	}), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteSuccess, "Waiting for user deletion...");

	check(bDeleteGameRecordSuccess);
	check(bDeleteGameRecordUnExistSuccess);
	check(bDeleteSuccess);
	
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
	FlushHttpRequests();
	Waiting(SaveGameRecord, "Waiting for saving game record UnExist key ...");

	check(SaveGameRecord)

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
		
		UE_LOG(LogAccelByteServerCloudSaveTest, Fatal, TEXT("Failed to GET Game ServerCloudRecord. Error code: %d\nError message:%s"), Code, *Message);
	}
	));

	FlushHttpRequests();
	Waiting(bGetGameRecordDone, "Waiting for Get Game ServerCloudRecord ...");

	check(bGetGameRecordSuccess)

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
    }), FErrorHandler::CreateLambda(
    [&bGetGameRecordSuccess, &bGetGameRecordDone](int32 Code, FString Message)
    {
        bGetGameRecordDone = true;
    	bGetGameRecordSuccess = false;
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Failed to GET Game ServerCloudRecord. Error code: %d\nError message:%s"), Code, *Message);
    }
    ));

	FlushHttpRequests();
	Waiting(bGetGameRecordDone, "Waiting for Get Game ServerCloudRecord with UnExist Key ...");

	check(!bGetGameRecordSuccess)

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
	FlushHttpRequests();
	Waiting(bUpdateGameRecord, "Waiting for Updating ServerCloudRecord value ... ");

	check(bUpdateGameRecord)
	
	bool bGetGameRecord = false;
	FAccelByteModelsGameRecord Compare;
	FRegistry::ServerCloudSave.GetGameRecord(ServerCloudGameRecordKey, THandler<FAccelByteModelsGameRecord>::CreateLambda([&Compare, &bGetGameRecord](FAccelByteModelsGameRecord Result)
	{
		Compare = Result;
		bGetGameRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success GetGameRecord to compare with ServerCloudRecord2"))
	}), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bGetGameRecord, "Waiting for GetGameRecord ...");

	check(bGetGameRecord)
	check(Compare.Key == ServerCloudGameRecordKey)
	
	return ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord2);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestReplaceGameRecordUnexistKey, "AccelByte.Tests.ServerCloudSave.D.ReplaceGameRecordUnexistKey", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestReplaceGameRecordUnexistKey::RunTest(const FString& Parameters)
{
	bool bReplaceGameRecordUnexistKey = false;
	const FString UnexistKey = "UE4UnexistKeyGameRecordTest";
	FRegistry::ServerCloudSave.ReplaceGameRecord(UnexistKey, ServerCloudRecord, FVoidHandler::CreateLambda([&bReplaceGameRecordUnexistKey]()
	{
		bReplaceGameRecordUnexistKey = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success replacing unexist key Game ServerCloudRecord"))
	}), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bReplaceGameRecordUnexistKey, "Waiting replace game record on unexist key ...");

	check(bReplaceGameRecordUnexistKey)
	
	bool bGetGameRecordUnexistKey = false;
	FAccelByteModelsGameRecord Compare;
	FRegistry::ServerCloudSave.GetGameRecord(UnexistKey, THandler<FAccelByteModelsGameRecord>::CreateLambda([&bGetGameRecordUnexistKey, &Compare](FAccelByteModelsGameRecord Result)
	{
		bGetGameRecordUnexistKey = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success getting Game ServerCloudRecord using unexist key (submited using replace game record)"))
	}), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bGetGameRecordUnexistKey, "Waiting get game record on unexist key ...");

	check(bGetGameRecordUnexistKey)
	
	check(Compare.Key == UnexistKey)
	ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord);

	bool bDeleteGameRecordUnexistKey = false;
	FRegistry::ServerCloudSave.DeleteGameRecord(UnexistKey, FVoidHandler::CreateLambda([&bDeleteGameRecordUnexistKey]()
	{
		bDeleteGameRecordUnexistKey = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Success deleting game record"))
	}), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteGameRecordUnexistKey, "Waiting to delete game record on unexist key ...");

	check(bDeleteGameRecordUnexistKey)
	
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
	FlushHttpRequests();
	Waiting(bDeleteGameRecord, "Waiting for Deleting ServerCloudRecord Value ... ");

	check(bDeleteGameRecord)
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
	FlushHttpRequests();
	Waiting(bDeleteGameRecord, "Waiting Delete game record ...");

	check(bDeleteGameRecord)
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
	FlushHttpRequests();
	Waiting(bSavePlayerRecord, "Waiting saving user record ...");

	check(bSavePlayerRecord)

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
	FlushHttpRequests();
	Waiting(bSavePlayerRecord, "Waiting saving user record ...");

	check(bSavePlayerRecord)

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
	FlushHttpRequests();
	Waiting(bGetPlayerRecord, "Waiting getting the user record ...");

	check(bGetPlayerRecord)
	check(Compare.Key == ServerCloudPlayerRecordKey)
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
	FlushHttpRequests();
	Waiting(bGetPlayerRecord, "Waiting getting the user record ...");

	check(bGetPlayerRecord)
    check(Compare.Key == ServerCloudPlayerRecordKeyPublic)
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
	FlushHttpRequests();
	Waiting(bGetRecordDone, "Waiting getting user record unexist key ...");

	check(!bGetRecordUnexistKey)

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
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to replace user record");

	check(bReplaceUserRecord)

	bool bGetReplacedUserRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetUserRecord(ServerCloudPlayerRecordKey, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetReplacedUserRecord, &Compare](FAccelByteModelsUserRecord Result)
	{
		bGetReplacedUserRecord = true;
		Compare = Result;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Successfuly get replaced user record"))
	}), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to get user record");

	check(bGetReplacedUserRecord)
	check(Compare.Key == ServerCloudPlayerRecordKey)

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
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to replace user record");

	check(bReplaceUserRecord)

    bool bGetReplacedUserRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetPublicUserRecord(ServerCloudPlayerRecordKeyPublic, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetReplacedUserRecord, &Compare](FAccelByteModelsUserRecord Result)
    {
        bGetReplacedUserRecord = true;
        Compare = Result;
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Successfuly get replaced user record"))
    }), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to get user record");

	check(bGetReplacedUserRecord)
    check(Compare.Key == ServerCloudPlayerRecordKeyPublic)

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
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to replace user record");

	check(bReplaceUserRecord)

    bool bGetReplacedUserRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetReplacedUserRecord, &Compare](FAccelByteModelsUserRecord Result)
    {
        bGetReplacedUserRecord = true;
        Compare = Result;
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Successfuly get replaced user record"))
    }), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to get user record");

	check(bGetReplacedUserRecord)
    check(Compare.Key == UnexistKey)

    ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord2);

	bool bDeleteUserRecord = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), false, FVoidHandler::CreateLambda([&bDeleteUserRecord]()
	{
		bDeleteUserRecord = true;
		UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Succefully delete user record"))
	}), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to delete user record");

	check(bDeleteUserRecord)

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
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to replace user record");

	check(bReplaceUserRecord)

    bool bGetReplacedUserRecord = false;
	FAccelByteModelsUserRecord Compare;
	FRegistry::ServerCloudSave.GetPublicUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), THandler<FAccelByteModelsUserRecord>::CreateLambda([&bGetReplacedUserRecord, &Compare](FAccelByteModelsUserRecord Result)
    {
        bGetReplacedUserRecord = true;
        Compare = Result;
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Successfuly get replaced user record"))
    }), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to get user record");

	check(bGetReplacedUserRecord)
    check(Compare.Key == UnexistKey)

    ServerCloudSaveCompareJsonObject(Compare.Value, ServerCloudRecord2);

	bool bDeleteUserRecord = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(UnexistKey, ServerCloudUserCreds.GetUserId(), true, FVoidHandler::CreateLambda([&bDeleteUserRecord]()
    {
        bDeleteUserRecord = true;
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Succefully delete user record"))
    }), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bReplaceUserRecord, "Waiting to delete user record");

	check(bDeleteUserRecord)

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
	FlushHttpRequests();
	Waiting(bDeleteUserRecord, "Waiting deleting user record");

	check(bDeleteUserRecord)

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
	FlushHttpRequests();
	Waiting(bDeleteUserRecord, "Waiting deleting user record");

	check(bDeleteUserRecord)

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
	FlushHttpRequests();
	Waiting(bDeleteUserRecord, "Waiting deleting user record");

	check(bDeleteUserRecord)

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerCloudTestDeletePlayerRecordUnExistPublic, "AccelByte.Tests.ServerCloudSave.H.DeleteUserRecordUnExistPublic", AutomationFlagMaskServerCloudSave)
bool ServerCloudTestDeletePlayerRecordUnExistPublic::RunTest(const FString& Parameters)
{
	bool bDeleteUserRecord = false;
	FRegistry::ServerCloudSave.DeleteUserRecord(ServerCloudPlayerRecordKeyUnExist, ServerCloudUserCreds.GetUserId(), true, FVoidHandler::CreateLambda([&bDeleteUserRecord]()
    {
        bDeleteUserRecord = true;
        UE_LOG(LogAccelByteServerCloudSaveTest, Log, TEXT("Delete user record"))
    }), ServerCloudSaveErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteUserRecord, "Waiting deleting user record");

	check(bDeleteUserRecord)

    return true;
}