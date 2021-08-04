// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Models/AccelByteUserProfileModels.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteEnvironment.h"
#include "HAL/FileManager.h"
#include "Api/AccelByteUserProfileApi.h"
#include "TestUtilities.h"
#include "TestModels.h"
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

	Waiting(bRegisterDone, "Waiting for Registered...");

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bLoginSuccessful, "Waiting for Login...");

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
		10,
			"Wait refresh token");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bVerifyUserSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("VerifyingAccount"));
	FRegistry::User.Verify(VerificationCode, FVoidHandler::CreateLambda([&bVerifyUserSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
			bVerifyUserSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bVerifyUserSuccessful, "Waiting for Verfying Account...");

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

	Waiting(bGetDataSuccessful, "Waiting for Get Data...");

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

	AB_TEST_TRUE(bGetDataSuccessful);
	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_EQUAL(RegisterResult.DisplayName, GetDataResult.DisplayName);
	AB_TEST_EQUAL(RegisterResult.UserId, GetDataResult.UserId);
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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bForgotPasswordSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("RequestPasswordReset"));

	FRegistry::User.SendResetPasswordCode(EmailAddress, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bForgotPasswordSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bForgotPasswordSuccessful, "Waiting for Code to be sent...");

	bool bGetVerificationCodeSuccess = false;
	FString VerificationCode;
	User_Get_Verification_Code(FRegistry::Credentials.GetUserId(), THandler<FVerificationCode>::CreateLambda([&VerificationCode, &bGetVerificationCodeSuccess](const FVerificationCode& result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Get Verification Code Success..!"));
			VerificationCode = result.passwordReset;
			bGetVerificationCodeSuccess = true;
		}), UserTestErrorHandler);
	Waiting(bGetVerificationCodeSuccess, "Getting Verification Code...");

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	Password = "1New_Password1";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("ResetPassword"));

	FRegistry::User.ResetPassword(VerificationCode, EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bResetPasswordSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bResetPasswordSuccessful, "Waiting for Reset...");

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Logout"));
	FRegistry::User.ForgetAllCredentials();

	bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));

	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bDeviceLoginSuccessful1, "Waiting for Login...");

	const FString FirstUserId = FRegistry::Credentials.GetUserId();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId //Second attempt"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    User ID: %s"), *FRegistry::Credentials.GetUserId());
			bDeviceLoginSuccessful2 = true;
		}), UserTestErrorHandler);
	Waiting(bDeviceLoginSuccessful2, "Waiting for Login...");

	const FString SecondUserId = FRegistry::Credentials.GetUserId();

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Delete...");

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

	Waiting(bDeviceLoginSuccessful, "Waiting for Login with device id...");

	FString UserId = FRegistry::Credentials.GetUserId();
	bool bLogoutSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("Logout"));
	FRegistry::User.Logout(FVoidHandler::CreateLambda([&bLogoutSuccessful]()
	{
		bLogoutSuccessful = true;
	}), UserTestErrorHandler);
	Waiting(bLogoutSuccessful, "Waiting for Logout...");

#pragma region DeleteUserById

	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(UserId, FVoidHandler::CreateLambda([&bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
	}), UserTestErrorHandler);

	Waiting(bDeleteSuccessful, "Waiting for Delete...");

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

	Waiting(bDeviceLoginSuccessful1, "Waiting for Login...");

	const FString FirstUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	FRegistry::User.ForgetAllCredentials();

	bool bDeviceLoginSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId // Second attempt"))
		FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				bDeviceLoginSuccessful2 = true;
			}), UserTestErrorHandler);

	Waiting(bDeviceLoginSuccessful2, "Waiting for Login...");

	const FString SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful2 = true;
			bDeleteDone2 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone2, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone1, "Waiting for Login...");

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

	Waiting(bSteamLoginDone2, "Waiting for Login...");

	if (!bSteamLoginSuccessful2)
	{
		return false;
	}

	const FString SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

	const FString FirstUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone2, "Waiting for Login...");

	if (!bSteamLoginSuccessful2)
	{
		return false;
	}

	const FString SecondUserId = FRegistry::Credentials.GetUserId();

