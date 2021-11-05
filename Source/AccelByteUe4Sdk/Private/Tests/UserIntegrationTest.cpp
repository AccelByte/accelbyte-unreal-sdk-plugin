// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Models/AccelByteUserProfileModels.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteUserProfileApi.h"
#include "TestUtilities.h"
#include "TestModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "UserTestAdmin.h"

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

UENUM(BlueprintType)
enum class EVerificationCode : uint8
{
	accountRegistration,
	accountUpgrade,
	passwordReset,
	updateEmail
};

FString GetVerificationCode(const FString& userId, EVerificationCode code);

const auto UserTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteUserTest, Error, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
	});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterTest, "AccelByte.Tests.AUser.RegisterEmail_ThenLogin", AutomationFlagMaskUser);
bool FUserRegisterTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());


	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	AB_TEST_TRUE(bLoginSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterv2Test, "AccelByte.Tests.AUser.RegisterUsername_ThenLogin", AutomationFlagMaskUser);
bool FUserRegisterv2Test::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	const FString Username = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Registerv2(EmailAddress, Username, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Username, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	AB_TEST_TRUE(bLoginSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserAutomatedRefreshSessionTest, "AccelByte.Tests.AUser.AutomatedRefreshSession", AutomationFlagMaskUser);
bool FUserAutomatedRefreshSessionTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	// set session expired time to 0
	const FString AccessToken = FRegistry::Credentials.GetAccessToken();
	const FString RefreshToken = FRegistry::Credentials.GetRefreshToken();
	FString NewAccessToken = FRegistry::Credentials.GetAccessToken();
	FString NewRefreshToken = FRegistry::Credentials.GetRefreshToken();
	FRegistry::Credentials.ScheduleRefreshToken(FPlatformTime::Seconds() + 2.0);

	WaitUntil(
		[&]()
		{
			NewAccessToken = FRegistry::Credentials.GetAccessToken();
			NewRefreshToken = FRegistry::Credentials.GetRefreshToken();

			return AccessToken != NewAccessToken && RefreshToken != NewRefreshToken;
		},
		"Wait refresh token",
		10);

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_NOT_EQUAL(AccessToken, NewAccessToken);
	AB_TEST_NOT_EQUAL(RefreshToken, NewRefreshToken);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginTest, "AccelByte.Tests.AUser.LoginEmail_ThenVerify", AutomationFlagMaskUser);
bool FUserLoginTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	FRegisterResponse RegisterResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&RegisterResult, &bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			RegisterResult = Result;
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	const FString VerificationCode = GetVerificationCode(RegisterResult.UserId, EVerificationCode::accountRegistration);

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bVerifyUserSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("VerifyingAccount"));
	FRegistry::User.Verify(VerificationCode, FVoidHandler::CreateLambda([&bVerifyUserSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bVerifyUserSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bVerifyUserSuccessful, "Waiting for Verfying Account...");

	bool bGetDataSuccessful = false;
	FAccountUserData GetDataResult;
	FRegistry::User.GetData(
		THandler<FAccountUserData>::CreateLambda([&](const FAccountUserData& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
				bGetDataSuccessful = true;
				GetDataResult = Result;
			}),
		UserTestErrorHandler);

	WaitUntil(bGetDataSuccessful, "Waiting for Get Data...");

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	AB_TEST_TRUE(bGetDataSuccessful);
	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_EQUAL(RegisterResult.DisplayName, GetDataResult.DisplayName);
	AB_TEST_EQUAL(RegisterResult.UserId, GetDataResult.UserId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginFailedTest, "AccelByte.Tests.AUser.LoginEmail_Invalid", AutomationFlagMaskUser);
bool FUserLoginFailedTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bLoginSuccessful = false;
	bool bLoginDone = false;
	int32 ErrorCode;
	FString ErrorMessage;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername("", "", FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bLoginSuccessful = true;
			bLoginDone = true;
		}), FErrorHandler::CreateLambda([&](int32 Code, const FString& Message)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
				ErrorCode = Code;
				ErrorMessage = Message;
				bLoginDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bLoginDone, "Waiting for Login...");

	const FString DefaultMessage = ErrorMessages::Default.at(ErrorCode);

	AB_TEST_FALSE(bLoginSuccessful);
	AB_TEST_TRUE(bLoginDone);
	AB_TEST_FALSE(ErrorMessage != DefaultMessage);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginSteamFailedTest, "AccelByte.Tests.AUser.LoginSteam_Invalid", AutomationFlagMaskUser);
bool FUserLoginSteamFailedTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bLoginSuccessful = false;
	bool bLoginDone = false;
	int32 ErrorCode;
	FString ErrorMessage;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, "Invalid", FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bLoginSuccessful = true;
			bLoginDone = true;
		}), FErrorHandler::CreateLambda([&](int32 Code, const FString& Message)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
				ErrorCode = Code;
				ErrorMessage = Message;
				bLoginDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bLoginDone, "Waiting for Login...");

	const FString DefaultMessage = ErrorMessages::Default.at(ErrorCode);

	AB_TEST_FALSE(bLoginSuccessful);
	AB_TEST_TRUE(bLoginDone);
	AB_TEST_FALSE(ErrorMessage != DefaultMessage);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginEmailFailedNoContentTest, "AccelByte.Tests.AUser.LoginEmail_InvalidNoContent", AutomationFlagMaskUser);
bool FUserLoginEmailFailedNoContentTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bLoginSuccessful = false;
	bool bLoginDone = false;
	int32 ErrorCode;
	FString ErrorMessage;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername("Invalid", "Invalid", FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bLoginSuccessful = true;
			bLoginDone = true;
		}), FErrorHandler::CreateLambda([&](int32 Code, const FString& Message)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
				ErrorCode = Code;
				ErrorMessage = Message;
				bLoginDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bLoginDone, "Waiting for Login...");

	const FString DefaultMessage = ErrorMessages::Default.at(ErrorCode);

	AB_TEST_FALSE(bLoginSuccessful);
	AB_TEST_TRUE(bLoginDone);
	AB_TEST_TRUE(ErrorMessage == DefaultMessage);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserResetPasswordTest, "AccelByte.Tests.AUser.RegisterEmail_ThenResetPassword", AutomationFlagMaskUser);
bool FUserResetPasswordTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "1Old_Password1";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bForgotPasswordSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("RequestPasswordReset"));

	FRegistry::User.SendResetPasswordCode(EmailAddress, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bForgotPasswordSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bForgotPasswordSuccessful, "Waiting for Code to be sent...");

	bool bGetVerificationCodeSuccess = false;
	FString VerificationCode;
	AdminGetUserVerificationCode(FRegistry::Credentials.GetUserId(), THandler<FVerificationCode>::CreateLambda([&VerificationCode, &bGetVerificationCodeSuccess](const FVerificationCode& result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Get Verification Code Success..!"));
			VerificationCode = result.passwordReset;
			bGetVerificationCodeSuccess = true;
		}), UserTestErrorHandler);
	WaitUntil(bGetVerificationCodeSuccess, "Getting Verification Code...");

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	Password = "1New_Password1";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("ResetPassword"));

	FRegistry::User.ResetPassword(VerificationCode, EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bResetPasswordSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bResetPasswordSuccessful, "Waiting for Reset...");

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Logout"));
	FRegistry::User.ForgetAllCredentials();

	bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));

	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

#if 0
	AB_TEST_TRUE(bForgotPaswordSuccessful);
	AB_TEST_TRUE(bResetPasswordSuccessful);
#endif
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdSuccess, "AccelByte.Tests.AUser.LoginWithDeviceId.LoginTwiceGetSameUserId", AutomationFlagMaskUser);
bool FLoginWithDeviceIdSuccess::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bDeviceLoginSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    User ID: %s"), *FRegistry::Credentials.GetUserId());

			bDeviceLoginSuccessful1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeviceLoginSuccessful1, "Waiting for Login...");

	const FString FirstUserId = FRegistry::Credentials.GetUserId();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId //Second attempt"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    User ID: %s"), *FRegistry::Credentials.GetUserId());
			bDeviceLoginSuccessful2 = true;
		}), UserTestErrorHandler);
	WaitUntil(bDeviceLoginSuccessful2, "Waiting for Login...");

	const FString SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	UE_LOG(LogAccelByteUserTest, Log, TEXT("-----------------SENT USER ID: %s-----------------"), *FRegistry::Credentials.GetUserId());
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Delete...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeviceLoginSuccessful1);
	AB_TEST_TRUE(bDeviceLoginSuccessful2);
	AB_TEST_EQUAL(FirstUserId, SecondUserId);
	AB_TEST_FALSE(FirstUserId.IsEmpty());
	AB_TEST_FALSE(SecondUserId.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLogoutSuccess, "AccelByte.Tests.AUser.Logout", AutomationFlagMaskUser);
bool FLogoutSuccess::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful = false;

	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bDeviceLoginSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful = true;
	}), UserTestErrorHandler);

	WaitUntil(bDeviceLoginSuccessful, "Waiting for Login with device id...");

	FString UserId = FRegistry::Credentials.GetUserId();
	bool bLogoutSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Logout"));
	FRegistry::User.Logout(FVoidHandler::CreateLambda([&bLogoutSuccessful]()
	{
		bLogoutSuccessful = true;
	}), UserTestErrorHandler);
	WaitUntil(bLogoutSuccessful, "Waiting for Logout...");

#pragma region DeleteUserById

	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(UserId, FVoidHandler::CreateLambda([&bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
	}), UserTestErrorHandler);

	WaitUntil(bDeleteSuccessful, "Waiting for Delete...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeviceLoginSuccessful);
	AB_TEST_TRUE(bLogoutSuccessful);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_TRUE(FRegistry::Credentials.GetUserId().IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdUniqueIdCreated, "AccelByte.Tests.AUser.LoginWithDeviceId.UniqueUserIdCreatedForEachDevice", AutomationFlagMaskUser);
bool FLoginWithDeviceIdUniqueIdCreated::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // First attempt"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeviceLoginSuccessful1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeviceLoginSuccessful1, "Waiting for Login...");

	const FString FirstUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"))
		FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bDeviceLoginSuccessful2 = true;
			}), UserTestErrorHandler);

	WaitUntil(bDeviceLoginSuccessful2, "Waiting for Login...");

	const FString SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful2 = true;
			bDeleteDone2 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone2, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Asserting..."));
	AB_TEST_TRUE(bDeviceLoginSuccessful1);
	AB_TEST_TRUE(bDeviceLoginSuccessful2);
	AB_TEST_NOT_EQUAL(FirstUserId, SecondUserId);
	AB_TEST_FALSE(FirstUserId.IsEmpty());
	AB_TEST_FALSE(SecondUserId.IsEmpty());
	return true;
}

