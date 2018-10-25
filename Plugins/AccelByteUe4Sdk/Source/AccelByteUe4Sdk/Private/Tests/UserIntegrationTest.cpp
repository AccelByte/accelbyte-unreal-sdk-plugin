// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteOrderApi.h"
#include "AccelByteUserManagementApi.h"
#include "AccelByteUserProfileModels.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "FileManager.h"
#include "AccelByteUserAuthenticationApi.h"
#include "AccelByteUserProfileApi.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::Settings;
using AccelByte::HandleHttpError;
using AccelByte::Api::UserAuthentication;
using AccelByte::Api::UserManagement;
using AccelByte::Api::UserProfile;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteUserTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteUserTest);

const int32 AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

DECLARE_DELEGATE(FDeleteUserByIdSuccess);
static void DeleteUserById(const FString& UserID, const FDeleteUserByIdSuccess& OnSuccess, const FErrorHandler& OnError);

static FString GetVerificationCode(const FString& Email);
static FString GetSteamTicket();

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UpdateUserAccountTest, "AccelByte.Tests.User.UpdateUserAccount", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);
bool UpdateUserAccountTest::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteUserTest, Display, TEXT("Settings"))
	UE_LOG(LogAccelByteUserTest, Display, TEXT("  ClientId: %s"), *Settings::ClientId)
	UE_LOG(LogAccelByteUserTest, Display, TEXT("  ClientSecret: %s"), *Settings::ClientSecret)
	UE_LOG(LogAccelByteUserTest, Display, TEXT("  GameId: %s"), *Settings::GameId)
	UE_LOG(LogAccelByteUserTest, Display, TEXT("  PublisherId: %s"), *Settings::PublisherId)
	UE_LOG(LogAccelByteUserTest, Display, TEXT("  IamServerUrl: %s"), *Settings::IamServerUrl)
	UE_LOG(LogAccelByteUserTest, Display, TEXT("  PlatformServerUrl: %s"), *Settings::PlatformServerUrl)
	UE_LOG(LogAccelByteUserTest, Display, TEXT("  LobbyServerUrl: %s"), *Settings::LobbyServerUrl)
    const auto GlobalErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    });
	const FString Username = FString::Printf(TEXT("bahamut+%s@bahamut.test"), *FGuid::NewGuid().ToString(EGuidFormats::Digits));
	const FString Password = TEXT("My super top secret password 1");
	const FString DisplayName = TEXT("Bahamut");

    UE_LOG(LogAccelByteUserTest, Display, TEXT("LoginWithClientCredentialsEasy"))
    UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([](){UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."))}), GlobalErrorHandler);
    FHttpModule::Get().GetHttpManager().Flush(false);
    
    UE_LOG(LogAccelByteUserTest, Display, TEXT("CreateUserAccountEasy"))
    UserManagement::CreateUserAccountEasy(Username, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([](const FAccelByteModelsUserCreateResponse& Result){UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."))}), GlobalErrorHandler);
    FHttpModule::Get().GetHttpManager().Flush(false);
	
    UE_LOG(LogAccelByteUserTest, Display, TEXT("LoginWithUsernameAndPasswordEasy"))
    UserAuthentication::LoginWithUsernameAndPasswordEasy(Username, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([](){UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."))}), GlobalErrorHandler);
    FHttpModule::Get().GetHttpManager().Flush(false);

    UE_LOG(LogAccelByteUserTest, Display, TEXT("UpdateUserAccountEasy"))
	FAccelByteModelsUserUpdateRequest UpdateRequest
	{
		TEXT("US")
	};
    UserManagement::UpdateUserAccountEasy(UpdateRequest, UserManagement::FUpdateUserAccountSuccess::CreateLambda([](){UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."))}), GlobalErrorHandler);
    FHttpModule::Get().GetHttpManager().Flush(false);

	return true;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LoginGameClientSuccess, "AccelByte.Tests.User.LoginGameClient", AutomationFlagMask);
bool LoginGameClientSuccess::RunTest(const FString& Parameters)
{
	UserAuthentication::ForgetAllCredentials();
	bool bHasDone = false;
	bool bClientTokenObtained = false;
	double LastTime = 0;

	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&bHasDone, &bClientTokenObtained]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bClientTokenObtained);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterTest, "AccelByte.Tests.User.RegisterEmail_ThenLogin", AutomationFlagMask);
bool FUserRegisterTest::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bHasDone = false;
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&bHasDone, &bClientTokenObtained]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccountEasy"));
	UserManagement::CreateUserAccountEasy(LoginId, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([&bHasDone, &bRegisterSuccessful](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bRegisterSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bRegisterSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bRegisterSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bool bLoginSuccessful = false;
	bHasDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPasswordEasy(LoginId, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&bHasDone, &bLoginSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bLoginSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	check(bLoginSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginTest, "AccelByte.Tests.User.LoginEmail_ThenVerify", AutomationFlagMask);
bool FUserLoginTest::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bHasDone = false;
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&bHasDone, &bClientTokenObtained]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccountEasy"));
	UserManagement::CreateUserAccountEasy(LoginId, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([&bHasDone, &bRegisterSuccessful](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bRegisterSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bRegisterSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bool bLoginSuccessful = false;
	bHasDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPasswordEasy(LoginId, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&bHasDone, &bLoginSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bLoginSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bLoginSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bool bSendSuccessful = false;
	bHasDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("SendEmailVerificationCode"));
	UserManagement::SendUserAccountVerificationCodeEasy(LoginId, UserManagement::FVerifyUserAccountSuccess::CreateLambda([&bHasDone, &bSendSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSendSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bSendSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bSendSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	///////////////////
	//FString VerificationCode = FIntegrationTestModule::GetVerificationCode(LoginId);
	//UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);
	///////////////////

	//bool bVerifySuccessful = false;
	//bHasDone = false;
	//UE_LOG(LogAccelByteUserTest, Log, TEXT("VerifyEmailAccount"));
	//UserManagement::VerifyUserAccountEasy(VerificationCode, UserManagement::FVerifyUserAccountSuccess::CreateLambda([&bHasDone, &bVerifySuccessful]()
	//{
	//	UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
	//	bVerifySuccessful = true;
	//	bHasDone = true;
	//}), FBlueprintErrorHandler::CreateLambda([&bHasDone, &bVerifySuccessful](int32 Code, FString Message)
	//{
	//	UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
	//	bVerifySuccessful = false;
	//	bHasDone = true;
	//}));

	//LastTime = FPlatformTime::Seconds();
	//while (!bHasDone)
	//{
	//	const double AppTime = FPlatformTime::Seconds();
	//	FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
	//	LastTime = AppTime;
	//	FPlatformProcess::Sleep(0.5f);
	//}

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bLoginSuccessful);
	check(bSendSuccessful);
	check(bDeleteSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserResetPasswordTest, "AccelByte.Tests.User.RegisterEmail_ThenResetPassword", AutomationFlagMask);
bool FUserResetPasswordTest::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	FString Password = "old_password";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bHasDone = false;
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&bHasDone, &bClientTokenObtained]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccountEasy"));
	UserManagement::CreateUserAccountEasy(LoginId, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([&bHasDone, &bRegisterSuccessful](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bRegisterSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bForgotPaswordSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("RequestPasswordReset"));
	UserManagement::SendPasswordResetCodeEasy(LoginId, UserManagement::FSendUserAccountVerificationCodeSuccess::CreateLambda([&bHasDone, &bForgotPaswordSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bForgotPaswordSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bForgotPaswordSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FString VerificationCode = GetVerificationCode(LoginId);
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	bHasDone = false;
	Password = "new_password";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("ResetPassword"));
	UserManagement::ResetPasswordEasy(LoginId, VerificationCode, Password, UserManagement::FResetPasswordSuccess::CreateLambda([&bHasDone, &bResetPasswordSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bResetPasswordSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bResetPasswordSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bResetPasswordSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bool bLoginSuccessful = false;
	bHasDone = false;
	UE_LOG(LogTemp, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPasswordEasy(LoginId, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&bHasDone, &bLoginSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bLoginSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

#if 0
	check(bForgotPaswordSuccessful);
	check(bResetPasswordSuccessful);
#endif
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdSuccess, "AccelByte.Tests.User.LoginWithDeviceId.LoginTwiceGetSameUserId", AutomationFlagMask);
bool FLoginWithDeviceIdSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	UserAuthentication::LoginWithDeviceId(Settings::ClientId, Settings::ClientSecret, UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bHasDone = true;
		bDeviceLoginSuccessful1 = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bHasDone = true;
		bDeviceLoginSuccessful1 = false;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = Credentials::Get().GetUserId();

	bHasDone = false;
	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId //Second attempt"))
		UserAuthentication::LoginWithDeviceId(Settings::ClientId, Settings::ClientSecret, UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful2, &bHasDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeviceLoginSuccessful2 = true;
			bHasDone = true;
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
			bDeviceLoginSuccessful2 = false;
			bHasDone = true;
		}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = Credentials::Get().GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

	check(bDeviceLoginSuccessful1)
	check(bDeviceLoginSuccessful2)
	check(bDeleteSuccessful)
	check(FirstUserId == SecondUserId && FirstUserId != "" && SecondUserId != "")
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdUniqueIdCreated, "AccelByte.Tests.User.LoginWithDeviceId.UniqueUserIdCreatedForEachDevice", AutomationFlagMask);
bool FLoginWithDeviceIdUniqueIdCreated::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	UserAuthentication::LoginWithDeviceId(Settings::ClientId, Settings::ClientSecret, UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful1 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = Credentials::Get().GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful1 = false;
		bDeleteDone1 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

	UserAuthentication::ForgetAllCredentials();

	bHasDone = false;
	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"))
		UserAuthentication::LoginWithDeviceId(Settings::ClientId, Settings::ClientSecret, UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful2, &bHasDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeviceLoginSuccessful2 = true;
			bHasDone = true;
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
			bDeviceLoginSuccessful2 = true;
			bHasDone = true;
		}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = Credentials::Get().GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful2 = false;
		bDeleteDone2 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		if (bDeleteDone2) { UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteDone")); }
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Asserting..."))
		check(bDeviceLoginSuccessful1)
		check(bDeviceLoginSuccessful2)
		check(bDeleteSuccessful1)
		check(bDeleteSuccessful2)
		check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "")
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeDeviceAccountSuccess, "AccelByte.Tests.User.UpgradeHeadlessDeviceAccount", AutomationFlagMask);
bool FUpgradeDeviceAccountSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString Email = TEXT("testSDK@game.test");
	FString Password = TEXT("password");
	bool bUpgradedHeadlessAccountUserIdRemain = false;
	FString FirstUserId = "";
	FString SecondUserId = "";
	FString ThirdUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bClientLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientLoginSuccessful = true;
		bHasDone = true;
	}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bClientLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	UserAuthentication::LoginWithDeviceId(Settings::ClientId, Settings::ClientSecret, UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful1 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = Credentials::Get().GetUserId();

	bHasDone = false;
	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	UserManagement::AddUsernameAndPasswordEasy(Email, Password, UserManagement::FAddUsernameAndPasswordSuccess::CreateLambda([&bUpgradeSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bUpgradeSuccessful, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bUpgradeSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UserAuthentication::ForgetAllCredentials();

	bHasDone = false;
	bool bEmailLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPasswordEasy(Email, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&bEmailLoginSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bEmailLoginSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bEmailLoginSuccessful, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bEmailLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bUpgradedHeadlessAccountUserIdRemain = (FirstUserId == Credentials::Get().GetUserId() && Credentials::Get().GetUserId() != "");
	UserAuthentication::ForgetAllCredentials();

	bHasDone = false;
	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"));
	UserAuthentication::LoginWithDeviceId(Settings::ClientId, Settings::ClientSecret, UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful2, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful2 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = Credentials::Get().GetUserId();

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FirstUserId, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful1 = false;
		bDeleteDone1 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser1

#pragma region DeleteUser2

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser2"));
	DeleteUserById(SecondUserId, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful2 = false;
		bDeleteDone2 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamSuccess, "AccelByte.Tests.User.LoginWithSteam.LoginTwiceGetSameUserId", AutomationFlagMask);
bool FLoginWithSteamSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bSteamLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	UserAuthentication::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([&bSteamLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful1 = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bSteamLoginSuccessful1 = false;
		bHasDone = true;

	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = Credentials::Get().GetUserId();
	UserAuthentication::ForgetAllCredentials();

	bHasDone = false;
	bool bSteamLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // Second attempt"));
	UserAuthentication::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([&bSteamLoginSuccessful2, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful2 = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bSteamLoginSuccessful2 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = Credentials::Get().GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

	check(bSteamLoginSuccessful1)
		check(bSteamLoginSuccessful2)
		check(bDeleteSuccessful)
		check(FirstUserId == SecondUserId && FirstUserId != "" && SecondUserId != "")
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamUniqueIdCreated, "AccelByte.Tests.User.LoginWithSteam.UniqueUserIdCreatedForSteamAccount", AutomationFlagMask);
bool FLoginWithSteamUniqueIdCreated::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bHasDone = false;
	bool bSteamLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	UserAuthentication::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([&bHasDone, &bSteamLoginSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful1 = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bSteamLoginSuccessful1 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = Credentials::Get().GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

	UserAuthentication::ForgetAllCredentials();

	bHasDone = false;
	bool bSteamLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // Second Attempt"));
	UserAuthentication::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([&bHasDone, &bSteamLoginSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bHasDone = true;
		bSteamLoginSuccessful2 = true;
	}), FErrorHandler::CreateLambda([&bHasDone, &bSteamLoginSuccessful2](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bHasDone = true;
		bSteamLoginSuccessful2 = false;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = Credentials::Get().GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful2 = false;
		bDeleteDone2 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

	check(bSteamLoginSuccessful1);
	check(bSteamLoginSuccessful2);
	check(bDeleteSuccessful);
	check(bDeleteSuccessful2);
	check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "");
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeSteamAccountSuccess, "AccelByte.Tests.User.UpgradeHeadlessSteamAccount", AutomationFlagMask);
bool FUpgradeSteamAccountSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString Email = TEXT("testSDK@game.test");
	FString Password = TEXT("password");
	double LastTime = 0;
	FString FirstUserId = TEXT("");

	bool bHasDone = false;
	bool bClientLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientLoginSuccessful = true;
		bHasDone = true;
	}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bClientLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bLoginPlatformSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	UserAuthentication::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&bLoginPlatformSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginPlatformSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginPlatformSuccessful = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FirstUserId = Credentials::Get().GetUserId();

	bHasDone = false;
	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	UserManagement::AddUsernameAndPasswordEasy(Email, Password, UserManagement::FAddUsernameAndPasswordSuccess::CreateLambda([&bUpgradeSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bUpgradeSuccessful, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bUpgradeSuccessful = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UserAuthentication::ForgetAllCredentials();

	bHasDone = false;
	bool bLoginEmailSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPasswordEasy(Email, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&bLoginEmailSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginEmailSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&bLoginEmailSuccessful, &bHasDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bLoginEmailSuccessful = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful1 = false;
		bDeleteDone1 = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser1

	check(FirstUserId == Credentials::Get().GetUserId());
	check(bLoginPlatformSuccessful);
	check(bUpgradeSuccessful);
	check(bLoginEmailSuccessful);
	check(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileUtilitiesSuccess, "AccelByte.Tests.User.GetAndUpdateProfile", AutomationFlagMask);
bool FUserProfileUtilitiesSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FAccelByteModelsUserProfileUpdateRequest ProfileUpdate;
	ProfileUpdate.Country = "US";
	ProfileUpdate.Language = "en";
	ProfileUpdate.Timezone = "Etc/UTC";
	ProfileUpdate.DateOfBirth = "2000-01-01";
	ProfileUpdate.Email = "test@game.test";
	ProfileUpdate.DisplayName = "TestProfile";
	FString UpdatedDisplayName = TEXT("");
	double LastTime = 0;

	bool bHasDone = false;
	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	UserAuthentication::LoginWithDeviceIdEasy(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&bDeviceLoginSuccessful1, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeviceLoginSuccessful1 = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bCreateProfileSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateProfile"));

	FAccelByteModelsUserProfileCreateRequest ProfileCreate;
	ProfileCreate.Country = "US";
	ProfileCreate.Language = "en";
	ProfileCreate.Timezone = "Etc/UTC";
	ProfileCreate.DateOfBirth = "1970-01-01";
	ProfileCreate.Email = "test@game.test";
	ProfileCreate.DisplayName = "CreateProfile";

	UserProfile::CreateUserProfileEasy(ProfileCreate, UserProfile::FCreateUserProfileSuccess::CreateLambda([&bCreateProfileSuccessful1, &bHasDone](FAccelByteModelsUserProfileInfo Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bHasDone = true;
		bCreateProfileSuccessful1 = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if (Code != 2271)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
			bHasDone = true;
			bCreateProfileSuccessful1 = false;
		}
		else
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bHasDone = true;
			bCreateProfileSuccessful1 = true;
		}
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;

	bool bGetProfileSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile"));
	UserProfile::GetUserProfileEasy(UserProfile::FGetUserProfileSuccess::CreateLambda([&bGetProfileSuccessful1, &bHasDone](FAccelByteModelsUserProfileInfo Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bHasDone = true;
		bGetProfileSuccessful1 = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bHasDone = true;
		bGetProfileSuccessful1 = false;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bUpdateProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpdateProfile"));
	UserProfile::UpdateUserProfileEasy(ProfileUpdate, UserProfile::FUpdateUserProfileSuccess::CreateLambda([&bUpdateProfileSuccessful, &bHasDone]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpdateProfileSuccessful = true;
		bHasDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bUpdateProfileSuccessful = false;
		bHasDone = true;

	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bGetProfileSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile // Second attempt"));
	UserProfile::GetUserProfileEasy(UserProfile::FGetUserProfileSuccess::CreateLambda([&bGetProfileSuccessful2, &bHasDone, &UpdatedDisplayName](FAccelByteModelsUserProfileInfo Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetProfileSuccessful2 = true;
		bHasDone = true;
		UpdatedDisplayName = Result.DisplayName;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bGetProfileSuccessful2 = false;
		bHasDone = true;

	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUserById

	check(bDeviceLoginSuccessful1);
	check(bGetProfileSuccessful2);
	check(bUpdateProfileSuccessful);
	check(UpdatedDisplayName == ProfileUpdate.DisplayName);
	check(bGetProfileSuccessful2);
	check(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSteamTicket, "AccelByte.Tests.User.SteamTicket", AutomationFlagMask);
bool FGetSteamTicket::RunTest(const FString & Parameter)
{
	FString Ticket = GetSteamTicket();
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Print Steam Ticket :\r\n%s"), *Ticket);
	check(Ticket != TEXT(""))
		return true;
}

void DeleteUserById(const FString& UserId, const FDeleteUserByIdSuccess& OnSuccess, const FErrorHandler& OnError)
{
	using AccelByte::Settings;
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([OnSuccess, OnError, UserId]()
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms/justice/%s"), *Settings::IamServerUrl, *Settings::GameId, *UserId, *Settings::PublisherId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content;
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
		{
			int32 Code = 0;
			FString Message;
			if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				TSharedPtr<FJsonObject> JsonParsed;
				TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
				if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
				{
					FString RealUserId = JsonParsed->GetStringField("UserId");

					FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetClientAccessToken());
					FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s"), *Settings::IamServerUrl, *Settings::PublisherId, *RealUserId);
					FString Verb = TEXT("DELETE");
					FString ContentType = TEXT("application/json");
					FString Accept = TEXT("application/json");
					FString Content;
					FHttpRequestPtr Request2 = FHttpModule::Get().CreateRequest();
					Request2->SetURL(Url);
					Request2->SetHeader(TEXT("Authorization"), Authorization);
					Request2->SetVerb(Verb);
					Request2->SetHeader(TEXT("Content-Type"), ContentType);
					Request2->SetHeader(TEXT("Accept"), Accept);
					Request2->SetContentAsString(Content);
					Request2->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
					{
						int32 Code;
						FString Message;
						if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
						{
							OnSuccess.ExecuteIfBound();
						}
						else
						{
							HandleHttpError(Request, Response, Code, Message);
							OnError.ExecuteIfBound(Code, Message);
						}
					});
					Request2->ProcessRequest();
				}
				else
				{
					HandleHttpError(Request, Response, Code, Message);
					OnError.ExecuteIfBound(Code, Message);
				}
			}
			else
			{
				HandleHttpError(Request, Response, Code, Message);
				OnError.ExecuteIfBound(Code, Message);
			}
		});
		Request->ProcessRequest();
	}), FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}



FString GetVerificationCode(const FString& Email)
{
	FString VerificationCodeOutput = TEXT("");
	FString CurrentDirectory = TEXT("");
	CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("TestUtilities/justice-test-utilities-windows_amd64.exe"));
	CurrentDirectory.Replace(TEXT("/"), TEXT("\\"));
	UE_LOG(LogAccelByteUserTest, Log, TEXT("%s"), *CurrentDirectory);
	FString args = TEXT("verificationcode -a " + Email);
#ifdef _WIN32
	FWindowsPlatformProcess::ExecProcess(CurrentDirectory.GetCharArray().GetData(), *args, nullptr, &VerificationCodeOutput, nullptr);
#endif
	return VerificationCodeOutput;
}

FString GetSteamTicket()
{
	FString SteamTicket = TEXT("");
	FString SteamHelperOutput = TEXT("");
	FString CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("SteamHelper/steamticket.txt"));
	CurrentDirectory.Replace(TEXT("/"), TEXT("\\"));
	FFileHelper::LoadFileToString(SteamTicket, *CurrentDirectory);

	return SteamTicket;
}