#pragma region DeleteUserById

	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone2, &bDeleteSuccessful2]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful2 = true;
			bDeleteDone2 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone2, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");
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

	Waiting(bUpgradeSuccessful, "Waiting for Upgrade...");

	FRegistry::User.ForgetAllCredentials();

	bool bLoginEmailSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Email, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginEmailSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bLoginEmailSuccessful, "Waiting for Login...");

	const FString RefreshedAccessToken = FRegistry::Credentials.GetAccessToken();

#pragma region DeleteUser1

	bDeleteDone1 = false;
	bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

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

	Waiting(bUpgradeSuccessful, "Waiting for Upgrade...");

	FRegistry::User.ForgetAllCredentials();

	bool bLoginUsernameSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Username, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginUsernameSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bLoginUsernameSuccessful, "Waiting for Login...");

	const FString RefreshedAccessToken = FRegistry::Credentials.GetAccessToken();

#pragma region DeleteUser1

	bDeleteDone1 = false;
	bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser1"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

#pragma region DeleteUserSteam

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserSteam"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

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

	Waiting(bGetUserData, "Waiting for Get User Data...");

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

	Waiting(bLinkSteamAcc, "Waiting for Link Account...");

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

	Waiting(bGetPlatformLinks, "Waiting for GetPlatformLinks...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

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

	Waiting(bGetUserData, "Waiting for Get User Data...");

#pragma region DeleteUser

	bDeleteDone1 = false;
	bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

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

	Waiting(bGetUserData, "Waiting for Get User Data...");

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

	Waiting(bLinkSteamAcc, "Waiting for Link Account...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

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

	Waiting(bGetUserData, "Waiting for Get User Data...");

#pragma region DeleteUser

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

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

	Waiting(bGetUserData, "Waiting for Get User Data...");

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

	Waiting(bLinkSteamAcc, "Waiting for Link Account...");

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

	Waiting(bSteamLoginDone, "Waiting for Login...");

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

	Waiting(bGetUserData, "Waiting for Get User Data...");

#pragma region DeleteUser

	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUser"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone1, &bDeleteSuccessful1]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful1 = true;
			bDeleteDone1 = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone1, "Waiting for Deletion...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

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

	Waiting(bCreateProfileDone, "Waiting for Create Profile...");

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

	Waiting(bDeviceLoginSuccessful, "Waiting for login...");

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

	Waiting(bGetProfileSuccessful, "Waiting for Get Profile...");

	FRegistry::User.ForgetAllCredentials();

	bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bLoginSuccessful, "Waiting for Login...");

#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	DeleteUserProfile(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), UserTestErrorHandler);

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

		Waiting(bIsDone, TEXT("Waiting ..."));
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
		Waiting(bIsDone, TEXT("Waiting ..."));
		return bIsOk;
	};

#pragma endregion

	const FString TestUID = TEXT("29008abd"); // Arbitrary unique id to identify this specific automation test

	TArray<TSharedPtr<FTestUser>> TestUsers;
	TArray<TSharedPtr<Credentials>> TestCredentials;

	// Setup

	AB_TEST_TRUE(SetupTestUsers(TestUID, 2, TestUsers, TestCredentials));

	// Create user profiles

	for (int i = 0; i < TestUsers.Num(); i++)
	{
		AB_TEST_TRUE(CreateUserProfile(*TestUsers[i], *TestCredentials[i]));
	}

	// Batch get public user profile infos

	FString UserIdsCsv;

	for (const auto& Credentials : TestCredentials)
	{
		UserIdsCsv.Append(FString::Printf(TEXT("%s%s"), UserIdsCsv.IsEmpty() ? TEXT("") : TEXT(","), *Credentials->GetUserId()));
	}

	TArray<FAccelByteModelsPublicUserProfileInfo> UserPublicProfiles;

	AB_TEST_TRUE(BatchGetPublicUserProfileInfos(*TestCredentials[0], UserIdsCsv, UserPublicProfiles)); // Using the first user credentials
	AB_TEST_EQUAL(UserPublicProfiles.Num(), TestUsers.Num());

	// Tear down

	AB_TEST_TRUE(TearDownTestUsers(TestCredentials));

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

	Waiting(bCreateProfileSuccessful1, "Waiting for Create Profile...");

	bool bGetProfileSuccessful1 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile"));
	FRegistry::UserProfile.GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bGetProfileSuccessful1 = true;
		}), UserTestErrorHandler);

	Waiting(bGetProfileSuccessful1, "Waiting for Get Profile...");

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

	Waiting(bUpdateProfileSuccessful, "Waiting for Update...");

	bool bGetProfileSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile // Second attempt"));
	FRegistry::UserProfile.GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bGetProfileSuccessful2 = true;
			UpdatedDateOfBirth = Result.DateOfBirth;
		}), UserTestErrorHandler);

	Waiting(bGetProfileSuccessful2, "Waiting for Get Profile...");