// TODO: Uncomment test below if bug CVE-213 is resolved
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeDeviceAccountSuccess, "AccelByte.Tests.AUser.UpgradeHeadlessDeviceAccount", AutomationFlagMaskUser);
//bool FUpgradeDeviceAccountSuccess::RunTest(const FString & Parameter)
//{
//	FRegistry::User.ForgetAllCredentials();
//	FString Email = TEXT("testSDK@game.test");
//	FString Password = TEXT("123SDKTest123");
//	bool bUpgradedHeadlessAccountUserIdRemain = false;
//	FString FirstUserId = "";
//	FString SecondUserId = "";
//	FString ThirdUserId = "";
//	double LastTime = 0;
//	FString OldAccessToken = "", RefreshedAccessToken = "";
//
//	bool bDeviceLoginSuccessful1 = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // First attempt"));
//	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bDeviceLoginSuccessful1 = true;
//	}), UserTestErrorHandler);
//
//	Waiting(bDeviceLoginSuccessful1, "Waiting for Login...");
//
//	FirstUserId = FRegistry::Credentials.GetUserId();
//	OldAccessToken = FRegistry::Credentials.GetUserSessionId();
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("------------------OLD TOKEN: %s----------------"), *OldAccessToken);
//
//	bool bUpgradeSuccessful = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
//	FRegistry::User.Upgrade(Email, Password, THandler<FAccountUserData>::CreateLambda([&](const FAccountUserData& Result)
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bUpgradeSuccessful = true;
//	}), UserTestErrorHandler);
//
//	Waiting(bUpgradeSuccessful, "Waiting for Upgrade...");
//
//	FRegistry::User.ForgetAllCredentials();
//
//	bool bEmailLoginSuccessful = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
//	FRegistry::User.LoginWithUsername(Email, Password, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bEmailLoginSuccessful = true;
//	}), UserTestErrorHandler);
//

//	Waiting(bEmailLoginSuccessful, "Waiting for Login...");
//
//	RefreshedAccessToken = FRegistry::Credentials.GetUserSessionId();
//
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("------------------NEW TOKEN: %s----------------"), *RefreshedAccessToken);
//
//	bUpgradedHeadlessAccountUserIdRemain = (FirstUserId == FRegistry::Credentials.GetUserId() && FRegistry::Credentials.GetUserId() != "");
//	FRegistry::User.ForgetAllCredentials();
//
//	bool bDeviceLoginSuccessful2 = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"));
//	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bDeviceLoginSuccessful2 = true;
//	}), UserTestErrorHandler);
//

//	Waiting(bDeviceLoginSuccessful2, "Waiting for Login...");
//
//	SecondUserId = FRegistry::Credentials.GetUserId();
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
//	}), UserTestErrorHandler);
//

//	Waiting(bDeleteDone1, "Waiting for Deletion...");
//
//#pragma endregion DeleteUser1
//
//#pragma region DeleteUser2
//
//	bool bDeleteDone2 = false;
//	bool bDeleteSuccessful2 = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser2"));
//	DeleteUserById(SecondUserId, FVoidHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bDeleteSuccessful2 = true;
//		bDeleteDone2 = true;
//	}), UserTestErrorHandler);
//

//	Waiting(bDeleteDone2, "Waiting for Deletion...");
//
//#pragma endregion DeleteUser2
//
//	AB_TEST_TRUE(bUpgradeSuccessful);
//	AB_TEST_TRUE(bUpgradedHeadlessAccountUserIdRemain);
//	AB_TEST_FALSE(FirstUserId.IsEmpty());
//	AB_TEST_FALSE(SecondUserId.IsEmpty());
//	AB_TEST_NOT_EQUAL(FirstUserId, SecondUserId);
//	AB_TEST_TRUE(bEmailLoginSuccessful);
//	AB_TEST_TRUE(bDeviceLoginSuccessful1);
//	AB_TEST_TRUE(bDeviceLoginSuccessful2);
//	AB_TEST_FALSE(OldAccessToken.IsEmpty());
//	AB_TEST_FALSE(RefreshedAccessToken.IsEmpty());
//	AB_TEST_EQUAL(OldAccessToken, RefreshedAccessToken);
//	AB_TEST_TRUE(bDeleteSuccessful1);
//	AB_TEST_TRUE(bDeleteSuccessful2);
//	return true;
//}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamSuccess, "AccelByte.Tests.AUser.LoginWithSteam.LoginTwiceGetSameUserId", AutomationFlagMaskUser);
bool FLoginWithSteamSuccess::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();

	bool bSteamLoginSuccessful1 = false;
	bool bSteamLoginDone1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bSteamLoginSuccessful1, &bSteamLoginDone1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bSteamLoginSuccessful1 = true;
			bSteamLoginDone1 = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone1](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone1 = true;
			}));

	WaitUntil(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

	const FString FirstUserId = FRegistry::Credentials.GetUserId();
	FRegistry::User.ForgetAllCredentials();

	bool bSteamLoginSuccessful2 = false;
	bool bSteamLoginDone2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // Second attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bSteamLoginSuccessful2, &bSteamLoginDone2]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bSteamLoginSuccessful2 = true;
			bSteamLoginDone2 = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone2](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone2 = true;
			}));

	WaitUntil(bSteamLoginDone2, "Waiting for Login...");

	if (!bSteamLoginSuccessful2)
	{
		return false;
	}

	const FString SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bSteamLoginSuccessful1);
	AB_TEST_TRUE(bSteamLoginSuccessful2);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_FALSE(FirstUserId.IsEmpty());
	AB_TEST_FALSE(SecondUserId.IsEmpty());
	AB_TEST_EQUAL(FirstUserId, SecondUserId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamUniqueIdCreated, "AccelByte.Tests.AUser.LoginWithSteam.UniqueUserIdCreatedForSteamAccount", AutomationFlagMaskUser);
bool FLoginWithSteamUniqueIdCreated::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();

	bool bSteamLoginSuccessful1 = false;
	bool bSteamLoginDone1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bSteamLoginSuccessful1, &bSteamLoginDone1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bSteamLoginSuccessful1 = true;
			bSteamLoginDone1 = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone1](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone1 = true;
			}));

	WaitUntil(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

	const FString FirstUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	FRegistry::User.ForgetAllCredentials();

	bool bSteamLoginSuccessful2 = false;
	bool bSteamLoginDone2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // Second Attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bSteamLoginSuccessful2, &bSteamLoginDone2]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bSteamLoginSuccessful2 = true;
			bSteamLoginDone2 = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone2](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone2 = true;
			}));

	WaitUntil(bSteamLoginDone2, "Waiting for Login...");

	if (!bSteamLoginSuccessful2)
	{
		return false;
	}

	const FString SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful2 = true;
			bDeleteDone2 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone2, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bSteamLoginSuccessful1);
	AB_TEST_TRUE(bSteamLoginSuccessful2);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_TRUE(bDeleteSuccessful2);
	AB_TEST_FALSE(FirstUserId.IsEmpty());
	AB_TEST_FALSE(SecondUserId.IsEmpty());
	AB_TEST_NOT_EQUAL(FirstUserId, SecondUserId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeSteamAccountSuccess, "AccelByte.Tests.AUser.UpgradeHeadlessSteamAccount", AutomationFlagMaskUser);
bool FUpgradeSteamAccountSuccess::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();
	const FString Email = TEXT("testSDKsteam@game.test");
	const FString Password = TEXT("123SDKTest123");

	//Setup: with deleting any user associated with this steam account
	bool bLoginPlatformSuccessful = false;
	bool bSteamLoginDone = false;
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

	FRegistry::User.ForgetAllCredentials();

	bLoginPlatformSuccessful = false;
	bSteamLoginDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");
	const FString FirstUserId = FRegistry::Credentials.GetUserId();
	const FString OldAccessToken = FRegistry::Credentials.GetAccessToken();

	AB_TEST_TRUE(bLoginPlatformSuccessful);


	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	FRegistry::User.Upgrade(Email, Password, THandler<FAccountUserData>::CreateLambda([&](const FAccountUserData& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bUpgradeSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bUpgradeSuccessful, "Waiting for Upgrade...");

	FRegistry::User.ForgetAllCredentials();

	bool bLoginEmailSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Email, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginEmailSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginEmailSuccessful, "Waiting for Login...");

	const FString RefreshedAccessToken = FRegistry::Credentials.GetAccessToken();

#pragma region DeleteUser1

	bDeleteDone1 = false;
	bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser1

	AB_TEST_EQUAL(FirstUserId, FRegistry::Credentials.GetUserId());
	AB_TEST_TRUE(bLoginPlatformSuccessful);
	AB_TEST_TRUE(bUpgradeSuccessful);
	AB_TEST_TRUE(bLoginEmailSuccessful);
	AB_TEST_FALSE(OldAccessToken.IsEmpty());
	AB_TEST_FALSE(RefreshedAccessToken.IsEmpty());
	AB_TEST_TRUE(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeSteamAccountv2Success, "AccelByte.Tests.AUser.UpgradeHeadlessSteamAccountv2", AutomationFlagMaskUser);
bool FUpgradeSteamAccountv2Success::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();
	const FString Email = TEXT("testSDKsteam@game.test");
	const FString Username = TEXT("testSDKsteam");
	const FString Password = TEXT("123SDKTest123");

	//Setup: with deleting any user associated with this steam account
	bool bLoginPlatformSuccessful = false;
	bool bSteamLoginDone = false;
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

	FRegistry::User.ForgetAllCredentials();

	bLoginPlatformSuccessful = false;
	bSteamLoginDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	const FString FirstUserId = FRegistry::Credentials.GetUserId();
	const FString OldAccessToken = FRegistry::Credentials.GetAccessToken();

	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	FRegistry::User.Upgradev2(Email, Username, Password, THandler<FAccountUserData>::CreateLambda([&](const FAccountUserData& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bUpgradeSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bUpgradeSuccessful, "Waiting for Upgrade...");

	FRegistry::User.ForgetAllCredentials();

	bool bLoginUsernameSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Username, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginUsernameSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginUsernameSuccessful, "Waiting for Login...");

	const FString RefreshedAccessToken = FRegistry::Credentials.GetAccessToken();

#pragma region DeleteUser1

	bDeleteDone1 = false;
	bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser1

	AB_TEST_EQUAL(FirstUserId, FRegistry::Credentials.GetUserId());
	AB_TEST_TRUE(bLoginPlatformSuccessful);
	AB_TEST_TRUE(bUpgradeSuccessful);
	AB_TEST_TRUE(bLoginUsernameSuccessful);
	AB_TEST_FALSE(OldAccessToken.IsEmpty());
	AB_TEST_FALSE(RefreshedAccessToken.IsEmpty());
	AB_TEST_TRUE(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLinkSteamAccountSuccess, "AccelByte.Tests.AUser.LinktoSteamAccount", AutomationFlagMaskUser);
bool FLinkSteamAccountSuccess::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();

	bool bLoginPlatformSuccessful = false;
	bool bSteamLoginDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

#pragma region DeleteUserSteam

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserSteam"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUserSteam

	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserData = false;
	FAccountUserData UserData;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FAccountUserData>::CreateLambda([&bGetUserData, &UserData](const FAccountUserData& Result)
		{
			UserData = Result;
			bGetUserData = true;
		}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
			{
				bGetUserData = true;
				UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
			}));

	WaitUntil(bGetUserData, "Waiting for Get User Data...");

	bool bLinkSteamAcc = false;
	bool bLinkSteamSuccess = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LinkSteamAcc"));
	FRegistry::User.LinkOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLinkSteamAcc, &bLinkSteamSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Link Success"));
			bLinkSteamSuccess = true;
			bLinkSteamAcc = true;
		}), FCustomErrorHandler::CreateLambda([&bLinkSteamAcc](int32 Code, const FString& Message, const FJsonObject& MessageVariables)
			{
				UE_LOG(LogAccelByteUserTest, Error, TEXT("Link Account Failed..! Error: %d | Message: %s | MsgVar: %s"), Code, *Message, *MessageVariables.GetStringField("publisherAccounts"));
				bLinkSteamAcc = true;
			}));

	WaitUntil(bLinkSteamAcc, "Waiting for Link Account...");

	bool bGetPlatformLinks = false;
	bool bGetPlatformLinksSuccess = false;
	FRegistry::User.GetPlatformLinks(THandler<FPagedPlatformLinks>::CreateLambda([&bGetPlatformLinks, &bGetPlatformLinksSuccess](const FPagedPlatformLinks& res)
		{
			bGetPlatformLinks = true;
			bGetPlatformLinksSuccess = true;
		}), FErrorHandler::CreateLambda([&bGetPlatformLinks](int32 Code, const FString& Message)
			{
				bGetPlatformLinks = true;
				UE_LOG(LogAccelByteUserTest, Error, TEXT("GetPlatformLinks..! Error: %d | Message: %s"), Code, *Message);
			}));

	WaitUntil(bGetPlatformLinks, "Waiting for GetPlatformLinks...");

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Logout"));
	FRegistry::User.ForgetAllCredentials();

	bLoginPlatformSuccessful = false;
	bSteamLoginDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	bGetUserData = false;
	FAccountUserData UserData2;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FAccountUserData>::CreateLambda([&bGetUserData, &UserData2](const FAccountUserData& Result)
		{
			UserData2 = Result;
			bGetUserData = true;
		}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
			{
				bGetUserData = true;
				UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
			}));

	WaitUntil(bGetUserData, "Waiting for Get User Data...");

