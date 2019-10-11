// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteStatisticApi.h"
#include "Api/AccelByteGameProfileApi.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteStatisticTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteStatisticTest);

const int32 AutomationFlagMaskStatistic = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto StatisticTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

FAccelByteModelsGameProfile Profile;

FAccelByteModelsGameProfileRequest CreateGameProfileRequest(int32 AttributesLength, FString AttributeKeyPrefix, FString AttributeValuePrefix, int32 TagsLength, FString TagPrefix, FString AvatarUrl = "http://example.com/", FString Label = "DefaultLabel", FString ProfileName = "DefaultName")
{
	TMap<FString, FString> Attributes;
	for (int i = 0; i < AttributesLength; i++)
	{
		Attributes.Add(FString::Printf(TEXT("%s%d"), *AttributeKeyPrefix, i), FString::Printf(TEXT("%s%d"), *AttributeValuePrefix, i));
	}
	TArray<FString> Tags;
	for (int i = 0; i < TagsLength; i++)
	{
		Tags.Add(FString::Printf(TEXT("%s%d"), *TagPrefix, i));
	}

	FAccelByteModelsGameProfileRequest Request{ Attributes, AvatarUrl, Label, ProfileName, Tags };
	return Request;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticSetup, "AccelByte.Tests.Statistic.A.Setup", AutomationFlagMaskStatistic);
