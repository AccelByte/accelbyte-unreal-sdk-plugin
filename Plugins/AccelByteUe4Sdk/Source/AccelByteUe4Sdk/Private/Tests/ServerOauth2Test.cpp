// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Core/AccelByteRegistry.h"
#include "HAL/FileManager.h"
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

void FlushHttpRequests();//defined in TestUtilities.cpp
void Waiting(bool& condition, FString text);

const auto ServerOauthErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteServerOauth2Test, Fatal, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LoginGameClientSuccess, "AccelByte.Tests.ServerOauth2.LoginGameClient", AutomationFlagMaskServerOauth);
bool LoginGameClientSuccess::RunTest(const FString& Parameters)
{
	FRegistry::ServerOauth2.ForgetAllCredentials();
	bool bClientTokenObtained = false;
	double LastTime = 0;

	UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), ServerOauthErrorHandler);

	FlushHttpRequests();
	Waiting(bClientTokenObtained, "Waiting for Login...");

	check(bClientTokenObtained);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ClientAutomatedRefreshTokenTest, "AccelByte.Tests.ServerOauth2.AutomatedRefreshToken", AutomationFlagMaskServerOauth);
bool ClientAutomatedRefreshTokenTest::RunTest(const FString& Parameter)
{
	FRegistry::ServerOauth2.ForgetAllCredentials();

	bool bClientTokenObtained = false;
	double LastTime = 0;

	UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("    Success"));
			bClientTokenObtained = true;
		}), ServerOauthErrorHandler);

	FlushHttpRequests();
	Waiting(bClientTokenObtained, "Waiting for Login...");

	// set session expired time to 0
	FString ClientAccessToken = FRegistry::ServerCredentials.GetClientAccessToken();
	FString ClientNamespace = FRegistry::ServerCredentials.GetClientNamespace();
	FRegistry::ServerCredentials.SetClientToken(ClientAccessToken, 0, ClientNamespace);

	FString NewAccessToken = FRegistry::ServerCredentials.GetClientAccessToken();

	bool bNewClientTokenObtained = false;

	// wait session to refresh
	for (int i = 0; i < 20; i++)
	{
		FPlatformProcess::Sleep(0.5f);
		FTicker::GetCoreTicker().Tick(0.5f);

		NewAccessToken = FRegistry::ServerCredentials.GetClientAccessToken();

		if (ClientAccessToken != NewAccessToken)
		{
			UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("Refresh token Success"));
			bNewClientTokenObtained = true;
			break;
		}
	}

	check(bNewClientTokenObtained);
	return true;
}