#pragma region DeleteUser

	bDeleteDone1 = false;
	bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser

	AB_TEST_TRUE(bLoginPlatformSuccessful);
	AB_TEST_TRUE(bLinkSteamSuccess);
	AB_TEST_TRUE(bGetPlatformLinksSuccess);
	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_EQUAL(UserData.UserId, UserData2.UserId);
	AB_TEST_TRUE(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLinkSteamAccountConflict, "AccelByte.Tests.AUser.LinktoSteamAccountConflict", AutomationFlagMaskUser);
bool FLinkSteamAccountConflict::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();

	bool bLoginPlatformSuccessful = false;
	bool bSteamLoginDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserData = false;
	FAccountUserData UserData;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FAccountUserData>::CreateLambda([&bGetUserData, &UserData](const FAccountUserData& Result)
		{
			UserData = Result;
			bGetUserData = true;
		}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
			{
				bGetUserData = true;
				UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
			}));

	WaitUntil(bGetUserData, "Waiting for Get User Data...");

	bool bLinkSteamAcc = false;
	bool bLinkSteamSuccess = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LinkSteamAcc"));
	FRegistry::User.LinkOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLinkSteamAcc, &bLinkSteamSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Error, TEXT("Link Success"));
			bLinkSteamSuccess = true;
			bLinkSteamAcc = true;
		}), FCustomErrorHandler::CreateLambda([&bLinkSteamAcc](int32 Code, const FString& Message, const FJsonObject& MessageVariables)
			{
				FString MsgVar;
				MessageVariables.TryGetStringField("publisherAccounts", MsgVar);
				UE_LOG(LogAccelByteUserTest, Log, TEXT("Link Account Failed..! Error: %d | Message: %s | PlatformUserId: %s"), Code, *Message, *MsgVar);
				bLinkSteamAcc = true;
			}));

	WaitUntil(bLinkSteamAcc, "Waiting for Link Account...");

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Logout"));
	FRegistry::User.ForgetAllCredentials();

	bLoginPlatformSuccessful = false;
	bSteamLoginDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	bGetUserData = false;
	FAccountUserData UserData2;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FAccountUserData>::CreateLambda([&bGetUserData, &UserData2](const FAccountUserData& Result)
		{
			UserData2 = Result;
			bGetUserData = true;
		}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
			{
				bGetUserData = true;
				UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
			}));

	WaitUntil(bGetUserData, "Waiting for Get User Data...");

#pragma region DeleteUser

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser

	AB_TEST_TRUE(bLoginPlatformSuccessful);
	AB_TEST_FALSE(bLinkSteamSuccess);
	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_NOT_EQUAL(UserData.UserId, UserData2.UserId);
	AB_TEST_TRUE(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLinkSteamAccountForcedSuccess, "AccelByte.Tests.AUser.LinktoSteamAccountForced", AutomationFlagMaskUser);
bool FLinkSteamAccountForcedSuccess::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();

	bool bLoginPlatformSuccessful = false;
	bool bSteamLoginDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserData = false;
	FAccountUserData UserData;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FAccountUserData>::CreateLambda([&bGetUserData, &UserData](const FAccountUserData& Result)
		{
			UserData = Result;
			bGetUserData = true;
		}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
			{
				bGetUserData = true;
				UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
			}));

	WaitUntil(bGetUserData, "Waiting for Get User Data...");

	//STEAM_USER_ID env var is supposed to be the current user logged in to steam
	const FString SteamUserID = GetSteamUserId();

	bool bLinkSteamAcc = false;
	bool bLinkSteamSuccess = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LinkSteamAcc"));
	FRegistry::User.ForcedLinkOtherPlatform(EAccelBytePlatformType::Steam, SteamUserID, FVoidHandler::CreateLambda([&bLinkSteamAcc, &bLinkSteamSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Link Success"));
			bLinkSteamSuccess = true;
			bLinkSteamAcc = true;
		}), FCustomErrorHandler::CreateLambda([&bLinkSteamAcc](int32 Code, const FString& Message, const FJsonObject& MessageVariables)
			{
				UE_LOG(LogAccelByteUserTest, Error, TEXT("Link Account Failed..! Error: %d | Message: %s | MsgVar: %s"), Code, *Message, *MessageVariables.GetStringField("publisherAccounts"));
				bLinkSteamAcc = true;
			}));

	WaitUntil(bLinkSteamAcc, "Waiting for Link Account...");

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Logout"));
	FRegistry::User.ForgetAllCredentials();

	bLoginPlatformSuccessful = false;
	bSteamLoginDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bLoginPlatformSuccessful, &bSteamLoginDone]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginPlatformSuccessful = true;
			bSteamLoginDone = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone = true;
			}));

	WaitUntil(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	bGetUserData = false;
	FAccountUserData UserData2;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FAccountUserData>::CreateLambda([&bGetUserData, &UserData2](const FAccountUserData& Result)
		{
			UserData2 = Result;
			bGetUserData = true;
		}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
			{
				bGetUserData = true;
				UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
			}));

	WaitUntil(bGetUserData, "Waiting for Get User Data...");

#pragma region DeleteUser

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser

	AB_TEST_TRUE(bLoginPlatformSuccessful);
	AB_TEST_TRUE(bLinkSteamSuccess);
	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_EQUAL(UserData.UserId, UserData2.UserId);
	AB_TEST_TRUE(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetOtherPublicUserProfileTest, "AccelByte.Tests.AUserProfile.GetOtherPublicUserProfile", AutomationFlagMaskUser);
bool FGetOtherPublicUserProfileTest::RunTest(const FString& Parameter)
{
	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "Jirolu123";
	const FString Country = "ID";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString DOBString = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	FRegisterResponse RegisterResponse;

	FRegistry::User.Register(
		EmailAddress,
		Password,
		DisplayName,
		Country,
		DOBString,
		THandler<FRegisterResponse>::CreateLambda([&RegisterResponse, &bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
				bRegisterSuccessful = true;
				RegisterResponse = Result;
				bRegisterDone = true;
			}),
		FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	FAccelByteModelsUserProfileCreateRequest ProfileCreate;
	ProfileCreate.FirstName = "Jirolu";
	ProfileCreate.LastName = "Patmonem";
	ProfileCreate.Language = "en";
	ProfileCreate.Timezone = "Etc/UTC";
	ProfileCreate.DateOfBirth = "1970-01-01";
	ProfileCreate.AvatarUrl = "http://example.com/avatar/normal.jpg";
	FAccelByteModelsUserProfileInfo CreatedProfile;

	bool bCreateProfileDone = false;
	FRegistry::UserProfile.CreateUserProfile(
		ProfileCreate,
		THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bCreateProfileDone = true;
				CreatedProfile = Result;
			}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				bCreateProfileDone = true;

				if (Code != 2271)
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
				}
				else
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				}
			}));

	WaitUntil(bCreateProfileDone, "Waiting for Create Profile...");

	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FRegistry::User.LoginWithDeviceId(
		FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bDeviceLoginSuccessful = true;
			}),
		UserTestErrorHandler);

	WaitUntil(bDeviceLoginSuccessful, "Waiting for login...");

	bool bGetProfileSuccessful = false;
	FAccelByteModelsPublicUserProfileInfo OtherUserProfile;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetOtherUserProfile"));
	FRegistry::UserProfile.GetPublicUserProfileInfo(
		RegisterResponse.UserId,
		THandler<FAccelByteModelsPublicUserProfileInfo>::CreateLambda([&](const FAccelByteModelsPublicUserProfileInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bGetProfileSuccessful = true;
				OtherUserProfile = Result;
			}),
		UserTestErrorHandler);

	WaitUntil(bGetProfileSuccessful, "Waiting for Get Profile...");

	FRegistry::User.ForgetAllCredentials();

	bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	AdminDeleteUserProfile(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteProfileDone, "Waiting for Deletion...");
#pragma endregion DeleteUserProfile

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById
	AB_TEST_EQUAL(OtherUserProfile.AvatarSmallUrl, CreatedProfile.AvatarSmallUrl);
	AB_TEST_EQUAL(OtherUserProfile.AvatarUrl, CreatedProfile.AvatarUrl);
	AB_TEST_EQUAL(OtherUserProfile.AvatarLargeUrl, CreatedProfile.AvatarLargeUrl);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchGetPublicUserProfileInfos, "AccelByte.Tests.AUserProfile.BatchGetPublicUserProfileInfos", AutomationFlagMaskUser);