#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	DeleteUserProfile(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), UserTestErrorHandler);

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bDeviceLoginSuccessful, "Waiting for Login...");

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

	Waiting(bCreateProfileSuccessful, "Waiting for Create Profile...");

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

	Waiting(bUpdateCustomAttributeSuccessful, "Waiting for Update Custom Attributes...");

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

	Waiting(bGetCustomAttributeSuccessful, "Waiting for Get Custom Attributes...");

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
	DeleteUserProfile(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), UserTestErrorHandler);

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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
	TSharedPtr<FApiClient> User1Registry = FMultiRegistry::GetApiClient(TEXT("User1"));
	User1Registry->User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeviceLoginSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bDeviceLoginSuccessful, "Waiting for Login...");

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

	Waiting(bCreateProfileSuccessful, "Waiting for Create Profile...");

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

	Waiting(bUpdateCustomAttributeSuccessful, "Waiting for Update Custom Attributes...");

	bool bGetCustomAttributeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetCustomAttributeProfile"));

	int32 updatedNumberAttribute;
	updatedCustomAttribute.TryGetNumberField("Two", updatedNumberAttribute);
	FString updatedStringAttribute;
	updatedCustomAttribute.TryGetStringField("Five", updatedStringAttribute);
	bool updatedBooleanAttribute;
	updatedCustomAttribute.TryGetBoolField("True", updatedBooleanAttribute);

	// New User
	TSharedPtr<FApiClient> User2Registry = FMultiRegistry::GetApiClient(TEXT("User2"));
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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

	FJsonObject getCustomAttribute;
	User2Registry->UserProfile.GetPublicCustomAttributes(User1Registry->Credentials.GetUserId(),
		THandler<FJsonObject>::CreateLambda([&getCustomAttribute, &bGetCustomAttributeSuccessful](const FJsonObject& Result)
			{
				UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
				getCustomAttribute = Result;
				bGetCustomAttributeSuccessful = true;
			}), UserTestErrorHandler);

	Waiting(bGetCustomAttributeSuccessful, "Waiting for Get Custom Attributes...");

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
	DeleteUserProfile(User1Registry->Credentials.GetNamespace(), User1Registry->Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteProfileDone, "Waiting for Deletion...");
#pragma endregion DeleteUserProfile

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(User1Registry->Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

	bool bDeleteUser2Done = false;
	bool bDeleteUser2Successful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(User2Registry->Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteUser2Done, &bDeleteUser2Successful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteUser2Successful = true;
			bDeleteUser2Done = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteUser2Done, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone1, "Waiting for Login...");

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bSteamLoginDone1, "Waiting for Login...");

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
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bSteamLoginSuccessful1);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_NOT_EQUAL(ReceivedUserData.UserIdPlatforms.Num(), 0);
	AB_TEST_EQUAL(ReceivedUserData.UserIdPlatforms[0].UserId, ABUserId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserByEmailAddressTest, "AccelByte.Tests.AUser.GetUserByEmailAddress", AutomationFlagMaskUser);
