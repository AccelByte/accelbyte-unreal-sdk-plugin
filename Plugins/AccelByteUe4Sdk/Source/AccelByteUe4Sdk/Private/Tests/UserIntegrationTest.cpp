// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteUserApi.h"
#include "Models/AccelByteUserProfileModels.h"
#include "Core/AccelByteRegistry.h"
#include "HAL/FileManager.h"
#include "Api/AccelByteUserProfileApi.h"
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

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteUserTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteUserTest);

const int32 AutomationFlagMaskUser = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

void FlushHttpRequests();//defined in TestUtilities.cpp
void Waiting(bool& condition, FString text);

static FString GetVerificationCode(const FString& Email);
FString GetVerificationCodeFromUserId(const FString& UserId);
static FString GetSteamTicket();

const auto GlobalErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteUserTest, Fatal, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
});

/*! commented because can't send PATCH request yet
IMPLEMENT_SIMPLE_AUTOMATION_TEST(UpdateUserAccountTest, "AccelByte.Tests.User.UpdateUserAccount", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);
bool UpdateUserAccountTest::RunTest(const FString& Parameters)
{
	const FString OriginalEmail = (FString::Printf(TEXT("originalEmail+%s@example.com"), *FGuid::NewGuid().ToString(EGuidFormats::Digits))).ToLower();
	const FString UpdatedEmail = (FString::Printf(TEXT("updatedEmail+%s@example.com"), *FGuid::NewGuid().ToString(EGuidFormats::Digits))).ToLower();
	const FString Password = TEXT("password");
	const FString DisplayName = TEXT("testName");
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bClientLoggedIn = false;
	UE_LOG(LogAccelByteUserTest, Display, TEXT("LoginWithClientCredentials"));
    FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoggedIn]()
	{
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bClientLoggedIn = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	while (!bClientLoggedIn)
	{
		Waiting("Waiting for Login...");
	}

	bool bUserAccountCreated = false;
	UE_LOG(LogAccelByteUserTest, Display, TEXT("CreateUserAccount"));
    
	FRegistry::User.Register(OriginalEmail, Password, DisplayName, Country, format, THandler<FUserData>::CreateLambda([&bUserAccountCreated](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bUserAccountCreated = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	while (!bUserAccountCreated)
	{
		Waiting("Waiting for account registered...");
	}

	bool bUserLoggedIn = false;
	UE_LOG(LogAccelByteUserTest, Display, TEXT("LoginWithUsernameAndPassword"));
    FRegistry::User.LoginWithUsername(OriginalEmail, Password, FVoidHandler::CreateLambda([&bUserLoggedIn]()
	{
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bUserLoggedIn = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	while (!bUserLoggedIn)
	{
		Waiting("Waiting for Login...");
	}

	FString OldAccessToken = FRegistry::Credentials.GetUserSessionId();

	bool bUserUpdated = false;
    UE_LOG(LogAccelByteUserTest, Display, TEXT("UpdateUserAccount"))
	FUserUpdateRequest UpdateRequest
	{
		TEXT("US"),
		FString(),
		TEXT("UpdateName"),
		FString(UpdatedEmail),
		FString()
	};
	FUserData UpdateResult;
	FRegistry::User.Update(UpdateRequest, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bUserUpdated = true;
		UpdateResult = Result;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	while (!bUserUpdated)
	{
		Waiting("Waiting for user updated...");
	}

	bool bLoginWithUpdatedAccount = false;
	UE_LOG(LogAccelByteUserTest, Display, TEXT("LoginWithUsernameAndPassword (Updated Email)"))
	FRegistry::User.LoginWithUsername(UpdatedEmail, Password, FVoidHandler::CreateLambda([&]() {
		UE_LOG(LogAccelByteUserTest, Display, TEXT("    Success."));
		bLoginWithUpdatedAccount = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	while (!bLoginWithUpdatedAccount)
	{
		Waiting("Waiting for Login...");
	}

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();

	while (!bDeleteDone)
	{
		Waiting("Waiting for Deletion...");
	}

#pragma endregion DeleteUserById

	check(bLoginWithUpdatedAccount);
	check(UpdateResult.Country.Equals("US"));
	check(bDeleteDone);
	return true;
};
*/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LoginGameClientSuccess, "AccelByte.Tests.User.LoginGameClient", AutomationFlagMaskUser);
bool LoginGameClientSuccess::RunTest(const FString& Parameters)
{
	FRegistry::User.ForgetAllCredentials();
	bool bClientTokenObtained = false;
	double LastTime = 0;

	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bClientTokenObtained, "Waiting for Login...");

	check(bClientTokenObtained);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterTest, "AccelByte.Tests.User.RegisterEmail_ThenLogin", AutomationFlagMaskUser);
