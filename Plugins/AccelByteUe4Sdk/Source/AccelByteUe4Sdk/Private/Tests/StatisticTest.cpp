// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteUserApi.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteRegistry.h"
#include "AccelByteStatisticApi.h"
#include "AccelByteGameProfileApi.h"
#include "TestUtilities.h"
#include "FileManager.h"

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

Credentials UserCred;
TSharedPtr<Api::GameProfile> gameProfile;
TSharedPtr<Api::Statistic> statistic;
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
	bool UsersCreationSuccess;
	bool UsersLoginSuccess;

	User::LoginWithClientCredentials(FSimpleDelegate::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Client Login Success"));
	}), StatisticTestErrorHandler);
	FlushHttpRequests();


	FString Email = FString::Printf(TEXT("StatisticUE4Test@example.com"));
	FString Password = FString::Printf(TEXT("password"), 0);
	FString DisplayName = FString::Printf(TEXT("StatisticUE4Test"), 0);

	User::Register(Email, Password, DisplayName, THandler<FUserData>::CreateLambda([&](const FUserData& Response)
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

	AccelByte::Api::Oauth2::GetAccessTokenWithPasswordGrant(
		FRegistry::Settings.ClientId,
		FRegistry::Settings.ClientSecret,
		Email,
		Password,
		THandler<FOauth2Token>::CreateLambda([&](const FOauth2Token& Token)
	{
		UsersLoginSuccess = true;
		UserCred.SetUserToken(Token.Access_token, Token.Refresh_token, Token.Expires_in, Token.User_id, Token.Display_name, Token.Namespace);
	}), StatisticTestErrorHandler);
	FlushHttpRequests();

	gameProfile = MakeShared<Api::GameProfile>(UserCred, FRegistry::Settings);
	statistic = MakeShared<Api::Statistic>(UserCred, FRegistry::Settings);

	TArray<FAccelByteModelsGameProfile> GetAllGameProfileResult;
	bool bGetAllGameProfileSuccess = false;
	gameProfile->GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsGameProfile>& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t\tsuccess"));
		GetAllGameProfileResult = Result;
		bGetAllGameProfileSuccess = true;
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t%d game profile is found!"), GetAllGameProfileResult.Num());
	check(bGetAllGameProfileSuccess);
	if (GetAllGameProfileResult.Num() == 0)
	{
		FAccelByteModelsGameProfileRequest Request = CreateGameProfileRequest(5, "key", "val", 3, "tag");

		FAccelByteModelsGameProfile ActualResult;
		bool bCreateGameProfileSuccess = false;
		gameProfile->CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&ActualResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result)
		{
			ActualResult = Result;
			Profile = Result;
			bCreateGameProfileSuccess = true;
		}), StatisticTestErrorHandler);
		FlushHttpRequests();

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

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticGetAllStatItems, "AccelByte.Tests.Statistic.B.GetAllStatItems_Success", AutomationFlagMaskStatistic);
bool StatisticGetAllStatItems ::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING ALL STATITEMS..."));
	bool bGetAllStatItemsSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	statistic->GetAllStatItems(Profile.profileId, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetAllStatItemsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
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
	check(bGetAllStatItemsSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticGetStatItemsByStatCodes, "AccelByte.Tests.Statistic.B.GetStatItemsByStatCodes_Success", AutomationFlagMaskStatistic);
bool StatisticGetStatItemsByStatCodes::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING STATITEMS BY STATCODES..."));
	bool bGetStatItemsByStatCodesSuccess = false;
	TArray<FAccelByteModelsUserStatItemInfo> GetResult;
	statistic->GetStatItemsByStatCodes(Profile.profileId, {"TOTAL_KILLS"}, THandler<TArray<FAccelByteModelsUserStatItemInfo>>::CreateLambda([this, &GetResult, &bGetStatItemsByStatCodesSuccess](const TArray<FAccelByteModelsUserStatItemInfo>& Result)
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
	check(bGetStatItemsByStatCodesSuccess);
	return true;
}