bool FBatchGetPublicUserProfileInfos::RunTest(const FString& Parameter)
{
#pragma region Test definitions

	const auto CreateUserProfile = [this](const FTestUser& InUser, const Credentials& InCredentials)
	{
		bool bIsDone = false;
		bool bIsOk = false;
		FAccelByteModelsUserProfileCreateRequest UserProfileCreateRequest;
		UserProfileCreateRequest.FirstName = InUser.FirstName;
		UserProfileCreateRequest.LastName = InUser.LastName;
		UserProfileCreateRequest.Language = InUser.Language;
		UserProfileCreateRequest.Timezone = InUser.Timezone;
		UserProfileCreateRequest.DateOfBirth = InUser.DateOfBirth;
		UserProfileCreateRequest.AvatarSmallUrl = InUser.AvatarSmallUrl;
		UserProfileCreateRequest.AvatarUrl = InUser.AvatarUrl;
		UserProfileCreateRequest.AvatarLargeUrl = InUser.AvatarLargeUrl;
		FAccelByteModelsUserProfileInfo UserProfileInfo;
		UE_LOG(LogAccelByteUserTest, Log, TEXT("%s: %s"), TEXT("Creating user profile"), *InUser.Email);
		Api::UserProfile UserProfileApi(InCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
		UserProfileApi.CreateUserProfile(UserProfileCreateRequest,
			THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
				{
					bIsOk = true;
					bIsDone = true;
					UserProfileInfo = Result;
				}),
			FErrorHandler::CreateLambda([&](int32 Code, FString Message)
				{
					if (Code == 11441) // Unable to createUserProfile: User profile already exists
					{
						bIsOk = true;
					}
					bIsDone = true;
				}));

		WaitUntil(bIsDone, TEXT("Waiting ..."));
		return bIsOk;
	};

	const auto BatchGetPublicUserProfileInfos = [this](const Credentials& InCredentials, const FString& InUserIds, TArray<FAccelByteModelsPublicUserProfileInfo>& OutResult) {
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteUserTest, Log, TEXT("%s: %s"), TEXT("Batch get user profiles"), *InUserIds);
		Api::UserProfile UserProfileApi(InCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
		UserProfileApi.BatchGetPublicUserProfileInfos(InUserIds,
			THandler<TArray<FAccelByteModelsPublicUserProfileInfo>>::CreateLambda([&](const TArray<FAccelByteModelsPublicUserProfileInfo>& Result)
				{
					OutResult = Result;
					bIsOk = true;
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 ErrorCode, const FString& ErrorMessage)
				{
					bIsDone = true;
				}));
		WaitUntil(bIsDone, TEXT("Waiting ..."));
		return bIsOk;
	};

#pragma endregion
	
	TArray<FTestUser> TestUsers;

	// Setup

	AB_TEST_TRUE(SetupTestUsers(2, TestUsers));

	// Create user profiles

	for (int i = 0; i < TestUsers.Num(); i++)
	{
		AB_TEST_TRUE(CreateUserProfile(TestUsers[i], TestUsers[i].Credentials));
	}

	// Batch get public user profile infos

	FString UserIdsCsv;

	for (const auto& TestUser : TestUsers)
	{
		UserIdsCsv.Append(FString::Printf(TEXT("%s%s"), UserIdsCsv.IsEmpty() ? TEXT("") : TEXT(","), *TestUser.Credentials.GetUserId()));
	}

	TArray<FAccelByteModelsPublicUserProfileInfo> UserPublicProfiles;

	AB_TEST_TRUE(BatchGetPublicUserProfileInfos(TestUsers[0].Credentials, UserIdsCsv, UserPublicProfiles)); // Using the first user credentials
	AB_TEST_EQUAL(UserPublicProfiles.Num(), TestUsers.Num());

	// Tear down

	AB_TEST_TRUE(TeardownTestUsers(TestUsers));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileUtilitiesSuccess, "AccelByte.Tests.AUserProfile.GetAndUpdateProfile", AutomationFlagMaskUser);
bool FUserProfileUtilitiesSuccess::RunTest(const FString& Parameter)
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

	bool bDeviceLoginSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeviceLoginSuccessful1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeviceLoginSuccessful1, "Waiting for Login...");

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

	WaitUntil(bCreateProfileSuccessful1, "Waiting for Create Profile...");

	bool bGetProfileSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile"));
	FRegistry::UserProfile.GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bGetProfileSuccessful1 = true;
		}), UserTestErrorHandler);

	WaitUntil(bGetProfileSuccessful1, "Waiting for Get Profile...");

	bool bUpdateProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpdateProfile"));
	FRegistry::UserProfile.UpdateUserProfile(
		ProfileUpdate,
		THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bUpdateProfileSuccessful = true;
			}),
		UserTestErrorHandler);

	WaitUntil(bUpdateProfileSuccessful, "Waiting for Update...");

	bool bGetProfileSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile // Second attempt"));
	FRegistry::UserProfile.GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bGetProfileSuccessful2 = true;
			UpdatedDateOfBirth = Result.DateOfBirth;
		}), UserTestErrorHandler);

	WaitUntil(bGetProfileSuccessful2, "Waiting for Get Profile...");

#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	AdminDeleteUserProfile(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteProfileDone, "Waiting for Deletion...");
#pragma endregion DeleteUserProfile

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeviceLoginSuccessful1);
	AB_TEST_TRUE(bGetProfileSuccessful1);
	AB_TEST_TRUE(bUpdateProfileSuccessful);
	AB_TEST_EQUAL(UpdatedDateOfBirth, ProfileUpdate.DateOfBirth);
	AB_TEST_TRUE(bGetProfileSuccessful2);
	AB_TEST_TRUE(bDeleteProfileSuccessful);
	AB_TEST_TRUE(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileCustomAttributesTest, "AccelByte.Tests.AUserProfile.UserProfileCustomAttributes", AutomationFlagMaskUser);
bool FUserProfileCustomAttributesTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeviceLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeviceLoginSuccessful, "Waiting for Login...");

	bool bCreateProfileSuccessful = false;
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

	FRegistry::UserProfile.CreateUserProfile(
		ProfileCreate,
		THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bCreateProfileSuccessful = true;
			}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				if (Code != 2271)
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
					bCreateProfileSuccessful = false;
				}
				else
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
					bCreateProfileSuccessful = true;
				}
			}));

	WaitUntil(bCreateProfileSuccessful, "Waiting for Create Profile...");

	bool bUpdateCustomAttributeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpdateCustomAttributeProfile"));

	int32 numberAttribute = 2;
	FString stringAttribute = "Five kuwi lima";
	bool bBooleanAttribute = true;
	FJsonObject customAttribute;
	customAttribute.SetNumberField("Two", numberAttribute);
	customAttribute.SetStringField("Five", stringAttribute);
	customAttribute.SetBoolField("True", bBooleanAttribute);
	FJsonObject updatedCustomAttribute;
	FRegistry::UserProfile.UpdateCustomAttributes(
		customAttribute,
		THandler<FJsonObject>::CreateLambda([&updatedCustomAttribute, &bUpdateCustomAttributeSuccessful](const FJsonObject& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				updatedCustomAttribute = Result;
				bUpdateCustomAttributeSuccessful = true;
			}), UserTestErrorHandler);

	WaitUntil(bUpdateCustomAttributeSuccessful, "Waiting for Update Custom Attributes...");

	bool bGetCustomAttributeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetCustomAttributeProfile"));

	int32 updatedNumberAttribute;
	updatedCustomAttribute.TryGetNumberField("Two", updatedNumberAttribute);
	FString updatedStringAttribute;
	updatedCustomAttribute.TryGetStringField("Five", updatedStringAttribute);
	bool updatedBooleanAttribute;
	updatedCustomAttribute.TryGetBoolField("True", updatedBooleanAttribute);

	FJsonObject getCustomAttribute;
	FRegistry::UserProfile.GetCustomAttributes(
		THandler<FJsonObject>::CreateLambda([&getCustomAttribute, &bGetCustomAttributeSuccessful](const FJsonObject& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				getCustomAttribute = Result;
				bGetCustomAttributeSuccessful = true;
			}), UserTestErrorHandler);

	WaitUntil(bGetCustomAttributeSuccessful, "Waiting for Get Custom Attributes...");

	int32 getNumberAttribute;
	getCustomAttribute.TryGetNumberField("Two", getNumberAttribute);
	FString getStringAttribute;
	getCustomAttribute.TryGetStringField("Five", getStringAttribute);
	bool getBooleanAttribute;
	getCustomAttribute.TryGetBoolField("True", getBooleanAttribute);

#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	AdminDeleteUserProfile(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteProfileDone, "Waiting for Deletion...");
#pragma endregion DeleteUserProfile

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeviceLoginSuccessful);
	AB_TEST_TRUE(bUpdateCustomAttributeSuccessful);
	AB_TEST_EQUAL(updatedNumberAttribute, numberAttribute);
	AB_TEST_EQUAL(updatedStringAttribute, stringAttribute);
	AB_TEST_EQUAL(updatedBooleanAttribute, bBooleanAttribute);
	AB_TEST_TRUE(bGetCustomAttributeSuccessful);
	AB_TEST_EQUAL(getNumberAttribute, numberAttribute);
	AB_TEST_EQUAL(getStringAttribute, stringAttribute);
	AB_TEST_EQUAL(getBooleanAttribute, bBooleanAttribute);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfilePublicCustomAttributesTest, "AccelByte.Tests.AUserProfile.UserProfilePublicCustomAttributes", AutomationFlagMaskUser);
bool FUserProfilePublicCustomAttributesTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FApiClientPtr User1Registry = FMultiRegistry::GetApiClient(TEXT("User1"));
	User1Registry->User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeviceLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeviceLoginSuccessful, "Waiting for Login...");

	bool bCreateProfileSuccessful = false;
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

	User1Registry->UserProfile.CreateUserProfile(
		ProfileCreate,
		THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bCreateProfileSuccessful = true;
			}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				if (Code != 2271)
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
					bCreateProfileSuccessful = false;
				}
				else
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
					bCreateProfileSuccessful = true;
				}
			}));

	WaitUntil(bCreateProfileSuccessful, "Waiting for Create Profile...");

	bool bUpdateCustomAttributeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpdateCustomAttributeProfile"));

	int32 numberAttribute = 2;
	FString stringAttribute = "Five kuwi lima";
	bool bBooleanAttribute = true;
	FJsonObject customAttribute;
	customAttribute.SetNumberField("Two", numberAttribute);
	customAttribute.SetStringField("Five", stringAttribute);
	customAttribute.SetBoolField("True", bBooleanAttribute);
	FJsonObject updatedCustomAttribute;
	User1Registry->UserProfile.UpdateCustomAttributes(
		customAttribute,
		THandler<FJsonObject>::CreateLambda([&updatedCustomAttribute, &bUpdateCustomAttributeSuccessful](const FJsonObject& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				updatedCustomAttribute = Result;
				bUpdateCustomAttributeSuccessful = true;
			}), UserTestErrorHandler);

	WaitUntil(bUpdateCustomAttributeSuccessful, "Waiting for Update Custom Attributes...");

	bool bGetCustomAttributeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetCustomAttributeProfile"));

	int32 updatedNumberAttribute;
	updatedCustomAttribute.TryGetNumberField("Two", updatedNumberAttribute);
	FString updatedStringAttribute;
	updatedCustomAttribute.TryGetStringField("Five", updatedStringAttribute);
	bool updatedBooleanAttribute;
	updatedCustomAttribute.TryGetBoolField("True", updatedBooleanAttribute);

	// New User
	FApiClientPtr User2Registry = FMultiRegistry::GetApiClient(TEXT("User2"));
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "1Old_Password1";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	User2Registry->User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	User2Registry->User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	FJsonObject getCustomAttribute;
	User2Registry->UserProfile.GetPublicCustomAttributes(User1Registry->CredentialsRef->GetUserId(),
		THandler<FJsonObject>::CreateLambda([&getCustomAttribute, &bGetCustomAttributeSuccessful](const FJsonObject& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				getCustomAttribute = Result;
				bGetCustomAttributeSuccessful = true;
			}), UserTestErrorHandler);

	WaitUntil(bGetCustomAttributeSuccessful, "Waiting for Get Custom Attributes...");

	int32 getNumberAttribute;
	getCustomAttribute.TryGetNumberField("Two", getNumberAttribute);
	FString getStringAttribute;
	getCustomAttribute.TryGetStringField("Five", getStringAttribute);
	bool getBooleanAttribute;
	getCustomAttribute.TryGetBoolField("True", getBooleanAttribute);

#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	AdminDeleteUserProfile(User1Registry->CredentialsRef->GetNamespace(), User1Registry->CredentialsRef->GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteProfileDone, "Waiting for Deletion...");
#pragma endregion DeleteUserProfile

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(User1Registry->CredentialsRef->GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	bool bDeleteUser2Done = false;
	bool bDeleteUser2Successful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(User2Registry->CredentialsRef->GetUserId(), FVoidHandler::CreateLambda([&bDeleteUser2Done, &bDeleteUser2Successful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteUser2Successful = true;
			bDeleteUser2Done = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteUser2Done, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeviceLoginSuccessful);
	AB_TEST_TRUE(bUpdateCustomAttributeSuccessful);
	AB_TEST_EQUAL(updatedNumberAttribute, numberAttribute);
	AB_TEST_EQUAL(updatedStringAttribute, stringAttribute);
	AB_TEST_EQUAL(updatedBooleanAttribute, bBooleanAttribute);
	AB_TEST_TRUE(bGetCustomAttributeSuccessful);
	AB_TEST_EQUAL(getNumberAttribute, numberAttribute);
	AB_TEST_EQUAL(getStringAttribute, stringAttribute);
	AB_TEST_EQUAL(getBooleanAttribute, bBooleanAttribute);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserBySteamUserIDTest, "AccelByte.Tests.AUser.GetUserBySteamUserID", AutomationFlagMaskUser);
bool FGetUserBySteamUserIDTest::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();

	bool bSteamLoginSuccessful1 = false;
	bool bSteamLoginDone1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bSteamLoginSuccessful1, &bSteamLoginDone1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bSteamLoginSuccessful1 = true;
			bSteamLoginDone1 = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone1](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone1 = true;
			}));

	WaitUntil(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

	FString FirstUserId = FRegistry::Credentials.GetUserId();

	//STEAM_USER_ID env var is supposed to be the current user logged in to steam
	const FString SteamUserID = GetSteamUserId();

	bool bGetUserDone = false;
	FAccountUserData ReceivedUserData;
	FRegistry::User.GetUserByOtherPlatformUserId(
		EAccelBytePlatformType::Steam,
		SteamUserID,
		THandler<FAccountUserData>::CreateLambda([&bGetUserDone, &ReceivedUserData](const FAccountUserData& UserData)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bGetUserDone = true;
				ReceivedUserData = UserData;
			}),
		UserTestErrorHandler);

	WaitUntil(bGetUserDone, "Waiting for Login...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bSteamLoginSuccessful1);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_FALSE(ReceivedUserData.UserId.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBulkGetUserBySteamUserIDTest, "AccelByte.Tests.AUser.BulkGetUserBySteamUserID", AutomationFlagMaskUser);
bool FBulkGetUserBySteamUserIDTest::RunTest(const FString& Parameter)
{
	if (!CheckSteamTicket())
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();

	bool bSteamLoginSuccessful1 = false;
	bool bSteamLoginDone1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithSteamAccount // First attempt"));
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&bSteamLoginSuccessful1, &bSteamLoginDone1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bSteamLoginSuccessful1 = true;
			bSteamLoginDone1 = true;
		}), FErrorHandler::CreateLambda([&bSteamLoginDone1](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bSteamLoginDone1 = true;
			}));

	WaitUntil(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

	const FString ABUserId = FRegistry::Credentials.GetUserId();

	//STEAM_USER_ID env var is supposed to be the current user logged in to steam
	const FString SteamUserID = GetSteamUserId();

	bool bGetUserDone = false;
	FBulkPlatformUserIdResponse ReceivedUserData;
	FRegistry::User.BulkGetUserByOtherPlatformUserIds(
		EAccelBytePlatformType::Steam,
		{ SteamUserID },
		THandler<FBulkPlatformUserIdResponse>::CreateLambda([&bGetUserDone, &ReceivedUserData](const FBulkPlatformUserIdResponse& UserData)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bGetUserDone = true;
				ReceivedUserData = UserData;
			}),
		UserTestErrorHandler);

	WaitUntil(bGetUserDone, "Waiting for Login...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bSteamLoginSuccessful1);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_NOT_EQUAL(ReceivedUserData.UserIdPlatforms.Num(), 0);
	AB_TEST_EQUAL(ReceivedUserData.UserIdPlatforms[0].UserId, ABUserId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserByDisplayNameTest, "AccelByte.Tests.AUser.GetUserByDisplayName", AutomationFlagMaskUser);
bool FGetUserByDisplayNameTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	// Complete DisplayName.
	bool bGetUserDone = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(
		DisplayName,
		THandler<FPagedPublicUsersInfo>::CreateLambda([&bGetUserDone, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				ReceivedUserData = Result;
				for (auto Data : ReceivedUserData.Data)
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("Get User, DisplayName: %s, UserId: %s"), *Data.DisplayName, *Data.UserId);
				}
				bGetUserDone = true;
			}),
		UserTestErrorHandler);

	WaitUntil(bGetUserDone, "Waiting for Search Users...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_TRUE(bGetUserDone);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_NOT_EQUAL(ReceivedUserData.Data.Num(), 0);
	const bool bDisplayNameFound = ReceivedUserData.Data.ContainsByPredicate([DisplayName](const FPublicUserInfo& Item)
		{
			return Item.DisplayName == DisplayName;
		});
	AB_TEST_TRUE(bDisplayNameFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserByUsernameTest, "AccelByte.Tests.AUser.GetUserByUsername", AutomationFlagMaskUser);
bool FGetUserByUsernameTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString Username = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + Username + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Registerv2(EmailAddress, Username, Password, "", Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Username, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	// Complete username.
	bool bGetUserDone = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(
		Username,
		THandler<FPagedPublicUsersInfo>::CreateLambda([&bGetUserDone, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				ReceivedUserData = Result;
				for (auto Data : ReceivedUserData.Data)
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("Get User, UserName: %s, UserId: %s"), *Data.UserName, *Data.UserId);
				}
				bGetUserDone = true;
			}),
		UserTestErrorHandler);

	WaitUntil(bGetUserDone, "Waiting for Search Users...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_TRUE(bGetUserDone);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_EQUAL(ReceivedUserData.Data.Num(), 1);
	AB_TEST_EQUAL(ReceivedUserData.Data[0].UserName, Username);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserFilterByUsernameTest, "AccelByte.Tests.AUser.GetUserFilterByUsername", AutomationFlagMaskUser);
bool FGetUserFilterByUsernameTest::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString SearchQuery = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString Username = SearchQuery;
	FString EmailAddress = "test+u4esdk+" + Username + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	FString UserId1 = "";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Registerv2(EmailAddress, Username, Password, "", Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone, &UserId1](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
			UserId1 = Result.UserId;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Error, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	FString Username2 = FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress2 = SearchQuery + "@game.test";
	FString DisplayName = SearchQuery;
	bRegisterSuccessful = false;
	bRegisterDone = false;
	FString UserId2 = "";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount2"));
	FRegistry::User.Registerv2(EmailAddress2, Username2, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone, &UserId2](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
			UserId2 = Result.UserId;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Error, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Username, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserDone = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(
		SearchQuery,
		EAccelByteSearchType::USERNAME,
		THandler<FPagedPublicUsersInfo>::CreateLambda([&bGetUserDone, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bGetUserDone = true;
				ReceivedUserData = Result;
			}),
		UserTestErrorHandler);

	WaitUntil(bGetUserDone, "Waiting for Search Users...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(UserId1, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	bDeleteDone = false;
	bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById2"));
	AdminDeleteUser(UserId2, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_TRUE(bGetUserDone);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_EQUAL(ReceivedUserData.Data.Num(), 1);
	AB_TEST_EQUAL(ReceivedUserData.Data[0].UserId, UserId1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetCountryFromIP, "AccelByte.Tests.AUser.GetCountryFromIP", AutomationFlagMaskUser);
bool FGetCountryFromIP::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();

	bool bGetCountryDone = false;
	FCountryInfo ReceivedCountryInfo;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Get country from IP"));
	FRegistry::User.GetCountryFromIP(
		THandler<FCountryInfo>::CreateLambda([&bGetCountryDone, &ReceivedCountryInfo](const FCountryInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bGetCountryDone = true;
				ReceivedCountryInfo = Result;

				UE_LOG(LogAccelByteUserTest, Log, TEXT("\nCountry Code:%s\nCountry Name:%s\nState:%s\nCity:%s\n"),
					*ReceivedCountryInfo.CountryCode,
					*ReceivedCountryInfo.CountryName,
					*ReceivedCountryInfo.State,
					*ReceivedCountryInfo.City);
			}), UserTestErrorHandler);

	WaitUntil(bGetCountryDone, "Waiting for CountryInfo...");

	AB_TEST_TRUE(bGetCountryDone);
	AB_TEST_FALSE(ReceivedCountryInfo.CountryCode.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterThenVerifyByRequestVerification, "AccelByte.Tests.AUser.RegisterThenVerifyByRequestVerification", AutomationFlagMaskUser);
bool FRegisterThenVerifyByRequestVerification::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 22));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	FRegisterResponse RegisterResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Register User"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone, &RegisterResult](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Register User"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
			RegisterResult = Result;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");
	AB_TEST_TRUE(bRegisterSuccessful);

	bool bLoginSuccessful = false;
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&bLoginSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Login User"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccessful, "Waiting for Login...");
	AB_TEST_TRUE(bLoginSuccessful);

	bool bSuccessSendVerifCode = false;
	FRegistry::User.SendVerificationCode(FVoidHandler::CreateLambda([&bSuccessSendVerifCode]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Send Request Verification Code"));
			bSuccessSendVerifCode = true;
		}), UserTestErrorHandler);

	WaitUntil(bSuccessSendVerifCode, "Waiting send verification code");
	AB_TEST_TRUE(bSuccessSendVerifCode);

	const FString VerificationCode = GetVerificationCode(RegisterResult.UserId, EVerificationCode::accountRegistration);
	AB_TEST_FALSE(VerificationCode.IsEmpty());

	bool bSuccessVerifyUser = false;
	FRegistry::User.Verify(VerificationCode, FVoidHandler::CreateLambda([&bSuccessVerifyUser]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Verify User"));
			bSuccessVerifyUser = true;
		}), UserTestErrorHandler);
	WaitUntil(bSuccessVerifyUser, "Waiting verify user");
	AB_TEST_TRUE(bSuccessVerifyUser);

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Delete User"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");
	AB_TEST_TRUE(bDeleteSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpdateUserEmail, "AccelByte.Tests.AUser.UpdateUserEmail", AutomationFlagMaskUser);
