// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteOrderApi.h"
#include "AccelByteUserManagementApi.h"
#include "AccelByteUserProfileModels.h"
#include "AccelByteRegistry.h"
#include "FileManager.h"
#include "AccelByteUserAuthenticationApi.h"
#include "AccelByteUserProfileApi.h"
#include "TestUtilities.h"

using namespace std;

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::Settings;
using AccelByte::HandleHttpError;
using AccelByte::Api::UserAuthentication;
using AccelByte::Api::UserManagement;
using AccelByte::Api::UserProfile;

typedef FSimpleDelegate FDeleteUserByIdSuccess;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteUserTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteUserTest);

const int32 AutomationFlagMaskUser = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

void FlushHttpRequests();//defined in TestUtilities.cpp

static FString GetVerificationCode(const FString& Email);
FString GetVerificationCodeFromUserId(const FString& UserId);
static FString GetSteamTicket();
const auto GlobalErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteUserTest, Fatal, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UpdateUserAccountTest, "AccelByte.Tests.User.UpdateUserAccount", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);
bool UpdateUserAccountTest::RunTest(const FString& Parameters)
{
	const FString OriginalEmail = FString::Printf(TEXT("originalEmail+%s@example.com"), *FGuid::NewGuid().ToString(EGuidFormats::Digits));
	const FString UpdatedEmail = FString::Printf(TEXT("updatedEmail+%s@example.com"), *FGuid::NewGuid().ToString(EGuidFormats::Digits));
	const FString Password = TEXT("password");
	const FString DisplayName = TEXT("testName");

	bool bClientLoggedIn = false;
	UE_LOG(LogAccelByteUserTest, Display, TEXT("LoginWithClientCredentials"));
    UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&bClientLoggedIn]()
	{
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bClientLoggedIn = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bUserAccountCreated = false;
	UE_LOG(LogAccelByteUserTest, Display, TEXT("CreateUserAccount"));
    UserManagement::CreateUserAccount(OriginalEmail, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([&bUserAccountCreated](const FAccelByteModelsUserCreateResponse& Result)
	{
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bUserAccountCreated = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bUserLoggedIn = false;
	UE_LOG(LogAccelByteUserTest, Display, TEXT("LoginWithUsernameAndPassword"));
    UserAuthentication::LoginWithUsernameAndPassword(OriginalEmail, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&bUserLoggedIn]()
	{
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bUserLoggedIn = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bUserUpdated = false;
    UE_LOG(LogAccelByteUserTest, Display, TEXT("UpdateUserAccount"))
	FAccelByteModelsUserUpdateRequest UpdateRequest
	{
		TEXT("US"),
		TEXT("UpdateName"),
		FString(UpdatedEmail),
		FString()
	};
    UserManagement::UpdateUserAccount(UpdateRequest, UserManagement::FUpdateUserAccountSuccess::CreateLambda([&bUserUpdated](const FAccelByteModelsUserResponse& Result) 
	{
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bUserUpdated = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bLoginWithUpdatedAccount = false;
	UE_LOG(LogAccelByteUserTest, Display, TEXT("LoginWithUsernameAndPassword (Updated Email)"))
	UserAuthentication::LoginWithUsernameAndPassword(UpdatedEmail, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&]() {
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bLoginWithUpdatedAccount = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	check(bLoginWithUpdatedAccount);
	check(bDeleteDone);
	return true;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LoginGameClientSuccess, "AccelByte.Tests.User.LoginGameClient", AutomationFlagMaskUser);
bool LoginGameClientSuccess::RunTest(const FString& Parameters)
{
	UserAuthentication::ForgetAllCredentials();
	bool bClientTokenObtained = false;
	double LastTime = 0;

	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	check(bClientTokenObtained);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterTest, "AccelByte.Tests.User.RegisterEmail_ThenLogin", AutomationFlagMaskUser);
bool FUserRegisterTest::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	UserManagement::CreateUserAccount(LoginId, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([&](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bRegisterSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPassword(LoginId, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	check(bLoginSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginTest, "AccelByte.Tests.User.LoginEmail_ThenVerify", AutomationFlagMaskUser);
bool FUserLoginTest::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	UserManagement::CreateUserAccount(LoginId, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([&](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPassword(LoginId, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bSendSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("SendEmailVerificationCode"));
	const FAccelByteModelsSendVerificationCodeRequest& Request = FAccelByteModelsSendVerificationCodeRequest
	{
		EAccelByteVerificationCodeContext::UserAccountRegistration,
		"",
		LoginId
	};
	UserManagement::SendUserAccountVerificationCode(Request, UserManagement::FVerifyUserAccountSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSendSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	///////////////////
	//FString VerificationCode = FIntegrationTestModule::GetVerificationCode(LoginId);
	//UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);
	///////////////////

	//bool bVerifySuccessful = false;
	//UE_LOG(LogAccelByteUserTest, Log, TEXT("VerifyEmailAccount"));
	//UserManagement::VerifyUserAccount(VerificationCode, UserManagement::FVerifyUserAccountSuccess::CreateLambda([&]()
	//{
	//	UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
	//	bVerifySuccessful = true;
	//}), GlobalErrorHandler);
	//FHttpModule::Get().GetHttpManager().Flush(false);

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	check(bLoginSuccessful);
	check(bSendSuccessful);
	check(bDeleteSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserResetPasswordTest, "AccelByte.Tests.User.RegisterEmail_ThenResetPassword", AutomationFlagMaskUser);
bool FUserResetPasswordTest::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	FString Password = "old_password";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	UserManagement::CreateUserAccount(LoginId, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([&](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bForgotPaswordSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("RequestPasswordReset"));
	UserManagement::SendPasswordResetCode(LoginId, UserManagement::FSendUserAccountVerificationCodeSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bForgotPaswordSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	FString VerificationCode = GetVerificationCode(LoginId);
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	Password = "new_password";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("ResetPassword"));
	UserManagement::ResetPassword(LoginId, VerificationCode, Password, UserManagement::FResetPasswordSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bResetPasswordSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bLoginSuccessful = false;
	UE_LOG(LogTemp, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPassword(LoginId, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

#if 0
	check(bForgotPaswordSuccessful);
	check(bResetPasswordSuccessful);
#endif
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdSuccess, "AccelByte.Tests.User.LoginWithDeviceId.LoginTwiceGetSameUserId", AutomationFlagMaskUser);
bool FLoginWithDeviceIdSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	FirstUserId = FRegistry::Credentials.GetUserId();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId //Second attempt"))
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	check(bDeviceLoginSuccessful1);
	check(bDeviceLoginSuccessful2);
	check(bDeleteSuccessful);
	check(FirstUserId == SecondUserId && FirstUserId != "" && SecondUserId != "");
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdUniqueIdCreated, "AccelByte.Tests.User.LoginWithDeviceId.UniqueUserIdCreatedForEachDevice", AutomationFlagMaskUser);
bool FLoginWithDeviceIdUniqueIdCreated::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	FirstUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	UserAuthentication::ForgetAllCredentials();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"))
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Asserting..."));
	check(bDeviceLoginSuccessful1);
	check(bDeviceLoginSuccessful2);
	check(bDeleteSuccessful1);
	check(bDeleteSuccessful2);
	check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "");
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeDeviceAccountSuccess, "AccelByte.Tests.User.UpgradeHeadlessDeviceAccount", AutomationFlagMaskUser);
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
	FString OldAccessToken = "", RefreshedAccessToken = "";

	bool bClientLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	FirstUserId = FRegistry::Credentials.GetUserId();
	OldAccessToken = FRegistry::Credentials.GetUserAccessToken();

	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	UserManagement::UpgradeHeadlessAccount(Email, Password, UserManagement::FUpgradeHeadlessAccountSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
	}), GlobalErrorHandler);
	
	FlushHttpRequests();
	RefreshedAccessToken = FRegistry::Credentials.GetUserAccessToken();

	UserAuthentication::ForgetAllCredentials();

	bool bEmailLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPassword(Email, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bEmailLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bUpgradedHeadlessAccountUserIdRemain = (FirstUserId == FRegistry::Credentials.GetUserId() && FRegistry::Credentials.GetUserId() != "");
	UserAuthentication::ForgetAllCredentials();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"));
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FirstUserId, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

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
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUser2

	check(bUpgradeSuccessful)
		check(bUpgradedHeadlessAccountUserIdRemain)
		check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "")
		check(bEmailLoginSuccessful)
		check(bDeviceLoginSuccessful1)
		check(bDeviceLoginSuccessful2)
		check(!OldAccessToken.IsEmpty() && !RefreshedAccessToken.IsEmpty())
		check(!OldAccessToken.Equals(RefreshedAccessToken))
		check(bDeleteSuccessful1)
		check(bDeleteSuccessful2)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamSuccess, "AccelByte.Tests.User.LoginWithSteam.LoginTwiceGetSameUserId", AutomationFlagMaskUser);
bool FLoginWithSteamSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bSteamLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	UserAuthentication::LoginWithOtherPlatformAccount(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	FirstUserId = FRegistry::Credentials.GetUserId();
	UserAuthentication::ForgetAllCredentials();

	bool bSteamLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // Second attempt"));
	UserAuthentication::LoginWithOtherPlatformAccount(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	check(bSteamLoginSuccessful1);
	check(bSteamLoginSuccessful2);
	check(bDeleteSuccessful);
	check(FirstUserId == SecondUserId && FirstUserId != "" && SecondUserId != "");
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamUniqueIdCreated, "AccelByte.Tests.User.LoginWithSteam.UniqueUserIdCreatedForSteamAccount", AutomationFlagMaskUser);
bool FLoginWithSteamUniqueIdCreated::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bSteamLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	UserAuthentication::LoginWithOtherPlatformAccount(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	FirstUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	UserAuthentication::ForgetAllCredentials();

	bool bSteamLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // Second Attempt"));
	UserAuthentication::LoginWithOtherPlatformAccount(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	check(bSteamLoginSuccessful1);
	check(bSteamLoginSuccessful2);
	check(bDeleteSuccessful);
	check(bDeleteSuccessful2);
	check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "");
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeSteamAccountSuccess, "AccelByte.Tests.User.UpgradeHeadlessSteamAccount", AutomationFlagMaskUser);
bool FUpgradeSteamAccountSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString Email = TEXT("testSDKsteam@game.test");
	FString Password = TEXT("password");
	double LastTime = 0;
	FString FirstUserId = TEXT("");
	FString OldAccessToken = "", RefreshedAccessToken = "";

	bool bClientLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bLoginPlatformSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	UserAuthentication::LoginWithOtherPlatformAccount(EAccelBytePlatformType::Steam, GetSteamTicket(), UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginPlatformSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	FirstUserId = FRegistry::Credentials.GetUserId();
	OldAccessToken = FRegistry::Credentials.GetUserAccessToken();

	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	UserManagement::UpgradeHeadlessAccount(Email, Password, UserManagement::FUpgradeHeadlessAccountSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
	}), GlobalErrorHandler);
	FlushHttpRequests();
	RefreshedAccessToken = FRegistry::Credentials.GetUserAccessToken();

	UserAuthentication::ForgetAllCredentials();

	bool bLoginEmailSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPassword(Email, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginEmailSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUser1

	check(FirstUserId == FRegistry::Credentials.GetUserId());
	check(bLoginPlatformSuccessful);
	check(bUpgradeSuccessful);
	check(bLoginEmailSuccessful);
	check(!OldAccessToken.IsEmpty() && !RefreshedAccessToken.IsEmpty())
	check(!OldAccessToken.Equals(RefreshedAccessToken))
	check(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileUtilitiesSuccess, "AccelByte.Tests.User.GetAndUpdateProfile", AutomationFlagMaskUser);
bool FUserProfileUtilitiesSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FAccelByteModelsUserProfileUpdateRequest ProfileUpdate;
	ProfileUpdate.Language = "en";
	ProfileUpdate.Timezone = "Etc/UTC";
	ProfileUpdate.DateOfBirth = "2000-01-01";
	FString UpdatedDateOfBirth = TEXT("");
	double LastTime = 0;

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bCreateProfileSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateProfile"));

	FAccelByteModelsUserProfileCreateRequest ProfileCreate;
	ProfileCreate.Language = "en";
	ProfileCreate.Timezone = "Etc/UTC";
	ProfileCreate.DateOfBirth = "1970-01-01";

	UserProfile::CreateUserProfile(ProfileCreate, UserProfile::FCreateUserProfileSuccess::CreateLambda([&](FAccelByteModelsUserProfileInfo Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bCreateProfileSuccessful1 = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if (Code != 2271)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
			bCreateProfileSuccessful1 = false;
		}
		else
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bCreateProfileSuccessful1 = true;
		}
	}));
	
	FlushHttpRequests();

	bool bGetProfileSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile"));
	UserProfile::GetUserProfile(UserProfile::FGetUserProfileSuccess::CreateLambda([&](FAccelByteModelsUserProfileInfo Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetProfileSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bUpdateProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpdateProfile"));
	UserProfile::UpdateUserProfile(ProfileUpdate, UserProfile::FUpdateUserProfileSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpdateProfileSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bGetProfileSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile // Second attempt"));
	UserProfile::GetUserProfile(UserProfile::FGetUserProfileSuccess::CreateLambda([&](FAccelByteModelsUserProfileInfo Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetProfileSuccessful2 = true;
		UpdatedDateOfBirth = Result.DateOfBirth;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUserById

	check(bDeviceLoginSuccessful1);
	check(bGetProfileSuccessful2);
	check(bUpdateProfileSuccessful);
	check(UpdatedDateOfBirth == ProfileUpdate.DateOfBirth);
	check(bGetProfileSuccessful2);
	check(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeAndVerifyHeadlessDeviceAccountSuccess, "AccelByte.Tests.User.UpgradeAndVerifyHeadlessDeviceAccount", AutomationFlagMaskUser);
bool FUpgradeAndVerifyHeadlessDeviceAccountSuccess::RunTest(const FString & Parameter)
{
	UserAuthentication::ForgetAllCredentials();
	FString Email = FString::Printf(TEXT("upgradeAndVerify+%s@example.com"), *FGuid::NewGuid().ToString(EGuidFormats::Digits));
	FString Password = TEXT("password");
	FString FirstUserId = "";
	double LastTime = 0;
	FString OldAccessToken = "", RefreshedAccessToken = "";

	bool bClientLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bDeviceLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	FirstUserId = FRegistry::Credentials.GetUserId();
	OldAccessToken = FRegistry::Credentials.GetUserAccessToken();

	bool bSendUserUpgradeVerificationCode = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("SendUserUpgradeVerificationCode"));
	UserManagement::SendUserUpgradeVerificationCode(Email, UserManagement::FSendUserUpgradeVerificationCodeSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSendUserUpgradeVerificationCode = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	bool bGetVerificationCode = false;
	FString VerificationCode = GetVerificationCodeFromUserId(FRegistry::Credentials.GetUserId());
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccountWithVerificationCode"));
	FAccelByteModelsUpgradeHeadlessAccountWithVerificationCodeRequest Request;
	Request.Code = VerificationCode;
	Request.loginId = Email;
	Request.Password = Password;
	UserManagement::UpgradeHeadlessAccountWithVerificationCode(Request, UserManagement::FUpgradeHeadlessAccountWithVerificationCodeSuccess::CreateLambda([&](const FAccelByteModelsUserResponse& Resposne) 
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetVerificationCode = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	RefreshedAccessToken = FRegistry::Credentials.GetUserAccessToken();

	bool bEmailLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPassword(Email, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bEmailLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FirstUserId, FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

#pragma endregion DeleteUser1

	check(bDeviceLoginSuccessful)
	check(bSendUserUpgradeVerificationCode)
	check(bGetVerificationCode)
	check(bEmailLoginSuccessful)
	check(!OldAccessToken.IsEmpty() && !RefreshedAccessToken.IsEmpty())
	check(!OldAccessToken.Equals(RefreshedAccessToken))
	check(bDeleteSuccessful1)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSteamTicket, "AccelByte.Tests.User.SteamTicket", AutomationFlagMaskUser);
bool FGetSteamTicket::RunTest(const FString & Parameter)
{
	FString Ticket = GetSteamTicket();
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Print Steam Ticket :\r\n%s"), *Ticket);
	check(Ticket != TEXT(""));
	return true;
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

FString GetVerificationCodeFromUserId(const FString& UserId)
{
	FString VerificationCodeOutput = TEXT("");
	FString CurrentDirectory = TEXT("");
	CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("TestUtilities/justice-test-utilities-windows_amd64.exe"));
	CurrentDirectory.Replace(TEXT("/"), TEXT("\\"));
	UE_LOG(LogAccelByteUserTest, Log, TEXT("%s"), *CurrentDirectory);
	FString args = TEXT("verificationcode -p " + UserId);
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
