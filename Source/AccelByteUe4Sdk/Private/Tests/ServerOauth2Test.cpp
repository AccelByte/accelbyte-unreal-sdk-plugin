// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Core/AccelByteRegistry.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"
#include "Core/AccelByteMultiRegistry.h"

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

DEFINE_SPEC(
	FServerOauth2Test,
	"AccelByte.Tests.ServerOauth2Test",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)


void FServerOauth2Test::Define()
{
	Describe("Given a ServerOauth2", [this]()
	{
		It("When login with client credentials, Then should be successful", [this]()
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
		});

		It("When token expired, Then automatically refreshed", [this]()
		{
			FRegistry::ServerOauth2.ForgetAllCredentials();
		
			bool bClientTokenObtained = false;
		
			UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("LoginWithClientCredentials"));
			FRegistry::ServerOauth2.LoginWithClientCredentials(
				FVoidHandler::CreateLambda(
					[&]()
					{
						UE_LOG(LogAccelByteServerOauth2Test, Log, TEXT("    Success"));
						bClientTokenObtained = true;
					}),
				ServerOauthErrorHandler);

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
		});

		It("When getting JWKS, Then should be successful", [this]()
		{
			FJwkSet KeySet{};
			auto bJwkSetObtained{false};

			FRegistry::ServerOauth2.GetJwks(
				THandler<FJwkSet>::CreateLambda([&](FJwkSet const& JWKSet)
				{
					KeySet = JWKSet;
					bJwkSetObtained = true;
				}),
				ServerOauthErrorHandler);

			WaitUntil(bJwkSetObtained, "Waiting for JWKS", 3);

			AB_TEST_TRUE(bJwkSetObtained);
			AB_TEST_TRUE(KeySet.keys.Num() > 0);
			AB_TEST_TRUE(KeySet.keys[0].JsonObject->HasField("n"));

			return true;
		});
	});
}
