// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Public/Test/IntegrationTest.h"
#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteServicesOrder.h"
#include "AccelByteServicesCategory.h"
#include "AccelByteServicesItem.h"
#include "AccelByteServicesUser.h"
#include "AccelByteModelsIdentity.h"
#include "AccelByteModelsUserProfile.h"
#include "AccelByteSettings.h"
#include "AccelByteHttpRetrySystem.h"
#include "AccelByteCredentials.h"
#include <algorithm>
#include "FileManager.h"

DEFINE_LOG_CATEGORY(LogAccelByteTest);
const int32 AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

using namespace AccelByte;
using namespace AccelByte::Services;

#define ServerBaseUrl Settings::ServerBaseUrl
#define ClientId Settings::ClientId
#define ClientSecret Settings::ClientSecret
#define Namespace Settings::Namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LoginGameClientSuccess, "LogAccelByteTest.LoginGameClientSuccess", AutomationFlagMask);
bool LoginGameClientSuccess::RunTest(const FString& Parameters)
{
	User::ResetCredentials();
	bool bHasDone = false;
	bool bClientTokenObtained = false;
	double LastTime = 0;

	UE_LOG(LogAccelByteTest, Log, TEXT("ClientLogin"));
	Identity::GetAccessTokenWithClientCredentialsGrant(ServerBaseUrl, ClientId, ClientSecret, Identity::FGetAccessTokenWithClientCredentialsGrantSuccess::CreateLambda([&bHasDone, &bClientTokenObtained](FAccelByteModelsOAuthToken Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
		UserCredentials.SetClientAccessToken(Result);
	}), ErrorDelegate::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bClientTokenObtained);
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterTest, "LogAccelByteTest.RegisterEmail_ThenLogin_Successful", AutomationFlagMask);
bool FUserRegisterTest::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@gmail.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	double LastTime = 0;
	
	bool bHasDone = false;
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("ClientLogin"));
	Identity::GetAccessTokenWithClientCredentialsGrant(ServerBaseUrl, ClientId, ClientSecret, Identity::FGetAccessTokenWithClientCredentialsGrantSuccess::CreateLambda([&bHasDone, &bClientTokenObtained](FAccelByteModelsOAuthToken Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
		UserCredentials.SetClientAccessToken(Result);
	}), ErrorDelegate::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("CreateEmailAccount"));
	User::CreateEmailAccount(ServerBaseUrl, LoginId, Password, DisplayName, User::FRegisterEmailAccountSuccess::CreateLambda([&bHasDone, &bRegisterSuccessful](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("   Success"));
		bRegisterSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bHasDone, &bRegisterSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bRegisterSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bool bLoginSuccessful = false;
	bHasDone = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithEmailAccount"));
	User::LoginWithEmailAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, LoginId, Password, User::FLoginWithEmailSuccess::CreateLambda([&bHasDone, &bLoginSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bHasDone, &bLoginSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser
	
	UE_LOG(LogAccelByteTest, Log, TEXT("Assert.."));
	check(bLoginSuccessful);
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginTest, "LogAccelByteTest.LoginEmail_ThenVerify_Successful", AutomationFlagMask);
bool FUserLoginTest::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@gmail.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bHasDone = false;
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("ClientLogin"));
	Identity::GetAccessTokenWithClientCredentialsGrant(ServerBaseUrl, ClientId, ClientSecret, Identity::FGetAccessTokenWithClientCredentialsGrantSuccess::CreateLambda([&bHasDone, &bClientTokenObtained](FAccelByteModelsOAuthToken Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
		UserCredentials.SetClientAccessToken(Result);
	}), ErrorDelegate::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("CreateEmailAccount"));
	User::CreateEmailAccount(ServerBaseUrl, LoginId, Password, DisplayName, User::FRegisterEmailAccountSuccess::CreateLambda([&bHasDone, &bRegisterSuccessful](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bHasDone, &bRegisterSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bRegisterSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bool bLoginSuccessful = false;
	bHasDone = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithEmailAccount"));
	User::LoginWithEmailAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, LoginId, Password, User::FLoginWithEmailSuccess::CreateLambda([&bHasDone, &bLoginSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("   Success"));
		bLoginSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bHasDone, &bLoginSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FString VerificationCode = FIntegrationTestModule::GetVerificationCode(LoginId);
	UE_LOG(LogAccelByteTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bVerifySuccessful = false;
	bHasDone = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("VerifyEmailAccount"));
	User::VerifyEmailAccount(ServerBaseUrl, UserCredentials.GetUserAccessToken().User_id, VerificationCode, User::FVerifyRegistrationSuccess::CreateLambda([&bHasDone, &bVerifySuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bVerifySuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bHasDone, &bVerifySuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bVerifySuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bLoginSuccessful);
	check(bVerifySuccessful);
	check(bDeleteSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserResetPasswordTest, "LogAccelByteTest.RegisterEmail_ThenResetPassword_Successful", AutomationFlagMask);
bool FUserResetPasswordTest::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@gmail.com";
	FString Password = "old_password";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bHasDone = false;
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("ClientLogin"));
	Identity::GetAccessTokenWithClientCredentialsGrant(ServerBaseUrl, ClientId, ClientSecret, Identity::FGetAccessTokenWithClientCredentialsGrantSuccess::CreateLambda([&bHasDone, &bClientTokenObtained](FAccelByteModelsOAuthToken Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
		UserCredentials.SetClientAccessToken(Result);
	}), ErrorDelegate::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("CreateEmailAccount"));
	User::CreateEmailAccount(ServerBaseUrl, LoginId, Password, DisplayName, User::FRegisterEmailAccountSuccess::CreateLambda([&bHasDone, &bRegisterSuccessful](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
		bHasDone = true;
	}),ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bRegisterSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bForgotPaswordSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("RequestPasswordResetCode"));
	User::RequestPasswordResetCode(ServerBaseUrl, ClientId, ClientSecret, Namespace, LoginId, User::FRequestResetPasswordSuccess::CreateLambda([&bHasDone, &bForgotPaswordSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bForgotPaswordSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bForgotPaswordSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FString VerificationCode = FIntegrationTestModule::GetVerificationCode(LoginId);
	UE_LOG(LogAccelByteTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	bHasDone = false;
	Password = "new_password";
	UE_LOG(LogAccelByteTest, Log, TEXT("ResetPasswordWithCode"));
	User::ResetPasswordWithCode(ServerBaseUrl, ClientId, ClientSecret, Namespace, LoginId, VerificationCode, Password, User::FVerifyResetPasswordSuccess::CreateLambda([&bHasDone, &bResetPasswordSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bResetPasswordSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bHasDone, &bResetPasswordSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bResetPasswordSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	
	bool bLoginSuccessful = false;
	bHasDone = false;
	UE_LOG(LogTemp, Log, TEXT("LoginWithEmailAccount"));
	User::LoginWithEmailAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, LoginId, Password, User::FLoginWithEmailSuccess::CreateLambda([&bHasDone, &bLoginSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bHasDone, &bLoginSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser

	check(bForgotPaswordSuccessful);
	check(bResetPasswordSuccessful);
	check(bResetPasswordSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdSuccess, "LogAccelByteTest.LoginWithDeviceId.LoginTwiceGetSameUserId", AutomationFlagMask);
bool FLoginWithDeviceIdSuccess::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithDeviceId"));
	User::LoginWithDeviceId(ServerBaseUrl, ClientId, ClientSecret, Namespace, User::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bHasDone = true;
		bDeviceLoginSuccessful1 = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bHasDone = true;
		bDeviceLoginSuccessful1 = false;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = UserCredentials.GetUserAccessToken().User_id;

	bHasDone = false;
	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithDeviceId //Second attempt"))
	User::LoginWithDeviceId(ServerBaseUrl, ClientId, ClientSecret, Namespace, User::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful2, &bHasDone]() 
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful2 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = UserCredentials.GetUserAccessToken().User_id;

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser

	check(bDeviceLoginSuccessful1)
		check(bDeviceLoginSuccessful2)
		check(bDeleteSuccessful)
		check(FirstUserId == SecondUserId && FirstUserId != "" && SecondUserId != "")
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdUniqueIdCreated, "LogAccelByteTest.LoginWithDeviceId.UniqueUserIdCreatedForEachDevice", AutomationFlagMask);
bool FLoginWithDeviceIdUniqueIdCreated::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	User::LoginWithDeviceId(ServerBaseUrl, ClientId, ClientSecret, Namespace, User::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful1 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = UserCredentials.GetUserAccessToken().User_id;

#pragma region DeleteUser

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful1 = false;
		bDeleteDone1 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser

	User::ResetCredentials();
	
	bHasDone = false;
	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithDeviceId // Second attempt"))
	User::LoginWithDeviceId(ServerBaseUrl, ClientId, ClientSecret, Namespace, User::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful2, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful2 = true;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = UserCredentials.GetUserAccessToken().User_id;

#pragma region DeleteUser

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful2 = false;
		bDeleteDone2 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		if (bDeleteDone2) { UE_LOG(LogAccelByteTest, Log, TEXT("DeleteDone")); }
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser
	
	UE_LOG(LogAccelByteTest, Log, TEXT("Asserting..."))
		check(bDeviceLoginSuccessful1)
		check(bDeviceLoginSuccessful2)
		check(bDeleteSuccessful1)
		check(bDeleteSuccessful2)
		check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "")
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeDeviceAccountSuccess, "LogAccelByteTest.UpgradeHeadlessDeviceAccount", AutomationFlagMask);
bool FUpgradeDeviceAccountSuccess::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString Email = TEXT("testSDK@example.com");
	FString Password = TEXT("password");
	bool bUpgradedHeadlessAccountUserIdRemain = false;
	FString FirstUserId = "";
	FString SecondUserId = "";
	FString ThirdUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	User::LoginWithDeviceId(ServerBaseUrl, ClientId, ClientSecret, Namespace, User::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful1 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = UserCredentials.GetUserAccessToken().User_id;

	bHasDone = false;
	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("UpgradeHeadlessAccount"));
	User::UpgradeHeadlessAccount(ServerBaseUrl, Email, Password, User::FUpgradeHeadlessAccountSuccess::CreateLambda([&bUpgradeSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bUpgradeSuccessful, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bUpgradeSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	User::ResetCredentials();

	bHasDone = false;
	bool bEmailLoginSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithEmailAccount"));
	User::LoginWithEmailAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, Email, Password, User::FLoginWithEmailSuccess::CreateLambda([&bEmailLoginSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bEmailLoginSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bEmailLoginSuccessful, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bEmailLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bUpgradedHeadlessAccountUserIdRemain = (FirstUserId == UserCredentials.GetUserAccessToken().User_id && UserCredentials.GetUserAccessToken().User_id != "");
	User::ResetCredentials();
	
	bHasDone = false;
	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithDeviceId // Second attempt"));
	User::LoginWithDeviceId(ServerBaseUrl, ClientId, ClientSecret, Namespace, User::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful2, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful2 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = UserCredentials.GetUserAccessToken().User_id;

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser1"));
	FIntegrationTestModule::DeleteUser(FirstUserId, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful1 = false;
		bDeleteDone1 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser1

#pragma region DeleteUser2

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser2"));
	FIntegrationTestModule::DeleteUser(SecondUserId, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful2 = false;
		bDeleteDone2 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser2

	check(bUpgradeSuccessful)
		check(bUpgradedHeadlessAccountUserIdRemain)
		check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "")
		check(bEmailLoginSuccessful)
		check(bDeviceLoginSuccessful1)
		check(bDeviceLoginSuccessful2)
		check(bDeleteSuccessful1)
		check(bDeleteSuccessful2)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamSuccess, "LogAccelByteTest.LoginWithSteam.LoginTwiceGetSameUserId", AutomationFlagMask);
bool FLoginWithSteamSuccess::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;
	
	bool bHasDone = false;
	bool bSteamLoginSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	User::LoginWithOtherPlatformAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), User::FLoginWithPlatformAccountSuccess::CreateLambda([&bSteamLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful1 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bSteamLoginSuccessful1 = false;
		bHasDone = true;

	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = UserCredentials.GetUserAccessToken().User_id;
	User::ResetCredentials();
	
	bHasDone = false;
	bool bSteamLoginSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithSteamAccount // Second attempt"));
	User::LoginWithOtherPlatformAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), User::FLoginWithPlatformAccountSuccess::CreateLambda([&bSteamLoginSuccessful2, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful2 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bSteamLoginSuccessful2 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = UserCredentials.GetUserAccessToken().User_id;

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser

	check(bSteamLoginSuccessful1)
		check(bSteamLoginSuccessful2)
		check(bDeleteSuccessful)
		check(FirstUserId == SecondUserId && FirstUserId != "" && SecondUserId != "")
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamUniqueIdCreated, "LogAccelByteTest.LoginWithSteam.UniqueUserIdCreatedForSteamAccount", AutomationFlagMask);
bool FLoginWithSteamUniqueIdCreated::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bSteamLoginSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	User::LoginWithOtherPlatformAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), User::FLoginWithPlatformAccountSuccess::CreateLambda([&bHasDone, &bSteamLoginSuccessful1]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful1 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bSteamLoginSuccessful1 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = UserCredentials.GetUserAccessToken().User_id;

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser
	
	User::ResetCredentials();

	bHasDone = false;
	bool bSteamLoginSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithSteamAccount // Second Attempt"));
	User::LoginWithOtherPlatformAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), User::FLoginWithPlatformAccountSuccess::CreateLambda([&bHasDone, &bSteamLoginSuccessful2]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bHasDone = true;
		bSteamLoginSuccessful2 = true;
	}), ErrorDelegate::CreateLambda([&bHasDone, &bSteamLoginSuccessful2](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bHasDone = true;
		bSteamLoginSuccessful2 = false;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = UserCredentials.GetUserAccessToken().User_id;

#pragma region DeleteUser

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful2 = false;
		bDeleteDone2 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser

	check(bSteamLoginSuccessful1);
	check(bSteamLoginSuccessful2);
	check(bDeleteSuccessful);
	check(bDeleteSuccessful2);
	check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "");
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeSteamAccountSuccess, "LogAccelByteTest.UpgradeHeadlessSteamAccount.Success", AutomationFlagMask);
bool FUpgradeSteamAccountSuccess::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FString Email = TEXT("testSDK@example.com");
	FString Password = TEXT("password");
	double LastTime = 0;
	FString FirstUserId = TEXT("");

	bool bHasDone = false;
	bool bLoginPlatformSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithSteamAccount"));
	User::LoginWithOtherPlatformAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), User::FLoginWithEmailSuccess::CreateLambda([&bLoginPlatformSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bLoginPlatformSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginPlatformSuccessful = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FirstUserId = UserCredentials.GetUserAccessToken().User_id;

	bHasDone = false;
	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("UpgradeHeadlessAccount"));
	User::UpgradeHeadlessAccount(ServerBaseUrl, Email, Password, User::FUpgradeHeadlessAccountSuccess::CreateLambda([&bUpgradeSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bUpgradeSuccessful, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bUpgradeSuccessful = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	User::ResetCredentials();

	bHasDone = false;
	bool bLoginEmailSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithEmailAccount"));
	User::LoginWithEmailAccount(ServerBaseUrl, ClientId, ClientSecret, Namespace, Email, Password, User::FLoginWithEmailSuccess::CreateLambda([&bLoginEmailSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bLoginEmailSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&bLoginEmailSuccessful, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginEmailSuccessful = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser1"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful1 = false;
		bDeleteDone1 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser1

	check(FirstUserId == UserCredentials.GetUserAccessToken().User_id);
	check(bLoginPlatformSuccessful);
	check(bUpgradeSuccessful);
	check(bLoginEmailSuccessful);
	check(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileUtilitiesSuccess, "LogAccelByteTest.GetAndUpdateProfile.Success", AutomationFlagMask);
bool FUserProfileUtilitiesSuccess::RunTest(const FString & Parameter)
{
	User::ResetCredentials();
	FAccelByteModelsUserProfileInfoUpdate ProfileUpdate;
	ProfileUpdate.Country = "US";
	ProfileUpdate.Language = "en";
	ProfileUpdate.Timezone = "Asia/Shanghai";
	ProfileUpdate.DateOfBirth = "1999-09-09";
	ProfileUpdate.Email = "test@example.com";
	ProfileUpdate.DisplayName = "TestProfile";
	FString UpdatedDisplayName = TEXT("");
	double LastTime = 0;

	bool bHasDone = false;
	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("LoginWithDeviceId"));
	User::LoginWithDeviceId(ServerBaseUrl, ClientId, ClientSecret, Namespace, User::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful1 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bGetProfileSuccessful1 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("GetProfile"));
	User::GetProfile(ServerBaseUrl, User::FGetProfileSuccess::CreateLambda([&bGetProfileSuccessful1, &bHasDone](FAccelByteModelsUserProfileInfo Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bHasDone = true;
		bGetProfileSuccessful1 = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bHasDone = true;
		bGetProfileSuccessful1 = false;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bUpdateProfileSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT(""));
	User::UpdateProfile(ServerBaseUrl, ProfileUpdate, User::FUpdateProfileSuccess::CreateLambda([&bUpdateProfileSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bUpdateProfileSuccessful = true;
		bHasDone = true;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bUpdateProfileSuccessful = false;
		bHasDone = true;

	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bGetProfileSuccessful2 = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("GetProfile // Second attempt"));
	User::GetProfile(ServerBaseUrl, User::FGetProfileSuccess::CreateLambda([&bGetProfileSuccessful2, &bHasDone, &UpdatedDisplayName](FAccelByteModelsUserProfileInfo Result)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bGetProfileSuccessful2 = true;
		bHasDone = true;
		UpdatedDisplayName = Result.DisplayName;
	}), ErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bGetProfileSuccessful2 = false;
		bHasDone = true;

	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(UserCredentials.GetUserAccessToken().User_id, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), ErrorDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		RetrySystem.Manager.Update();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser

	check(bDeviceLoginSuccessful1);
	check(bGetProfileSuccessful2);
	check(bUpdateProfileSuccessful);
	check(UpdatedDisplayName == ProfileUpdate.DisplayName);
	check(bGetProfileSuccessful2);
	check(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSteamTicket, "LogAccelByteTest.SteamTicket.Success", AutomationFlagMask);
bool FGetSteamTicket::RunTest(const FString & Parameter)
{
	FString Ticket = FIntegrationTestModule::GetSteamTicket();
	UE_LOG(LogAccelByteTest, Log, TEXT("Print Steam Ticket :\r\n%s"), *Ticket);
	check(Ticket != TEXT(""))
		return true;
}

void FIntegrationTestModule::DeleteUser(FString UserId, FDeleteUserByIdSuccess OnSuccess, ErrorDelegate OnError)
{
	Identity::GetAccessTokenWithClientCredentialsGrant(ServerBaseUrl, ClientId, ClientSecret, Identity::FGetAccessTokenWithClientCredentialsGrantSuccess::CreateLambda([UserId, OnSuccess, OnError](FAccelByteModelsOAuthToken Result)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Result.Access_token);
		FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s"), *ServerBaseUrl, *Namespace, *UserId);
		FString Verb = TEXT("DELETE");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content = TEXT("");
		FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
		{
			FIntegrationTestModule::OnDeleteUserComplete(Request, Response, OnSuccess, OnError);
		});
		Request->ProcessRequest();
	}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));

}

void FIntegrationTestModule::OnDeleteUserComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, FDeleteUserByIdSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

FString FIntegrationTestModule::GetVerificationCode(FString Email)
{
	FString VerificationCodeOutput = TEXT("");
	FString CurrentDirectory = TEXT("");
	CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("TestUtilities/justice-test-utilities-windows_amd64.exe"));
	CurrentDirectory.Replace(TEXT("/"), TEXT("\\"));
	UE_LOG(LogAccelByteTest, Log, TEXT("%s"), *CurrentDirectory);
	FString args = TEXT("verificationcode -a " + Email);
	FWindowsPlatformProcess::ExecProcess(CurrentDirectory.GetCharArray().GetData(), *args, nullptr, &VerificationCodeOutput, nullptr);
	return VerificationCodeOutput;
}

FString FIntegrationTestModule::GetSteamTicket()
{
	FString SteamTicket = TEXT("");
	FString SteamHelperOutput = TEXT("");
	FString CurrentDirectory = TEXT("");
	CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("SteamHelper/SteamTicketHelper.exe"));
	CurrentDirectory.Replace(TEXT("/"), TEXT("\\"));
	FWindowsPlatformProcess::ExecProcess(CurrentDirectory.GetCharArray().GetData(), nullptr, nullptr, &SteamHelperOutput, nullptr);
	SteamHelperOutput.Split(TEXT("STEAMTICKET\r\n"), nullptr, &SteamTicket);
	UE_LOG(LogAccelByteTest, Log, TEXT("%s"), *SteamTicket);
	return SteamTicket;
}