bool StatisticSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess = false;
	bool UsersLoginSuccess = false;

	FRegistry::User.LoginWithClientCredentials(FSimpleDelegate::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Client Login Success"));
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bClientLoginSuccess,"Waiting for Login...");


	FString Email = FString::Printf(TEXT("Statistic_UE4Test@example.com"));
	Email.ToLowerInline();
	FString Password = FString::Printf(TEXT("123Password123"), 0);
	FString DisplayName = FString::Printf(TEXT("StatisticUE4Test"), 0);
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	FRegistry::User.Register(Email, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Response)
	{
		UsersCreationSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test GameProfile User is successfuly created."));
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if (Code == EHttpResponseCodes::Conflict)
		{
			UsersCreationSuccess = true;
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test GameProfile User is already created."));
		}
		else
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test GameProfile User can't be created"));
		}
	}));
	FlushHttpRequests();
	Waiting(UsersCreationSuccess,"Waiting for user created...");

	FRegistry::User.LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&]()
	{
		UsersLoginSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t\tSuccessfully Login."));
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(UsersLoginSuccess,"Waiting for Login...");
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	TArray<FAccelByteModelsGameProfile> GetAllGameProfileResult;
	bool bGetAllGameProfileSuccess = false;
	FRegistry::GameProfile.GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsGameProfile>& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t\tsuccess"));
		GetAllGameProfileResult = Result;
		bGetAllGameProfileSuccess = true;
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetAllGameProfileSuccess,"Waiting for get all game profiles...");
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t%d game profile is found!"), GetAllGameProfileResult.Num());
	check(bGetAllGameProfileSuccess);
	if (GetAllGameProfileResult.Num() == 0)
	{
		FAccelByteModelsGameProfileRequest Request = CreateGameProfileRequest(5, "key", "val", 3, "tag");

		FAccelByteModelsGameProfile ActualResult;
		bool bCreateGameProfileSuccess = false;
		FRegistry::GameProfile.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&ActualResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result)
		{
			ActualResult = Result;
			Profile = Result;
			bCreateGameProfileSuccess = true;
		}), StatisticTestErrorHandler);
		FlushHttpRequests();
		Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");

		check(bCreateGameProfileSuccess)
			check(ActualResult.attributes.Num() == Request.attributes.Num());
		TArray<FString> Keys;
		Request.attributes.GetKeys(Keys);
		for (int i = 0; i < Keys.Num(); i++)
		{
			check(ActualResult.attributes[Keys[i]] == Request.attributes[Keys[i]]);
		}
		check(ActualResult.avatarUrl == Request.avatarUrl);
		check(ActualResult.label == Request.label);
		check(ActualResult.profileName == Request.profileName);
		for (int i = 0; i < Request.tags.Num(); i++)
		{
			check(ActualResult.tags[i] == Request.tags[i]);
		}
	}
	else
	{
		Profile = GetAllGameProfileResult[0];
	}

	check(bClientLoginSuccess);
	check(UsersLoginSuccess);

	bool bStatIsExist = false;
	bool bGetStatDone = false;
	FAccelByteModelsStatInfo GetStatResult;
	Statistic_Get_Stat_By_StatCode("MVP", THandler<FAccelByteModelsStatInfo>::CreateLambda([&bStatIsExist, &bGetStatDone, &GetStatResult](const FAccelByteModelsStatInfo& Result)
	{
		GetStatResult = Result;
		bStatIsExist = true;
		bGetStatDone = true;
	}), FErrorHandler::CreateLambda([&bStatIsExist, &bGetStatDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bGetStatDone = true;
	}));
	FlushHttpRequests();
	Waiting(bStatIsExist, "Waiting for get stat...");

	if (!bStatIsExist)
	{
		bool bCreateStatDone = false;
		FAccelByteModelsStatInfo CreateStatResult;
		FStatCreateRequest createStat;
		createStat.defaultValue = 0;
		createStat.description = "Player MVP";
		createStat.incrementOnly = true;
		createStat.maximum = 999999;
		createStat.minimum = 0;
		createStat.name = "MVP";
		createStat.setAsGlobal = true;
		createStat.setBy = EAccelByteStatisticSetBy::CLIENT;
		createStat.statCode = "MVP";

		Statistic_Create_Stat(createStat, THandler<FAccelByteModelsStatInfo>::CreateLambda([&bStatIsExist, &bCreateStatDone, &CreateStatResult](FAccelByteModelsStatInfo Result)
		{
			CreateStatResult = Result;
			bCreateStatDone = true;
			bStatIsExist = true;
		}), StatisticTestErrorHandler);
		FlushHttpRequests();
		Waiting(bCreateStatDone, "Waiting for stat created...");
		check(bCreateStatDone);
	}
	check(bStatIsExist);
	
	bool bStatItemIsExist = false;
	bool bGetStatItemDone = false;
	TArray<FAccelByteModelsUserStatItemInfo> GetStatItemResult;
	FRegistry::Statistic.GetStatItemsByStatCodes(Profile.profileId, { "MVP" }, THandler<TArray<FAccelByteModelsUserStatItemInfo>>::CreateLambda([&bGetStatItemDone, &bStatItemIsExist, &GetStatItemResult](TArray<FAccelByteModelsUserStatItemInfo> Result)
	{
		GetStatItemResult = Result;
		bGetStatItemDone = true;
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetStatItemDone, "Waiting for get statitem...");
	check(bGetStatDone);
	if (GetStatItemResult.Num() == 0)
	{
		bool bCreateStatItemDone = false;
		TArray<FAccelByteModelsBulkStatItemIncResult> CreateStatItemResult;
		Statistic_Bulk_Create_StatItem(Profile.userId, Profile.profileId, { "MVP" }, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([&bStatItemIsExist, &bCreateStatItemDone, &CreateStatItemResult](TArray<FAccelByteModelsBulkStatItemIncResult> Result)
		{
			CreateStatItemResult = Result;
			bStatItemIsExist = true;
			bCreateStatItemDone = true;
		}), StatisticTestErrorHandler);
		FlushHttpRequests();
		Waiting(bCreateStatItemDone, "Waiting for statitem created...");
		check(bCreateStatItemDone);
	}
	else
	{
		bStatItemIsExist = true;
	}
	check(bStatItemIsExist);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticTearDown, "AccelByte.Tests.Statistic.Z.Teardown", AutomationFlagMaskStatistic);
bool StatisticTearDown::RunTest(const FString& Parameters)
{
    bool bDeleteUsersSuccessful = false;

    UE_LOG(LogAccelByteStatisticTest, Log, TEXT("DeleteUserById..."));
    DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Success"));
        bDeleteUsersSuccessful = true;
    }), StatisticTestErrorHandler);
    FlushHttpRequests();
    Waiting(bDeleteUsersSuccessful, "Waiting for user deletion...");


    check(bDeleteUsersSuccessful);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticGetAllStatItems, "AccelByte.Tests.Statistic.B.GetAllStatItems_Success", AutomationFlagMaskStatistic);