bool FUserRegisterTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	LoginId.ToLowerInline();
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bClientTokenObtained, "Waiting for Login...");

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(LoginId, Password, DisplayName, Country, format, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bRegisterSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bRegisterSuccessful, "Waiting for Registered...");

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(LoginId, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	check(bLoginSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginTest, "AccelByte.Tests.User.LoginEmail_ThenVerify", AutomationFlagMaskUser);
bool FUserLoginTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	LoginId.ToLowerInline();
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bClientTokenObtained, "Waiting for Login...");

	bool bRegisterSuccessful = false;
	FUserData RegisterResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(LoginId, Password, DisplayName, Country, format, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		RegisterResult = Result;
		bRegisterSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bRegisterSuccessful, "Waiting for Registered...");

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(LoginId, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bSendSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("SendEmailVerificationCode"));
	
	FRegistry::User.SendVerificationCode(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSendSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bSendSuccessful, "Waiting for Verification to be sent...");

	bool bGetDataSuccessful = false;
	FUserData GetDataResult;
	FRegistry::User.GetData(
		THandler<FUserData>::CreateLambda([&](const FUserData& Result) 
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bGetDataSuccessful = true;
			GetDataResult = Result;
		}),
		GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bGetDataSuccessful, "Waiting for Get Data...");

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

	check(bGetDataSuccessful);
	check(bLoginSuccessful);
	check(bSendSuccessful);
	check(bDeleteSuccessful);
	check(RegisterResult.Username.Equals(GetDataResult.Username));
	check(RegisterResult.LoginId.Equals(GetDataResult.LoginId));
	check(RegisterResult.UserId.Equals(GetDataResult.UserId));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserResetPasswordTest, "AccelByte.Tests.User.RegisterEmail_ThenResetPassword", AutomationFlagMaskUser);
bool FUserResetPasswordTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	LoginId.ToLowerInline();
	FString Password = "old_password";
	FString DisplayName = "testSDK";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bClientTokenObtained, "Waiting for Login...");

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(LoginId, Password, DisplayName, Country, format, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bRegisterSuccessful, "Waiting for Registered...");

	bool bForgotPasswordSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("RequestPasswordReset"));
	
	FRegistry::User.SendResetPasswordCode(LoginId, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bForgotPasswordSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bForgotPasswordSuccessful, "Waiting for Code to be sent...");

	FString VerificationCode = GetVerificationCode(LoginId);
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	Password = "new_password";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("ResetPassword"));
	
	FRegistry::User.ResetPassword(VerificationCode, LoginId, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bResetPasswordSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bResetPasswordSuccessful, "Waiting for Reset...");

	bool bLoginSuccessful = false;
	UE_LOG(LogTemp, Log, TEXT("LoginWithUsernameAndPassword"));
	
	FRegistry::User.LoginWithUsername(LoginId, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

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
	FRegistry::User.ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bDeviceLoginSuccessful1, &FirstUserId]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    User ID: %s"), *FRegistry::Credentials.GetUserId());
		
		bDeviceLoginSuccessful1 = true;
	}), GlobalErrorHandler);
	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful1, "Waiting for Login...");
	
	FirstUserId = FRegistry::Credentials.GetUserId();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId //Second attempt"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    User ID: %s"), *FRegistry::Credentials.GetUserId());
		bDeviceLoginSuccessful2 = true;
	}), GlobalErrorHandler);
	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful2, "Waiting for Login...");

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	UE_LOG(LogAccelByteUserTest, Log, TEXT("-----------------SENT USER ID: %s-----------------"), *FRegistry::Credentials.GetUserId());
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Delete...");

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
	FRegistry::User.ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful1, "Waiting for Login...");

	FirstUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"))
		FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful2, "Waiting for Login...");

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone2, "Waiting for Deletion...");

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
	FRegistry::User.ForgetAllCredentials();
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
	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bClientLoginSuccessful, "Waiting for Login...");

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful1, "Waiting for Login...");

	FirstUserId = FRegistry::Credentials.GetUserId();
	OldAccessToken = FRegistry::Credentials.GetUserSessionId();
	UE_LOG(LogAccelByteUserTest, Log, TEXT("------------------OLD TOKEN: %s----------------"), *OldAccessToken);

	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	FRegistry::User.Upgrade(Email, Password, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bUpgradeSuccessful, "Waiting for Upgrade...");
	
	FRegistry::User.ForgetAllCredentials();

	bool bEmailLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Email, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bEmailLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bEmailLoginSuccessful, "Waiting for Login...");

	RefreshedAccessToken = FRegistry::Credentials.GetUserSessionId();

	UE_LOG(LogAccelByteUserTest, Log, TEXT("------------------NEW TOKEN: %s----------------"), *RefreshedAccessToken);

	bUpgradedHeadlessAccountUserIdRemain = (FirstUserId == FRegistry::Credentials.GetUserId() && FRegistry::Credentials.GetUserId() != "");
	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful2, "Waiting for Login...");

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FirstUserId, FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser1

