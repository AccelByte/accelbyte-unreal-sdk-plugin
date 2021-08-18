// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Misc/AutomationTest.h"
#include "Core/AccelByteRegistry.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"

using namespace std;

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::Settings;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::UserProfile;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerOauth2Test, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerOauth2Test);

const int32 AutomationFlagMaskServerOauth = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto ServerOauthErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteServerOauth2Test, Error, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LoginGameClientSuccess, "AccelByte.Tests.ServerOauth2.LoginGameClient", AutomationFlagMaskServerOauth);
bool LoginGameClientSuccess::RunTest(const FString& Parameters)
{
	FRegistry::ServerOauth2.ForgetAllCredentials();
	bool bClientTokenObtained = false;
	
	UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), ServerOauthErrorHandler);

	WaitUntil(bClientTokenObtained, "Waiting for Login...");

	AB_TEST_TRUE(bClientTokenObtained);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ClientAutomatedRefreshTokenTest, "AccelByte.Tests.ServerOauth2.AutomatedRefreshToken", AutomationFlagMaskServerOauth);
bool ClientAutomatedRefreshTokenTest::RunTest(const FString& Parameter)
{
	FRegistry::ServerOauth2.ForgetAllCredentials();
	
	bool bClientTokenObtained = false;
	
	UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("    Success"));
			bClientTokenObtained = true;
		}), ServerOauthErrorHandler);

	WaitUntil(bClientTokenObtained, "Waiting for Login...");

	// set session expired time to 0
	const FString ClientAccessToken = FRegistry::ServerCredentials.GetClientAccessToken();
	const FString ClientNamespace = FRegistry::ServerCredentials.GetClientNamespace();
	FRegistry::ServerCredentials.SetClientToken(ClientAccessToken, FPlatformTime::Seconds() + 2, ClientNamespace);
	FString NewClientAccessToken;

	WaitUntil(
		[&]()
		{
			NewClientAccessToken = FRegistry::ServerCredentials.GetClientAccessToken();
			return ClientAccessToken != NewClientAccessToken;
		},
		"Wait refresh token success",
		10);

	AB_TEST_NOT_EQUAL(ClientAccessToken, NewClientAccessToken);
	return true;
}