bool StatisticGetAllStatItems ::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING ALL STATITEMS..."));
	bool bGetAllStatItemsSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::Statistic.GetAllStatItems(Profile.profileId, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetAllStatItemsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET ALL STATITEMS SUCCESS!"));
		bGetAllStatItemsSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetAllStatItemsSuccess,"Waiting for get all stat items...");
	check(bGetAllStatItemsSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticGetStatItemsByStatCodes, "AccelByte.Tests.Statistic.B.GetStatItemsByStatCodes_Success", AutomationFlagMaskStatistic);
bool StatisticGetStatItemsByStatCodes::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING STATITEMS BY STATCODES..."));
	bool bGetStatItemsByStatCodesSuccess = false;
	TArray<FAccelByteModelsUserStatItemInfo> GetResult;
	FRegistry::Statistic.GetStatItemsByStatCodes(Profile.profileId, {"MVP"}, THandler<TArray<FAccelByteModelsUserStatItemInfo>>::CreateLambda([this, &GetResult, &bGetStatItemsByStatCodesSuccess](const TArray<FAccelByteModelsUserStatItemInfo>& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET STATITEMS SUCCESS!"));
		bGetStatItemsByStatCodesSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetStatItemsByStatCodesSuccess,"Waiting for get stat items...");
	check(bGetStatItemsByStatCodesSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticBulkAddStatItemValue, "AccelByte.Tests.Statistic.B.BulkAddStatItemValue_Success", AutomationFlagMaskStatistic);
bool StatisticBulkAddStatItemValue::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD STATITEM VALUE..."));
	bool bBulkAddStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemIncResult> BulkAddStatItemResult;
	FAccelByteModelsBulkUserStatItemInc MVP;
	MVP.inc = 1;
	MVP.profileId = Profile.profileId;
	MVP.statCode = "MVP";
	FRegistry::Statistic.BulkAddStatItemValue({ MVP }, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([&bBulkAddStatItemSuccess, &BulkAddStatItemResult](TArray<FAccelByteModelsBulkStatItemIncResult> Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD STATITEMS SUCCESS!"));
		bBulkAddStatItemSuccess = true;
		BulkAddStatItemResult = Result;
		for (FAccelByteModelsBulkStatItemIncResult data : Result)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Success: %d"), *data.StatCode, data.Success);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bBulkAddStatItemSuccess, "Waiting for bulk add statitem...");
	check(bBulkAddStatItemSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticBulkAddUserStatItemValue, "AccelByte.Tests.Statistic.B.BulkAddUserStatItemValue_Success", AutomationFlagMaskStatistic);
bool StatisticBulkAddUserStatItemValue::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD USER STATITEM VALUE..."));
	bool bBulkAddUserStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemIncResult> BulkAddUserStatItemResult;
	FAccelByteModelsBulkStatItemInc MVP;
	MVP.inc = 1;
	MVP.statCode = "MVP";
	FRegistry::Statistic.BulkAddUserStatItemValue(Profile.profileId, { MVP }, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([&bBulkAddUserStatItemSuccess, &BulkAddUserStatItemResult](TArray<FAccelByteModelsBulkStatItemIncResult> Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD USER STATITEMS SUCCESS!"));
		bBulkAddUserStatItemSuccess = true;
		BulkAddUserStatItemResult = Result;
		for (FAccelByteModelsBulkStatItemIncResult data : Result)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Success: %d"), *data.StatCode, data.Success);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bBulkAddUserStatItemSuccess, "Waiting for bulk add user statitem...");
	check(bBulkAddUserStatItemSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticAddUserStatItemValue, "AccelByte.Tests.Statistic.B.AddUserStatItemValue_Success", AutomationFlagMaskStatistic);
bool StatisticAddUserStatItemValue::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("ADD USER STATITEM VALUE..."));
	bool bAddUserStatItemSuccess = false;
	FAccelByteModelsStatItemIncResult AddUserStatItemResult;
	FString statCode = "MVP";
	const float increasedValue = 1;
	FRegistry::Statistic.AddUserStatItemValue(Profile.profileId, statCode, increasedValue, THandler<FAccelByteModelsStatItemIncResult>::CreateLambda([&bAddUserStatItemSuccess, &AddUserStatItemResult](FAccelByteModelsStatItemIncResult Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("ADD USER STATITEMS SUCCESS!"));
		bAddUserStatItemSuccess = true;
		AddUserStatItemResult = Result;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Current Value: %d"), Result.currentValue);
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bAddUserStatItemSuccess, "Waiting for add user statitem...");
	check(bAddUserStatItemSuccess);
	return true;
}