#pragma region DeleteUser2

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser2"));
	DeleteUserById(SecondUserId, FVoidHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone2, "Waiting for Deletion...");

#pragma endregion DeleteUser2

	check(bUpgradeSuccessful);
	check(bUpgradedHeadlessAccountUserIdRemain);
	check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "");
	check(bEmailLoginSuccessful);
	check(bDeviceLoginSuccessful1);
	check(bDeviceLoginSuccessful2);
	check(!OldAccessToken.IsEmpty() && !RefreshedAccessToken.IsEmpty());
	check(!OldAccessToken.Equals(RefreshedAccessToken));
	check(bDeleteSuccessful1);
	check(bDeleteSuccessful2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamSuccess, "AccelByte.Tests.User.LoginWithSteam.LoginTwiceGetSameUserId", AutomationFlagMaskUser);
bool FLoginWithSteamSuccess::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bSteamLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bSteamLoginSuccessful1, "Waiting for Login...");

	FirstUserId = FRegistry::Credentials.GetUserId();
	FRegistry::User.ForgetAllCredentials();

	bool bSteamLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // Second attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bSteamLoginSuccessful2, "Waiting for Login...");

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

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
	FRegistry::User.ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	bool bSteamLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bSteamLoginSuccessful1, "Waiting for Login...");

	FirstUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	FRegistry::User.ForgetAllCredentials();

	bool bSteamLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // Second Attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bSteamLoginSuccessful2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bSteamLoginSuccessful2, "Waiting for Login...");

	SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful2 = true;
		bDeleteDone2 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone2, "Waiting for Deletion...");

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
	FRegistry::User.ForgetAllCredentials();
	FString Email = TEXT("testSDKsteam@game.test");
	FString Password = TEXT("password");
	double LastTime = 0;
	FString FirstUserId = TEXT("");
	FString OldAccessToken = "", RefreshedAccessToken = "";

	bool bClientLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bClientLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bClientLoginSuccessful, "Waiting for Login...");

	bool bLoginPlatformSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginPlatformSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginPlatformSuccessful, "Waiting for Login...");

	FirstUserId = FRegistry::Credentials.GetUserId();
	OldAccessToken = FRegistry::Credentials.GetUserSessionId();

	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	FRegistry::User.Upgrade(Email, Password, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
	}), GlobalErrorHandler);
	FlushHttpRequests();
	Waiting(bUpgradeSuccessful, "Waiting for Upgrade...");

	FRegistry::User.ForgetAllCredentials();

	bool bLoginEmailSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Email, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginEmailSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginEmailSuccessful, "Waiting for Login...");

	RefreshedAccessToken = FRegistry::Credentials.GetUserSessionId();

#pragma region DeleteUser1

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful1 = true;
		bDeleteDone1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser1

	check(FirstUserId == FRegistry::Credentials.GetUserId());
	check(bLoginPlatformSuccessful);
	check(bUpgradeSuccessful);
	check(bLoginEmailSuccessful);
	check(!OldAccessToken.IsEmpty() && !RefreshedAccessToken.IsEmpty());
	check(!OldAccessToken.Equals(RefreshedAccessToken));
	check(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileUtilitiesSuccess, "AccelByte.Tests.UserProfile.GetAndUpdateProfile", AutomationFlagMaskUser);