bool FUpdateUserEmail::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString NewDisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString NewEmailAddress = "test+u4esdk+" + NewDisplayName + "@game.test";
	NewEmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 22));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccess = false;
	bool bRegisterDone = false;
	FRegisterResponse RegisterResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Register User"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccess, &bRegisterDone, &RegisterResult](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Register User"));
			bRegisterSuccess = true;
			bRegisterDone = true;
			RegisterResult = Result;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");
	AB_TEST_TRUE(bRegisterSuccess);

	bool bLoginSuccess = false;
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Login User"));
			bLoginSuccess = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bLoginSuccess);

	bool bSendChangeEmailSuccess = false;
	FRegistry::User.SendUpdateEmailVerificationCode(FVoidHandler::CreateLambda([&bSendChangeEmailSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Send Change Email Verification Request"));
			bSendChangeEmailSuccess = true;
		}), UserTestErrorHandler);

	WaitUntil(bSendChangeEmailSuccess, "Waiting for Send Verification Request...");
	AB_TEST_TRUE(bSendChangeEmailSuccess);

	const FString VerificationCode = GetVerificationCode(RegisterResult.UserId, EVerificationCode::updateEmail);
	AB_TEST_FALSE(VerificationCode.IsEmpty());

	FUpdateEmailRequest updateEmailRequest;
	updateEmailRequest.Code = VerificationCode;
	updateEmailRequest.EmailAddress = NewEmailAddress;
	bool bUpdateEmailSuccess = false;
	FRegistry::User.UpdateEmail(updateEmailRequest, FVoidHandler::CreateLambda([&bUpdateEmailSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Update New Users Email"));
			bUpdateEmailSuccess = true;
		}), UserTestErrorHandler);

	WaitUntil(bUpdateEmailSuccess, "Waiting for Update Email...");
	AB_TEST_TRUE(bUpdateEmailSuccess);

	const FString NewEmailVerificationCode = GetVerificationCode(RegisterResult.UserId, EVerificationCode::updateEmail);
	AB_TEST_FALSE(NewEmailVerificationCode.IsEmpty());

	bool bVerifyNewEmailSuccess = false;
	FRegistry::User.Verify(NewEmailVerificationCode, FVoidHandler::CreateLambda([&bVerifyNewEmailSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Verify New Email"));
			bVerifyNewEmailSuccess = true;
		}), UserTestErrorHandler);

	WaitUntil(bVerifyNewEmailSuccess, "Waiting for Verify on New Email...");
	AB_TEST_TRUE(bVerifyNewEmailSuccess);

	FRegistry::User.ForgetAllCredentials();

	bLoginSuccess = false;
	FRegistry::User.LoginWithUsername(NewEmailAddress, Password, FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Login New Email"));
			bLoginSuccess = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccess, "Waiting for Login New Email...");
	AB_TEST_TRUE(bLoginSuccess);

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Delete User"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");
	AB_TEST_TRUE(bDeleteSuccessful);

	FRegistry::User.ForgetAllCredentials();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpdateUserMultiFields, "AccelByte.Tests.AUser.UpdateUserMultiFields", AutomationFlagMaskUser);
bool FUpdateUserMultiFields::RunTest(const FString& Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 22));
	const FString FormatDOB = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccess = false;
	bool bRegisterDone = false;
	FRegisterResponse RegisterResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Register User"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, FormatDOB, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccess, &bRegisterDone, &RegisterResult](const FRegisterResponse& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Register User"));
		bRegisterSuccess = true;
		bRegisterDone = true;
		RegisterResult = Result;
	}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
		bRegisterDone = true;
	}));

	WaitUntil(bRegisterDone, "Waiting for Registered...");
	AB_TEST_TRUE(bRegisterSuccess);

	bool bLoginSuccess = false;
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Login User"));
			bLoginSuccess = true;
		}), UserTestErrorHandler);

	WaitUntil(bLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bLoginSuccess);

	const FString NewCountry = "ID";
	const FDateTime NewDateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 23));
	const FString NewFormatDOB = FString::Printf(TEXT("%04d-%02d-%02d"), NewDateOfBirth.GetYear(), NewDateOfBirth.GetMonth(), NewDateOfBirth.GetDay());
	FString NewDisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	FUserUpdateRequest UpdateUserRequest;
	UpdateUserRequest.Country = NewCountry;
	UpdateUserRequest.DateOfBirth = NewFormatDOB;
	UpdateUserRequest.DisplayName = NewDisplayName;
	bool bUpdateUserSuccess = false;
	FAccountUserData UpdateUserResult;
	FRegistry::User.UpdateUser(UpdateUserRequest, THandler<FAccountUserData>::CreateLambda([&bUpdateUserSuccess, &UpdateUserResult](const FAccountUserData& Response)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Update User Data"));
		UpdateUserResult = Response;
		bUpdateUserSuccess = true;
	}), 
	FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
	}));

	WaitUntil(bUpdateUserSuccess, "Waiting for update User Datas");
	AB_TEST_TRUE(bUpdateUserSuccess);

	bool bGetDataSuccessful = false;
	FAccountUserData GetDataResult;
	FRegistry::User.GetData(
	THandler<FAccountUserData>::CreateLambda([&](const FAccountUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bGetDataSuccessful = true;
		GetDataResult = Result;
	}),
	FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
	}));

	WaitUntil(bGetDataSuccessful, "Waiting for Get Data...");
	AB_TEST_TRUE(bGetDataSuccessful);

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Delete User"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), UserTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");
	AB_TEST_TRUE(bDeleteSuccessful);

	FRegistry::User.ForgetAllCredentials();

	return true;
}

void AwsCognitoAuthUserPassword(const FString& AwsRegion, const FString& AwsClientId, const FString& Username, const FString& Password, const THandler<FAwsCognitoAuthResult>& OnSuccess, const FErrorHandler& OnError)
{
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString::Printf(TEXT("https://cognito-idp.%s.amazonaws.com/"), *AwsRegion));
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), "application/x-amz-json-1.1");
	Request->SetHeader(TEXT("Accept"), TEXT("*/*"));
	Request->SetHeader(TEXT("X-Amz-Target"), TEXT("AWSCognitoIdentityProviderService.InitiateAuth"));
	Request->SetContentAsString(
		FString::Printf(TEXT(R"({"AuthFlow":"USER_PASSWORD_AUTH","AuthParameters": {"PASSWORD": "%s","USERNAME": "%s"},"ClientId": "%s"})"), *Password, *Username, *AwsClientId)
	);

	// handle error differently for AWS API
	FRegistry::HttpRetryScheduler.ProcessRequest(
		Request,
		FHttpRequestCompleteDelegate::CreateLambda(
		[OnSuccess, OnError]
		(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished)
		{
			if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				HandleHttpResultOk(Response, OnSuccess);
				return;
			}

			if (!bFinished)
			{
				OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::NetworkError), "Request not sent.");
				return;
			}

			FAwsErrorResult Result;
			FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);

			OnError.ExecuteIfBound(Response->GetResponseCode(), Result.Message);
		}),
		FPlatformTime::Seconds());
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithAwscognito, "AccelByte.Tests.AUser.LoginWithAwscognito", AutomationFlagMaskUser);
bool FLoginWithAwscognito::RunTest(const FString& Parameter)
{
	FString AwsRegion = FPlatformMisc::GetEnvironmentVariable(TEXT("AWS_COGNITO_REGION"));
	FString AwsClientId = FPlatformMisc::GetEnvironmentVariable(TEXT("AWS_COGNITO_CLIENT_ID"));
	FString Username = FPlatformMisc::GetEnvironmentVariable(TEXT("AWS_COGNITO_USERNAME"));
	FString Password = FPlatformMisc::GetEnvironmentVariable(TEXT("AWS_COGNITO_PASSWORD"));

	if (AwsRegion.IsEmpty() || AwsClientId.IsEmpty() || Username.IsEmpty() || Password.IsEmpty())
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("=== Skipping test `%s` because one of the environment variables AWS_COGNITO_REGION, AWS_COGNITO_CLIENT_ID, AWS_COGNITO_USERNAME, AWS_COGNITO_PASSWORD is undefined"), *TestName);
		return true;
	}

	bool bAwsAuthComplete = false;
	bool bAwsAuthSuccess = false;
	FString AccessToken;
	AwsCognitoAuthUserPassword(
		AwsRegion, AwsClientId, Username, Password,
		THandler<FAwsCognitoAuthResult>::CreateLambda(
			[&bAwsAuthComplete, &bAwsAuthSuccess, &AccessToken](const FAwsCognitoAuthResult& Result)
		{
			bAwsAuthSuccess = true;
			bAwsAuthComplete = true;
			AccessToken = Result.AuthenticationResult.AccessToken;
		}),
		FErrorHandler::CreateLambda(
			[&bAwsAuthComplete](int32 ErrorCode, const FString& ErrorMessage)
		{
			UE_LOG(LogAccelByteUserTest, Error, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
			bAwsAuthComplete = true;
		})
	);

	WaitUntil(bAwsAuthComplete, "AWS Cognito authenticate user password...");

	AB_TEST_TRUE(bAwsAuthSuccess);

	FRegistry::User.ForgetAllCredentials();

	bool bLoginWithAwsCognitoComplete = false;
	bool bLoginWithAwsCognitoSuccess = false;

	FRegistry::User.LoginWithOtherPlatform(
		EAccelBytePlatformType::AwsCognito,
		AccessToken,
		FVoidHandler::CreateLambda(
			[&bLoginWithAwsCognitoComplete, &bLoginWithAwsCognitoSuccess]()
		{
			bLoginWithAwsCognitoSuccess = true;
			bLoginWithAwsCognitoComplete = true;
		}),
		FErrorHandler::CreateLambda(
			[&bLoginWithAwsCognitoComplete](int32 ErrorCode, const FString& ErrorMessage)
		{
			UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
			bLoginWithAwsCognitoComplete = true;
		})
	);

	WaitUntil(bLoginWithAwsCognitoComplete, "Login with AWS Cognito...");

	AB_TEST_TRUE(bLoginWithAwsCognitoSuccess);

	return true;
}

