// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
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

UENUM(BlueprintType)
enum class EVerificationCode : uint8
{
    accountRegistration,
    accountUpgrade,
    passwordReset
};

FString GetVerificationCode(const FString& userId, EVerificationCode code);
static FString GetSteamTicket();

const auto GlobalErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteUserTest, Fatal, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterTest, "AccelByte.Tests.AUser.RegisterEmail_ThenLogin", AutomationFlagMaskUser);
bool FUserRegisterTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	LoginId.ToLowerInline();
	FString Password = "123SDKTest123";
	FString DisplayName = "testSDK";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(LoginId, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginTest, "AccelByte.Tests.AUser.LoginEmail_ThenVerify", AutomationFlagMaskUser);
bool FUserLoginTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString EmailAddress = "testeraccelbyte+" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	FString DisplayName = "testSDK";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
    FRegisterResponse RegisterResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		RegisterResult = Result;
		bRegisterSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bRegisterSuccessful, "Waiting for Registered...");

    const FString VerificationCode = GetVerificationCode(RegisterResult.UserId, EVerificationCode::accountRegistration);

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bLoginSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

    bool bVerifyUserSuccessful = false;
    UE_LOG(LogAccelByteUserTest, Log, TEXT("VerifyingAccount"));
    FRegistry::User.Verify(VerificationCode, FVoidHandler::CreateLambda([&bVerifyUserSuccessful]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
        bVerifyUserSuccessful = true;
    }), GlobalErrorHandler);

    FlushHttpRequests();
    Waiting(bVerifyUserSuccessful, "Waiting for Verfying Account...");

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
	check(bDeleteSuccessful);
	check(RegisterResult.DisplayName.Equals(GetDataResult.DisplayName));
	check(RegisterResult.EmailAddress.Equals(GetDataResult.EmailAddress));
	check(RegisterResult.UserId.Equals(GetDataResult.UserId));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserResetPasswordTest, "AccelByte.Tests.AUser.RegisterEmail_ThenResetPassword", AutomationFlagMaskUser);