bool FUserProfileUtilitiesSuccess::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FAccelByteModelsUserProfileUpdateRequest ProfileUpdate;
	ProfileUpdate.Language = "en";
	ProfileUpdate.Timezone = "Etc/UTC";
	ProfileUpdate.DateOfBirth = "2000-01-01";
	FString UpdatedDateOfBirth = TEXT("");
	double LastTime = 0;

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful1, "Waiting for Login...");

	bool bCreateProfileSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateProfile"));

	FAccelByteModelsUserProfileCreateRequest ProfileCreate;
	ProfileCreate.Language = "en";
	ProfileCreate.Timezone = "Etc/UTC";
	ProfileCreate.DateOfBirth = "1970-01-01";

	FRegistry::UserProfile.CreateUserProfile(ProfileCreate, THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
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
	Waiting(bCreateProfileSuccessful1, "Waiting for Create Profile...");

	bool bGetProfileSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile"));
	FRegistry::UserProfile.GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetProfileSuccessful1 = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bGetProfileSuccessful1, "Waiting for Get Profile...");

	bool bUpdateProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpdateProfile"));
	FRegistry::UserProfile.UpdateUserProfile(ProfileUpdate, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpdateProfileSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bUpdateProfileSuccessful, "Waiting for Update...");

	bool bGetProfileSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile // Second attempt"));
	FRegistry::UserProfile.GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetProfileSuccessful2 = true;
		UpdatedDateOfBirth = Result.DateOfBirth;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bGetProfileSuccessful2, "Waiting for Get Profile...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	check(bDeviceLoginSuccessful1);
	check(bGetProfileSuccessful2);
	check(bUpdateProfileSuccessful);
	check(UpdatedDateOfBirth == ProfileUpdate.DateOfBirth);
	check(bGetProfileSuccessful2);
	check(bDeleteSuccessful);
	return true;
}

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeAndVerifyHeadlessDeviceAccountSuccess, "AccelByte.Tests.User.UpgradeAndVerifyHeadlessDeviceAccount", AutomationFlagMaskUser);
//bool FUpgradeAndVerifyHeadlessDeviceAccountSuccess::RunTest(const FString & Parameter)
//{
//	FRegistry::User.ForgetAllCredentials();
//	FString Email = FString::Printf(TEXT("upgradeAndVerify+%s@example.com"), *FGuid::NewGuid().ToString(EGuidFormats::Digits));
//	FString Password = TEXT("password");
//	FString FirstUserId = "";
//	double LastTime = 0;
//	FString OldAccessToken = "", RefreshedAccessToken = "";
//
//	bool bClientLoginSuccessful = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithClientCredentials"));
//	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bClientLoginSuccessful = true;
//	}), GlobalErrorHandler);
//
//	FlushHttpRequests();
//
//	bool bDeviceLoginSuccessful = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
//	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bDeviceLoginSuccessful = true;
//	}), GlobalErrorHandler);
//
//	FlushHttpRequests();
//
//	FirstUserId = FRegistry::Credentials.GetUserId();
//	OldAccessToken = FRegistry::Credentials.GetUserSessionId();
//
//	bool bSendUserUpgradeVerificationCode = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("SendUserUpgradeVerificationCode"));
//	FRegistry::User.SendUpgradeVerificationCode(Email, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bSendUserUpgradeVerificationCode = true;
//	}), GlobalErrorHandler);
//
//	FlushHttpRequests();
//
//	bool bGetVerificationCode = false;
//	FString VerificationCode = GetVerificationCodeFromUserId(FRegistry::Credentials.GetUserId());
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccountWithVerificationCode"));
//
//	FRegistry::User.UpgradeAndVerify(Email, Password, VerificationCode, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bGetVerificationCode = true;
//	}), GlobalErrorHandler);
//
//	FlushHttpRequests();
//	RefreshedAccessToken = FRegistry::Credentials.GetUserSessionId();
//
//	bool bEmailLoginSuccessful = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
//	FRegistry::User.LoginWithUsername(Email, Password, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bEmailLoginSuccessful = true;
//	}), GlobalErrorHandler);
//
//	FlushHttpRequests();
//
//#pragma region DeleteUser1
//
//	bool bDeleteDone1 = false;
//	bool bDeleteSuccessful1 = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
//	DeleteUserById(FirstUserId, FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bDeleteSuccessful1 = true;
//		bDeleteDone1 = true;
//	}), GlobalErrorHandler);
//
//	FlushHttpRequests();
//
//#pragma endregion DeleteUser1
//
//	check(bDeviceLoginSuccessful);
//	check(bSendUserUpgradeVerificationCode);
//	check(bGetVerificationCode);
//	check(bEmailLoginSuccessful);
//	check(!OldAccessToken.IsEmpty() && !RefreshedAccessToken.IsEmpty());
//	check(!OldAccessToken.Equals(RefreshedAccessToken));
//	check(bDeleteSuccessful1);
//	return true;
//}

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