FString GetVerificationCode(const FString& userId, EVerificationCode code)
{
	bool bGetVerificationCodeSuccess = false;
	FVerificationCode verificationCode;
	AdminGetUserVerificationCode(userId, THandler<FVerificationCode>::CreateLambda([&verificationCode, &bGetVerificationCodeSuccess](const FVerificationCode& result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Get Verification Code Success..!"));
			verificationCode = result;
			bGetVerificationCodeSuccess = true;
		}), UserTestErrorHandler);
	WaitUntil(bGetVerificationCodeSuccess, "Getting Verification Code...");

	switch (code)
	{
	case EVerificationCode::accountRegistration: return verificationCode.accountRegistration;
	case EVerificationCode::accountUpgrade: return verificationCode.accountUpgrade;
	case EVerificationCode::passwordReset: return verificationCode.passwordReset;
	case EVerificationCode::updateEmail: return verificationCode.updateEmail;
	}
	return FString("");
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBan_FeatureBan_TokenRefreshed, "AccelByte.Tests.AUser.Ban_FeatureBan_TokenRefreshed", AutomationFlagMaskUser);
bool FBan_FeatureBan_TokenRefreshed::RunTest(const FString& Parameter)
{
	//Create User and Login
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	const FString OldSessionId = FRegistry::Credentials.GetAccessToken();

	//Ban
	FBanRequest body =
	{
		EBanType::MATCHMAKING,
		"User Ban Test",
		(FDateTime::Now() + FTimespan::FromSeconds(180)).ToIso8601(),
		EBanReason::IMPERSONATION
	};

	bool bBanSuccessful = false;
	FString BanId;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("BanUser"));
	AdminBanUser(FRegistry::Credentials.GetUserId(), body, THandler<FBanResponse>::CreateLambda([&bBanSuccessful, &BanId](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			BanId = Result.BanId;
			bBanSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bBanSuccessful, "Waiting for Ban...");

	int RandomRequestsCount = 5;
	//Perform Several requests
	bool bSearchUserSuccessful = false;
	int SearchUserSuccessfulCount = 0;
	for (int i = 0; i < RandomRequestsCount; i++)
	{
		FPagedPublicUsersInfo GetDataResult;
		FRegistry::User.SearchUsers("",
			THandler<FPagedPublicUsersInfo>::CreateLambda([&](const FPagedPublicUsersInfo& Result)
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("SearchUsers On Ban Success"));
					SearchUserSuccessfulCount++;
					bSearchUserSuccessful = true;
					GetDataResult = Result;
				}),
			UserTestErrorHandler);

		FPlatformProcess::Sleep(0.2f);
		FTicker::GetCoreTicker().Tick(0.2f);
	}

	// wait session to refresh
	FString NewSessionId = FRegistry::Credentials.GetAccessToken();
	for (int i = 0; i < 20; i++)
	{
		FPlatformProcess::Sleep(0.5f);
		FTicker::GetCoreTicker().Tick(0.5f);

		NewSessionId = FRegistry::Credentials.GetAccessToken();

		if (OldSessionId != NewSessionId)
		{
			break;
		}
	}
	FlushHttpRequests();
	WaitUntil(bSearchUserSuccessful, "Waiting for Search User...");

	//Unban
	bool bUnbanSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UnbanUser"));
	AdminBanUserChangeStatus(FRegistry::Credentials.GetUserId(), BanId, false, THandler<FBanResponse>::CreateLambda([&bUnbanSuccessful](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("User Unbanned: %s"), *Result.UserId);
			bUnbanSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bUnbanSuccessful, "Waiting for Unban...");

	//Perform Several requests
	bool bSearchUserSuccessfulAfter = false;
	int SearchUserSuccessfulAfterCount = 0;
	FPagedPublicUsersInfo GetDataResultAfter;
	for (int i = 0; i < RandomRequestsCount; i++)
	{
		FRegistry::User.SearchUsers("",
			THandler<FPagedPublicUsersInfo>::CreateLambda([&](const FPagedPublicUsersInfo& Result)
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("SearchUsers On UnBan Success"));
					bSearchUserSuccessfulAfter = true;
					SearchUserSuccessfulAfterCount++;
					GetDataResultAfter = Result;
				}),
			UserTestErrorHandler);

		FPlatformProcess::Sleep(0.2f);
		FTicker::GetCoreTicker().Tick(0.2f);
	}

	// wait session to refresh
	FString NewSessionIdAfter = FRegistry::Credentials.GetAccessToken();
	for (int i = 0; i < 20; i++)
	{
		FPlatformProcess::Sleep(0.5f);
		FTicker::GetCoreTicker().Tick(0.5f);

		NewSessionIdAfter = FRegistry::Credentials.GetAccessToken();

		if (NewSessionId != NewSessionIdAfter)
		{
			break;
		}
	}
	FlushHttpRequests();
	WaitUntil(bSearchUserSuccessfulAfter, "Waiting for Search User...");

	//Enable Ban
	bool bEnableBanSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("EnablebanUser"));
	AdminBanUserChangeStatus(FRegistry::Credentials.GetUserId(), BanId, true, THandler<FBanResponse>::CreateLambda([&bEnableBanSuccessful](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			bEnableBanSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bEnableBanSuccessful, "Waiting for Enable Ban...");

	//Random request
	bool bSearchUserSuccessfulAfterEnable = false;
	FPagedPublicUsersInfo GetDataResultAfterEnable;
	int SearchUserSuccessfulAfterEnableCount = 0;
	for (int i = 0; i < RandomRequestsCount; i++)
	{
		FRegistry::User.SearchUsers("",
			THandler<FPagedPublicUsersInfo>::CreateLambda([&](const FPagedPublicUsersInfo& Result)
				{
					UE_LOG(LogAccelByteUserTest, Log, TEXT("SearchUsers On Enable Ban Success"));
					bSearchUserSuccessfulAfterEnable = true;
					SearchUserSuccessfulAfterEnableCount++;
					GetDataResultAfterEnable = Result;
				}),
			UserTestErrorHandler);

		FPlatformProcess::Sleep(0.2f);
		FTicker::GetCoreTicker().Tick(0.2f);
	}

	// wait session to refresh
	FString NewSessionIdAfterEnable = FRegistry::Credentials.GetAccessToken();
	for (int i = 0; i < 20; i++)
	{
		FPlatformProcess::Sleep(0.5f);
		FTicker::GetCoreTicker().Tick(0.5f);

		NewSessionIdAfterEnable = FRegistry::Credentials.GetAccessToken();

		if (NewSessionIdAfter != NewSessionIdAfterEnable)
		{
			break;
		}
	}
	FlushHttpRequests();
	WaitUntil(bSearchUserSuccessfulAfterEnable, "Waiting for Search User...");

	//Assert
	AB_TEST_TRUE(bSearchUserSuccessful);
	AB_TEST_EQUAL(SearchUserSuccessfulCount, RandomRequestsCount);
	AB_TEST_TRUE(bSearchUserSuccessfulAfter);
	AB_TEST_EQUAL(SearchUserSuccessfulAfterCount, RandomRequestsCount);
	AB_TEST_TRUE(bSearchUserSuccessfulAfterEnable);
	AB_TEST_EQUAL(SearchUserSuccessfulAfterEnableCount, RandomRequestsCount);
	AB_TEST_TRUE(OldSessionId != NewSessionId);
	AB_TEST_TRUE(NewSessionId != NewSessionIdAfter);
	AB_TEST_TRUE(NewSessionIdAfter != NewSessionIdAfterEnable);

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBan_AccountBan, "AccelByte.Tests.AUser.Ban_AccountBan", AutomationFlagMaskUser);
bool FBan_AccountBan::RunTest(const FString& Parameter)
{
	//Create User and Login
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	const FString OldSessionId = FRegistry::Credentials.GetAccessToken();
	const FString UserId = FRegistry::Credentials.GetUserId();

	//Ban
	FBanRequest body =
	{
		EBanType::LOGIN,
		"User Ban Test",
		(FDateTime::Now() + FTimespan::FromSeconds(180)).ToIso8601(),
		EBanReason::IMPERSONATION
	};

	bool bBanSuccessful = false;
	FString BanId;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("BanUser"));
	AdminBanUser(UserId, body, THandler<FBanResponse>::CreateLambda([&bBanSuccessful, &BanId](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			BanId = Result.BanId;
			bBanSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bBanSuccessful, "Waiting for Ban...");

	//Random request
	bool bSearchUserSuccessful = false;
	bool bSearchUserError = false;
	bool bSearchUserDone = false;
	FPagedPublicUsersInfo GetDataResult;
	int ErrorCode;
	FRegistry::User.SearchUsers("",
		THandler<FPagedPublicUsersInfo>::CreateLambda([&](const FPagedPublicUsersInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
				bSearchUserSuccessful = true;
				bSearchUserDone = true;
				GetDataResult = Result;
			}),
		FErrorHandler::CreateLambda([&bSearchUserDone, &bSearchUserError, &ErrorCode](int Code, const FString& Message)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				ErrorCode = Code;
				bSearchUserError = true;
				bSearchUserDone = true;
			}));

	// wait session to refresh
	FString NewSessionId = FRegistry::Credentials.GetAccessToken();
	for (int i = 0; i < 20; i++)
	{
		FPlatformProcess::Sleep(0.5f);
		FTicker::GetCoreTicker().Tick(0.5f);

		NewSessionId = FRegistry::Credentials.GetAccessToken();

		if (OldSessionId != NewSessionId)
		{
			break;
		}
	}
	FlushHttpRequests();
	WaitUntil(bSearchUserDone, "Waiting for Search User...");
	AB_TEST_TRUE(bSearchUserError);

	//try to relogin
	FRegistry::Credentials.ForgetAll();
	bLoginSuccessful = false;
	bool bLoginError = false;
	bool bLoginDone = false;
	int LoginErrorCode;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
			bLoginDone = true;
		}), FErrorHandler::CreateLambda([&bLoginError, &bLoginDone, &LoginErrorCode](int Code, const FString& Message)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				LoginErrorCode = Code;
				bLoginError = true;
				bLoginDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bLoginDone, "Waiting for Login...");
	AB_TEST_TRUE(bLoginError);

	//Unban
	bool bUnbanSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UnbanUser"));
	AdminBanUserChangeStatus(UserId, BanId, false, THandler<FBanResponse>::CreateLambda([&bUnbanSuccessful](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("User Unbanned: %s"), *Result.UserId);
			bUnbanSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bUnbanSuccessful, "Waiting for Unban...");

	//try to relogin
	FRegistry::Credentials.ForgetAll();
	bLoginSuccessful = false;
	bool bLoginErrorAfterUnban = false;
	bool bLoginDoneAfterUnban = false;
	int LoginErrorCodeAfterUnban;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
			bLoginDoneAfterUnban = true;
		}), FErrorHandler::CreateLambda([&bLoginErrorAfterUnban, &bLoginDoneAfterUnban, &LoginErrorCodeAfterUnban](int Code, const FString& Message)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				LoginErrorCodeAfterUnban = Code;
				bLoginErrorAfterUnban = true;
				bLoginDoneAfterUnban = true;
			}));

	FlushHttpRequests();
	WaitUntil(bLoginDoneAfterUnban, "Waiting for Login...");
	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_TRUE(!bLoginErrorAfterUnban);

	//Random request
	bool bSearchUserSuccessfulAfter = false;
	FPagedPublicUsersInfo GetDataResultAfter;
	FRegistry::User.SearchUsers("",
		THandler<FPagedPublicUsersInfo>::CreateLambda([&](const FPagedPublicUsersInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
				bSearchUserSuccessfulAfter = true;
				GetDataResultAfter = Result;
			}),
		UserTestErrorHandler);

	// wait session to refresh
	FString NewSessionIdAfter = FRegistry::Credentials.GetAccessToken();
	for (int i = 0; i < 20; i++)
	{
		FPlatformProcess::Sleep(0.5f);
		FTicker::GetCoreTicker().Tick(0.5f);

		NewSessionIdAfter = FRegistry::Credentials.GetAccessToken();

		if (NewSessionId != NewSessionIdAfter)
		{
			break;
		}
	}
	FlushHttpRequests();
	WaitUntil(bSearchUserSuccessfulAfter, "Waiting for Search User...");

	//Enable Ban
	bool bEnableBanSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("EnablebanUser"));
	AdminBanUserChangeStatus(UserId, BanId, true, THandler<FBanResponse>::CreateLambda([&bEnableBanSuccessful](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			bEnableBanSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bEnableBanSuccessful, "Waiting for Enable Ban...");

	//Random request
	bool bSearchUserSuccessfulAfterEnable = false;
	bool bSearchUserDoneAfterEnable = false;
	bool bSearchUserErrorAfterEnable = false;
	int ErrorCodeAfterEnable;
	FPagedPublicUsersInfo GetDataResultAfterEnable;
	FRegistry::User.SearchUsers("",
		THandler<FPagedPublicUsersInfo>::CreateLambda([&](const FPagedPublicUsersInfo& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
				bSearchUserSuccessfulAfterEnable = true;
				bSearchUserDoneAfterEnable = true;
				GetDataResultAfterEnable = Result;
			}),
		FErrorHandler::CreateLambda([&bSearchUserDoneAfterEnable, &bSearchUserErrorAfterEnable, &ErrorCodeAfterEnable](int Code, const FString& Message)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				ErrorCodeAfterEnable = Code;
				bSearchUserErrorAfterEnable = true;
				bSearchUserDoneAfterEnable = true;
			}));

	// wait session to refresh
	FString NewSessionIdAfterEnable = FRegistry::Credentials.GetAccessToken();
	for (int i = 0; i < 20; i++)
	{
		FPlatformProcess::Sleep(0.5f);
		FTicker::GetCoreTicker().Tick(0.5f);

		NewSessionIdAfterEnable = FRegistry::Credentials.GetAccessToken();

		if (NewSessionIdAfter != NewSessionIdAfterEnable)
		{
			break;
		}
	}
	FlushHttpRequests();
	WaitUntil(bSearchUserDoneAfterEnable, "Waiting for Search User...");
	AB_TEST_TRUE(bSearchUserErrorAfterEnable);

	//try to relogin
	FRegistry::Credentials.ForgetAll();
	bLoginSuccessful = false;
	bool bLoginErrorAfterEnable = false;
	bool bLoginDoneAfterEnable = false;
	int LoginErrorCodeAfterEnable;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
			bLoginDoneAfterEnable = true;
		}), FErrorHandler::CreateLambda([&bLoginErrorAfterEnable, &bLoginDoneAfterEnable, &LoginErrorCodeAfterEnable](int Code, const FString& Message)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				LoginErrorCodeAfterEnable = Code;
				bLoginErrorAfterEnable = true;
				bLoginDoneAfterEnable = true;
			}));

	FlushHttpRequests();
	WaitUntil(bLoginDoneAfterEnable, "Waiting for Login...");
	AB_TEST_TRUE(bLoginError);

	//Assert
	AB_TEST_TRUE(!bSearchUserSuccessful);
	AB_TEST_TRUE(bSearchUserSuccessfulAfter);
	AB_TEST_TRUE(!bSearchUserSuccessfulAfterEnable);

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(UserId, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBulkUserInfo_GetBulkUserInfoSuccess, "AccelByte.Tests.AUser.BulkUserInfo_GetBulkUserInfoSuccess", AutomationFlagMaskUser);
bool FBulkUserInfo_GetBulkUserInfoSuccess::RunTest(const FString& Parameter)
{
	//Create some users
	const int UserCount = 3;
	TArray<FString> UserIds;
	TArray<FRegisterResponse> RegisteredUsers;
	TArray<FAccelByteModelsUserProfileInfo> RegisteredUserProfiles;
	for (int i = 0; i < UserCount; i++)
	{
		FRegistry::User.ForgetAllCredentials();
		FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
		FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
		EmailAddress.ToLowerInline();
		FString Password = "123SDKTest123";
		const FString Country = "US";
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
		const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		bool bRegisterSuccessful = false;
		bool bRegisterDone = false;
		UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
		FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone, &UserIds, &RegisteredUsers](const FRegisterResponse& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
				UserIds.Add(Result.UserId);
				RegisteredUsers.Add(Result);
				bRegisterSuccessful = true;
				bRegisterDone = true;
			}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
				{
					UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
					bRegisterDone = true;
				}));

		FlushHttpRequests();
		WaitUntil(bRegisterDone, "Waiting for Registered...");

		if (!bRegisterSuccessful)
		{
			return false;
		}
	}
	
	//Create Primary User and Login
	FString PrimaryUserId;
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = ("abBulkUserInfo" + FGuid::NewGuid().ToString(EGuidFormats::Digits)).LeftChop(20);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone, &PrimaryUserId](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			PrimaryUserId = Result.UserId;
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bBulkUserInfoSuccess = false;
	FListBulkUserInfo BulkUserInfoResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("BulkUserInfo"));
	FRegistry::User.BulkGetUserInfo(UserIds, THandler<FListBulkUserInfo>::CreateLambda([&bBulkUserInfoSuccess, &BulkUserInfoResult](const FListBulkUserInfo& Result) 
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			BulkUserInfoResult = Result;
			bBulkUserInfoSuccess = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bBulkUserInfoSuccess, "Waiting for Bulk UserInfo...");

	bool bIsDisplayNameExpected = true;
	for (auto UserInfo : BulkUserInfoResult.Data)
	{
		for (auto RegisteredUser : RegisteredUsers)
		{
			if (UserInfo.UserId == RegisteredUser.UserId)
			{
				if (UserInfo.DisplayName != RegisteredUser.DisplayName)
				{
					bIsDisplayNameExpected = false;
					break;
				}
				break;
			}
		}
	}

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(PrimaryUserId, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	for (int i = 0; i < UserCount; i++)
	{
		bDeleteDone = false;
		bDeleteSuccessful = false;
		UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
		AdminDeleteUser(UserIds[i], FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bDeleteSuccessful = true;
				bDeleteDone = true;
			}), UserTestErrorHandler);

		FlushHttpRequests();
		WaitUntil(bDeleteDone, "Waiting for Deletion...");
	}

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_TRUE(BulkUserInfoResult.Data.Num() == UserCount);
	AB_TEST_TRUE(bIsDisplayNameExpected);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBulkUserInfo_EmptyUserIds, "AccelByte.Tests.AUser.BulkUserInfo_EmptyUserIds", AutomationFlagMaskUser);