bool FUserResetPasswordTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString EmailAddress = "testeraccelbyte+" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "1Old_Password1";
	FString DisplayName = "testSDK";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bRegisterSuccessful, "Waiting for Registered...");

    bool bLoginSuccessful = false;
    UE_LOG(LogTemp, Log, TEXT("LoginWithUsernameAndPassword"));
    FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
        bLoginSuccessful = true;
    }), GlobalErrorHandler);

    FlushHttpRequests();
    Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bForgotPasswordSuccessful = false; 
	UE_LOG(LogAccelByteUserTest, Log, TEXT("RequestPasswordReset"));
	
	FRegistry::User.SendResetPasswordCode(EmailAddress, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bForgotPasswordSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bForgotPasswordSuccessful, "Waiting for Code to be sent...");

    bool bGetVerificationCodeSuccess = false;
    FString VerificationCode;
    User_Get_Verification_Code(FRegistry::Credentials.GetUserId(), THandler<FVerificationCode>::CreateLambda([&VerificationCode, &bGetVerificationCodeSuccess](const FVerificationCode& result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Get Verification Code Success..!"));
        VerificationCode = result.passwordReset;
        bGetVerificationCodeSuccess = true;
    }), GlobalErrorHandler);
    Waiting(bGetVerificationCodeSuccess, "Getting Verification Code...");
    FlushHttpRequests();

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	Password = "1New_Password1";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("ResetPassword"));
	
	FRegistry::User.ResetPassword(VerificationCode, EmailAddress, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bResetPasswordSuccessful = true;
	}), GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bResetPasswordSuccessful, "Waiting for Reset...");

    UE_LOG(LogAccelByteUserTest, Log, TEXT("Logout"));
    FRegistry::User.ForgetAllCredentials();

	bLoginSuccessful = false;
	UE_LOG(LogTemp, Log, TEXT("LoginWithUsernameAndPassword"));
	
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdSuccess, "AccelByte.Tests.AUser.LoginWithDeviceId.LoginTwiceGetSameUserId", AutomationFlagMaskUser);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdUniqueIdCreated, "AccelByte.Tests.AUser.LoginWithDeviceId.UniqueUserIdCreatedForEachDevice", AutomationFlagMaskUser);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeDeviceAccountSuccess, "AccelByte.Tests.AUser.UpgradeHeadlessDeviceAccount", AutomationFlagMaskUser);
bool FUpgradeDeviceAccountSuccess::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString Email = TEXT("testSDK@game.test");
	FString Password = TEXT("123SDKTest123");
	bool bUpgradedHeadlessAccountUserIdRemain = false;
	FString FirstUserId = "";
	FString SecondUserId = "";
	FString ThirdUserId = "";
	double LastTime = 0;
	FString OldAccessToken = "", RefreshedAccessToken = "";

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamSuccess, "AccelByte.Tests.AUser.LoginWithSteam.LoginTwiceGetSameUserId", AutomationFlagMaskUser);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamUniqueIdCreated, "AccelByte.Tests.AUser.LoginWithSteam.UniqueUserIdCreatedForSteamAccount", AutomationFlagMaskUser);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeSteamAccountSuccess, "AccelByte.Tests.AUser.UpgradeHeadlessSteamAccount", AutomationFlagMaskUser);
bool FUpgradeSteamAccountSuccess::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString Email = TEXT("testSDKsteam@game.test");
	FString Password = TEXT("123SDKTest123");
	double LastTime = 0;
	FString FirstUserId = TEXT("");
	FString OldAccessToken = "", RefreshedAccessToken = "";

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileUtilitiesSuccess, "AccelByte.Tests.AUserProfile.GetAndUpdateProfile", AutomationFlagMaskUser);
bool FUserProfileUtilitiesSuccess::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FAccelByteModelsUserProfileUpdateRequest ProfileUpdate;
	ProfileUpdate.Language = "en";
	ProfileUpdate.Timezone = "Etc/UTC";
	ProfileUpdate.DateOfBirth = "2000-01-01";
    ProfileUpdate.FirstName = "first";
    ProfileUpdate.LastName = "last";
    ProfileUpdate.AvatarSmallUrl = "http://example.com";
    ProfileUpdate.AvatarUrl = "http://example.com";
    ProfileUpdate.AvatarLargeUrl = "http://example.com";
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
    ProfileCreate.FirstName = "first";
    ProfileCreate.LastName = "last";
	ProfileCreate.Language = "en";
	ProfileCreate.Timezone = "Etc/UTC";
	ProfileCreate.DateOfBirth = "1970-01-01";
    ProfileCreate.AvatarSmallUrl = "http://example.com";
    ProfileCreate.AvatarUrl = "http://example.com";
    ProfileCreate.AvatarLargeUrl = "http://example.com";

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

#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	DeleteUserProfile(FRegistry::Credentials.GetUserNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteProfileSuccessful = true;
		bDeleteProfileDone = true;
	}), GlobalErrorHandler);

	Waiting(bDeleteProfileDone, "Waiting for Deletion...");
#pragma endregion DeleteUserProfile

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
	check(bDeleteProfileSuccessful);
	check(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserByEmailAddressTest, "AccelByte.Tests.AUser.GetUserByEmailAddress", AutomationFlagMaskUser);
bool FGetUserByEmailAddressTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	LoginId.ToLowerInline();
	FString Password = "123SDKTest123";
	FString DisplayName = "testSDK";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(LoginId, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
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

	bool bGetUserDone = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(
		LoginId,
		THandler<FPagedPublicUsersInfo>::CreateLambda([&bGetUserDone, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetUserDone = true;
		ReceivedUserData = Result;
		for (auto Data : ReceivedUserData.Data)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Get User, Email: %s, DisplayName: %s, UserId: %s"), *Data.EmailAddress, *Data.DisplayName, *Data.UserId);
		}
	}),
		GlobalErrorHandler);

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

	check(bLoginSuccessful);
	check(bGetUserDone);
	check(bDeleteSuccessful);
	check(ReceivedUserData.Data.Num() != 0);
	check(ReceivedUserData.Data[0].EmailAddress == LoginId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserByDisplayNameTest, "AccelByte.Tests.AUser.GetUserByDisplayName", AutomationFlagMaskUser);
bool FGetUserByDisplayNameTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString LoginId = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@game.test";
	LoginId.ToLowerInline();
	FString Password = "123SDKTest123";
	FString DisplayName = "testSDK";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(LoginId, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
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

	bool bGetUserDone = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(
		DisplayName,
		THandler<FPagedPublicUsersInfo>::CreateLambda([&bGetUserDone, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetUserDone = true;
		ReceivedUserData = Result;
		for (auto Data : ReceivedUserData.Data)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Get User, Email: %s, DisplayName: %s, UserId: %s"), *Data.EmailAddress, *Data.DisplayName, *Data.UserId);
		}
	}),
		GlobalErrorHandler);

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

	check(bLoginSuccessful);
	check(bGetUserDone);
	check(bDeleteSuccessful);
	check(ReceivedUserData.Data.Num() != 0);
	check(ReceivedUserData.Data[0].DisplayName.Contains(DisplayName));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserBySteamUserIDTest, "AccelByte.Tests.AUser.GetUserBySteamUserID", AutomationFlagMaskUser);
bool FGetUserBySteamUserIDTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString FirstUserId = "";
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
	FString SteamUserID;

	//STEAM_USER_ID env var is supposed to be the current user logged in to steam

	const int32 length = 100;
	TCHAR SteamUserIDStr[length];
#if PLATFORM_WINDOWS
	FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("STEAM_USER_ID"), SteamUserIDStr, length);
#elif PLATFORM_LINUX
	FLinuxPlatformMisc::GetEnvironmentVariable(TEXT("STEAM_USER_ID"), SteamUserIDStr, length);
#elif PLATFORM_MAC
	FApplePlatformMisc::GetEnvironmentVariable(TEXT("STEAM_USER_ID"), SteamUserIDStr, length);
#endif
	SteamUserID = SteamUserIDStr;

	bool bGetUserDone = false;
	FUserData ReceivedUserData;
	FRegistry::User.GetUserByOtherPlatformUserId(
		EAccelBytePlatformType::Steam, 
		SteamUserID,
		THandler<FUserData>::CreateLambda([&bGetUserDone, &ReceivedUserData](const FUserData& UserData)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bGetUserDone = true;
			ReceivedUserData = UserData;
		}),
		GlobalErrorHandler);

	FlushHttpRequests();
	Waiting(bGetUserDone, "Waiting for Login...");	

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
	check(bDeleteSuccessful);
	check(ReceivedUserData.UserId.Compare("") != 0);


	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSteamTicket, "AccelByte.Tests.AUser.SteamTicket", AutomationFlagMaskUser);
bool FGetSteamTicket::RunTest(const FString & Parameter)
{
	FString Ticket = GetSteamTicket();
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Print Steam Ticket :\r\n%s"), *Ticket);
	check(Ticket != TEXT(""));
	return true;
}

FString GetVerificationCode(const FString& userId, EVerificationCode code)
{
    bool bGetVerificationCodeSuccess = false;
    FVerificationCode verificationCode;
    User_Get_Verification_Code(userId, THandler<FVerificationCode>::CreateLambda([&verificationCode, &bGetVerificationCodeSuccess](const FVerificationCode& result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Get Verification Code Success..!"));
        verificationCode = result;
        bGetVerificationCodeSuccess = true;
    }), GlobalErrorHandler);
    Waiting(bGetVerificationCodeSuccess, "Getting Verification Code...");

    switch (code)
    {
    case EVerificationCode::accountRegistration: return verificationCode.accountRegistration;
    case EVerificationCode::accountUpgrade: return verificationCode.accountUpgrade;
    case EVerificationCode::passwordReset: return verificationCode.passwordReset;
    }
    return FString("");
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