bool FGetUserByEmailAddressTest::RunTest(const FString& Parameter)
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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

	// Complete email.
	bool bGetUserDone = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(
		EmailAddress,
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

	Waiting(bGetUserDone, "Waiting for Search Users...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	AB_TEST_TRUE(bLoginSuccessful);
	AB_TEST_TRUE(bGetUserDone);
	AB_TEST_TRUE(bDeleteSuccessful);
	AB_TEST_EQUAL(ReceivedUserData.Data.Num(), 1);
	AB_TEST_EQUAL(ReceivedUserData.Data[0].DisplayName, DisplayName);
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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

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

	Waiting(bGetUserDone, "Waiting for Search Users...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

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

	Waiting(bGetUserDone, "Waiting for Search Users...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");

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

	Waiting(bLoginSuccessful, "Waiting for Login...");

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

	Waiting(bGetUserDone, "Waiting for Search Users...");

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(UserId1, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

	bDeleteDone = false;
	bDeleteSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserById2"));
	DeleteUserById(UserId2, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

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

	Waiting(bGetCountryDone, "Waiting for CountryInfo...");

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

	Waiting(bRegisterDone, "Waiting for Registered...");
	AB_TEST_TRUE(bRegisterSuccessful);

	bool bLoginSuccessful = false;
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&bLoginSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Login User"));
			bLoginSuccessful = true;
		}), UserTestErrorHandler);

	Waiting(bLoginSuccessful, "Waiting for Login...");
	AB_TEST_TRUE(bLoginSuccessful);

	bool bSuccessSendVerifCode = false;
	FRegistry::User.SendVerificationCode(FVoidHandler::CreateLambda([&bSuccessSendVerifCode]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Send Request Verification Code"));
			bSuccessSendVerifCode = true;
		}), UserTestErrorHandler);

	Waiting(bSuccessSendVerifCode, "Waiting send verification code");
	AB_TEST_TRUE(bSuccessSendVerifCode);

	const FString VerificationCode = GetVerificationCode(RegisterResult.UserId, EVerificationCode::accountRegistration);
	AB_TEST_FALSE(VerificationCode.IsEmpty());

	bool bSuccessVerifyUser = false;
	FRegistry::User.Verify(VerificationCode, FVoidHandler::CreateLambda([&bSuccessVerifyUser]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Verify User"));
			bSuccessVerifyUser = true;
		}), UserTestErrorHandler);
	Waiting(bSuccessVerifyUser, "Waiting verify user");
	AB_TEST_TRUE(bSuccessVerifyUser);

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Delete User"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");
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

	Waiting(bRegisterDone, "Waiting for Registered...");
	AB_TEST_TRUE(bRegisterSuccess);

	bool bLoginSuccess = false;
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Login User"));
			bLoginSuccess = true;
		}), UserTestErrorHandler);

	Waiting(bLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bLoginSuccess);

	bool bSendChangeEmailSuccess = false;
	FRegistry::User.SendUpdateEmailVerificationCode(FVoidHandler::CreateLambda([&bSendChangeEmailSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Send Change Email Verification Request"));
			bSendChangeEmailSuccess = true;
		}), UserTestErrorHandler);

	Waiting(bSendChangeEmailSuccess, "Waiting for Send Verification Request...");
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

	Waiting(bUpdateEmailSuccess, "Waiting for Update Email...");
	AB_TEST_TRUE(bUpdateEmailSuccess);

	const FString NewEmailVerificationCode = GetVerificationCode(RegisterResult.UserId, EVerificationCode::updateEmail);
	AB_TEST_FALSE(NewEmailVerificationCode.IsEmpty());

	bool bVerifyNewEmailSuccess = false;
	FRegistry::User.Verify(NewEmailVerificationCode, FVoidHandler::CreateLambda([&bVerifyNewEmailSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Verify New Email"));
			bVerifyNewEmailSuccess = true;
		}), UserTestErrorHandler);

	Waiting(bVerifyNewEmailSuccess, "Waiting for Verify on New Email...");
	AB_TEST_TRUE(bVerifyNewEmailSuccess);

	FRegistry::User.ForgetAllCredentials();

	bLoginSuccess = false;
	FRegistry::User.LoginWithUsername(NewEmailAddress, Password, FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Login New Email"));
			bLoginSuccess = true;
		}), UserTestErrorHandler);

	Waiting(bLoginSuccess, "Waiting for Login New Email...");
	AB_TEST_TRUE(bLoginSuccess);

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Success Delete User"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), UserTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");
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

	Waiting(bAwsAuthComplete, "AWS Cognito authenticate user password...");

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

	Waiting(bLoginWithAwsCognitoComplete, "Login with AWS Cognito...");

	AB_TEST_TRUE(bLoginWithAwsCognitoSuccess);

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
		}), UserTestErrorHandler);
	Waiting(bGetVerificationCodeSuccess, "Getting Verification Code...");

	switch (code)
	{
	case EVerificationCode::accountRegistration: return verificationCode.accountRegistration;
	case EVerificationCode::accountUpgrade: return verificationCode.accountUpgrade;
	case EVerificationCode::passwordReset: return verificationCode.passwordReset;
	case EVerificationCode::updateEmail: return verificationCode.updateEmail;
	}
	return FString("");
}