bool FBulkUserInfo_EmptyUserIds::RunTest(const FString& Parameter)
{
	//Create Primary User and Login
	FString PrimaryUserId;
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = ("abBulkUserInfo" + FGuid::NewGuid().ToString(EGuidFormats::Digits)).LeftChop(20);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone, &PrimaryUserId](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			PrimaryUserId = Result.UserId;
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bBulkUserInfoSuccess = false;
	bool bBulkUserInfoDone = false;
	ErrorCodes BulkUserInfoError;
	FListBulkUserInfo BulkUserInfoResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("BulkUserInfo"));
	FRegistry::User.BulkGetUserInfo(TArray<FString>{}, THandler<FListBulkUserInfo>::CreateLambda([&bBulkUserInfoSuccess, &BulkUserInfoResult, &bBulkUserInfoDone](const FListBulkUserInfo& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			BulkUserInfoResult = Result;
			bBulkUserInfoSuccess = true;
			bBulkUserInfoDone = true;
		}), FErrorHandler::CreateLambda([&bBulkUserInfoDone, &BulkUserInfoError](int32 Code, const FString& Message) 
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Failed, Code: %d | Message: %s"), Code, *Message);
				BulkUserInfoError = (ErrorCodes)Code;
				bBulkUserInfoDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bBulkUserInfoDone, "Waiting for Bulk UserInfo...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(PrimaryUserId, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_FALSE(bBulkUserInfoSuccess);
	AB_TEST_TRUE(BulkUserInfoError == ErrorCodes::InvalidRequest);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBulkUserInfo_InvalidUserIds, "AccelByte.Tests.AUser.BulkUserInfo_InvalidUserIds", AutomationFlagMaskUser);
bool FBulkUserInfo_InvalidUserIds::RunTest(const FString& Parameter)
{
	//Create Primary User and Login
	FString PrimaryUserId;
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = ("abBulkUserInfo" + FGuid::NewGuid().ToString(EGuidFormats::Digits)).LeftChop(20);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount"));
	FRegistry::User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone, &PrimaryUserId](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			PrimaryUserId = Result.UserId;
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteUserTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	bool bBulkUserInfoSuccess = false;
	FListBulkUserInfo BulkUserInfoResult;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("BulkUserInfo"));
	FRegistry::User.BulkGetUserInfo(TArray<FString>{"Invalid"}, THandler<FListBulkUserInfo>::CreateLambda([&bBulkUserInfoSuccess, &BulkUserInfoResult](const FListBulkUserInfo& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			BulkUserInfoResult = Result;
			bBulkUserInfoSuccess = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bBulkUserInfoSuccess, "Waiting for Bulk UserInfo...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(PrimaryUserId, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_TRUE(bBulkUserInfoSuccess);
	AB_TEST_TRUE(BulkUserInfoResult.Data.Num() == 0);
	return true;
}