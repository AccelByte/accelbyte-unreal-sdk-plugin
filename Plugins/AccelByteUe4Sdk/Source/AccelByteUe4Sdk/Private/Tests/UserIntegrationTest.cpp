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
#include "Core/AccelByteEnvironment.h"
#include "HAL/FileManager.h"
#include "Api/AccelByteUserProfileApi.h"
#include "TestUtilities.h"
#include "Runtime/Launch/Resources/Version.h"

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

const auto UserTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteUserTest, Fatal, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterTest, "AccelByte.Tests.AUser.RegisterEmail_ThenLogin", AutomationFlagMaskUser);
bool FUserRegisterTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
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
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	check(bLoginSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterv2Test, "AccelByte.Tests.AUser.RegisterUsername_ThenLogin", AutomationFlagMaskUser);
bool FUserRegisterv2Test::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString Username = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

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

	FlushHttpRequests();
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
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	check(bLoginSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserAutomatedRefreshSessionTest, "AccelByte.Tests.AUser.AutomatedRefreshSession", AutomationFlagMaskUser);
bool FUserAutomatedRefreshSessionTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	bool bRegisterDone = true;
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

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	// set session expired time to 0
	FString SessionId = FRegistry::Credentials.GetUserSessionId();
	FString RefreshId = FRegistry::Credentials.GetUserRefreshId();
	FRegistry::Credentials.SetUserSession(SessionId, 0, RefreshId);

	FString NewSessionId = FRegistry::Credentials.GetUserSessionId();
	FString NewRefreshId = FRegistry::Credentials.GetUserRefreshId();

	// wait session to refresh
	for (int i = 0; i < 20; i++)
	{
		FPlatformProcess::Sleep(0.5f);
		FTicker::GetCoreTicker().Tick(0.5f);
        
		NewSessionId = FRegistry::Credentials.GetUserSessionId();
		NewRefreshId = FRegistry::Credentials.GetUserRefreshId();

		if (SessionId != NewSessionId && RefreshId != NewRefreshId)
		{
			break;
		}
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
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Assert.."));
	check(bLoginSuccessful);
	check(SessionId != NewSessionId);
	check(RefreshId != NewRefreshId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginTest, "AccelByte.Tests.AUser.LoginEmail_ThenVerify", AutomationFlagMaskUser);
bool FUserLoginTest::RunTest(const FString & Parameter)
{
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

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bVerifyUserSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("VerifyingAccount"));
	FRegistry::User.Verify(VerificationCode, FVoidHandler::CreateLambda([&bVerifyUserSuccessful]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("   Success"));
		bVerifyUserSuccessful = true;
	}), UserTestErrorHandler);

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
		UserTestErrorHandler);

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
	}), UserTestErrorHandler);

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
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "1Old_Password1";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

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

	FlushHttpRequests();
	Waiting(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogTemp, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bForgotPasswordSuccessful = false; 
	UE_LOG(LogAccelByteUserTest, Log, TEXT("RequestPasswordReset"));
	
	FRegistry::User.SendResetPasswordCode(EmailAddress, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bForgotPasswordSuccessful = true;
	}), UserTestErrorHandler);

	FlushHttpRequests();
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
	FlushHttpRequests();

	UE_LOG(LogAccelByteUserTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	Password = "1New_Password1";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("ResetPassword"));
	
	FRegistry::User.ResetPassword(VerificationCode, EmailAddress, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bResetPasswordSuccessful = true;
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);
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
	}), UserTestErrorHandler);
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
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);

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
//	FlushHttpRequests();
//	Waiting(bDeviceLoginSuccessful1, "Waiting for Login...");
//
//	FirstUserId = FRegistry::Credentials.GetUserId();
//	OldAccessToken = FRegistry::Credentials.GetUserSessionId();
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("------------------OLD TOKEN: %s----------------"), *OldAccessToken);
//
//	bool bUpgradeSuccessful = false;
//	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
//	FRegistry::User.Upgrade(Email, Password, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
//	{
//		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
//		bUpgradeSuccessful = true;
//	}), UserTestErrorHandler);
//
//	FlushHttpRequests();
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
//	FlushHttpRequests();
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
//	FlushHttpRequests();
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
//	FlushHttpRequests();
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
//	FlushHttpRequests();
//	Waiting(bDeleteDone2, "Waiting for Deletion...");
//
//#pragma endregion DeleteUser2
//
//	check(bUpgradeSuccessful);
//	check(bUpgradedHeadlessAccountUserIdRemain);
//	check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "");
//	check(bEmailLoginSuccessful);
//	check(bDeviceLoginSuccessful1);
//	check(bDeviceLoginSuccessful2);
//	check(!OldAccessToken.IsEmpty() && !RefreshedAccessToken.IsEmpty());
//	check(!OldAccessToken.Equals(RefreshedAccessToken));
//	check(bDeleteSuccessful1);
//	check(bDeleteSuccessful2);
//	return true;
//}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamSuccess, "AccelByte.Tests.AUser.LoginWithSteam.LoginTwiceGetSameUserId", AutomationFlagMaskUser);
bool FLoginWithSteamSuccess::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

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

	FlushHttpRequests();
	Waiting(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

	FirstUserId = FRegistry::Credentials.GetUserId();
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

	FlushHttpRequests();
	Waiting(bSteamLoginDone2, "Waiting for Login...");

	if (!bSteamLoginSuccessful2)
	{
		return false;
	}

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
	}), UserTestErrorHandler);

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

	FlushHttpRequests();
	Waiting(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

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
	}), UserTestErrorHandler);

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bSteamLoginDone2, "Waiting for Login...");

	if (!bSteamLoginSuccessful2)
	{
		return false;
	}

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
	}), UserTestErrorHandler);

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

	FlushHttpRequests();
	Waiting(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	FirstUserId = FRegistry::Credentials.GetUserId();
	OldAccessToken = FRegistry::Credentials.GetUserSessionId();

	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	FRegistry::User.Upgrade(Email, Password, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
	}), UserTestErrorHandler);
	FlushHttpRequests();
	Waiting(bUpgradeSuccessful, "Waiting for Upgrade...");

	FRegistry::User.ForgetAllCredentials();

	bool bLoginEmailSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Email, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginEmailSuccessful = true;
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeSteamAccountv2Success, "AccelByte.Tests.AUser.UpgradeHeadlessSteamAccountv2", AutomationFlagMaskUser);
bool FUpgradeSteamAccountv2Success::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString Email = TEXT("testSDKsteam@game.test");
	FString Username = TEXT("testSDKsteam");
	FString Password = TEXT("123SDKTest123");
	double LastTime = 0;
	FString FirstUserId = TEXT("");
	FString OldAccessToken = "", RefreshedAccessToken = "";

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

	FlushHttpRequests();
	Waiting(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	FirstUserId = FRegistry::Credentials.GetUserId();
	OldAccessToken = FRegistry::Credentials.GetUserSessionId();

	bool bUpgradeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpgradeHeadlessAccount"));
	FRegistry::User.Upgradev2(Email, Username, Password, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bUpgradeSuccessful = true;
	}), UserTestErrorHandler);
	FlushHttpRequests();
	Waiting(bUpgradeSuccessful, "Waiting for Upgrade...");

	FRegistry::User.ForgetAllCredentials();

	bool bLoginUsernameSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(Username, Password, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bLoginUsernameSuccessful = true;
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginUsernameSuccessful, "Waiting for Login...");

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
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser1

	check(FirstUserId == FRegistry::Credentials.GetUserId());
	check(bLoginPlatformSuccessful);
	check(bUpgradeSuccessful);
	check(bLoginUsernameSuccessful);
	check(!OldAccessToken.IsEmpty() && !RefreshedAccessToken.IsEmpty());
	check(!OldAccessToken.Equals(RefreshedAccessToken));
	check(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLinkSteamAccountSuccess, "AccelByte.Tests.AUser.LinktoSteamAccount", AutomationFlagMaskUser);
bool FLinkSteamAccountSuccess::RunTest(const FString & Parameter)
{
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

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUserSteam

	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
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

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserData = false;
	FUserData UserData;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FUserData>::CreateLambda([&bGetUserData, &UserData](const FUserData& Result)
	{
		UserData = Result;
		bGetUserData = true;
	}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
	{
		bGetUserData = true;
		UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
	}));

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bLinkSteamAcc, "Waiting for Link Account...");

	bool bGetPlatformLinks = false;
	bool bGetPlatformLinksSuccess = false;
	FRegistry::User.GetPlatformLinks(THandler<FPagedPlatformLinks>::CreateLambda ([&bGetPlatformLinks, &bGetPlatformLinksSuccess](const FPagedPlatformLinks& res)
	{
		bGetPlatformLinks = true;
		bGetPlatformLinksSuccess = true;
	}),  FErrorHandler::CreateLambda([&bGetPlatformLinks](int32 Code, const FString& Message)
	{
		bGetPlatformLinks = true;
		UE_LOG(LogAccelByteUserTest, Error, TEXT("GetPlatformLinks..! Error: %d | Message: %s"), Code, *Message);
	}));

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	bGetUserData = false;
	FUserData UserData2;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FUserData>::CreateLambda([&bGetUserData, &UserData2](const FUserData& Result)
	{
		UserData2 = Result;
		bGetUserData = true;
	}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
	{
		bGetUserData = true;
		UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
	}));

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser

	check(bLoginPlatformSuccessful);
	check(bLinkSteamSuccess);
	check(bGetPlatformLinksSuccess);
	check(bLoginSuccessful);
	check(UserData.UserId == UserData2.UserId);
	check(UserData.EmailAddress == UserData2.EmailAddress);
	check(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLinkSteamAccountConflict, "AccelByte.Tests.AUser.LinktoSteamAccountConflict", AutomationFlagMaskUser);
bool FLinkSteamAccountConflict::RunTest(const FString & Parameter)
{
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

	FlushHttpRequests();
	Waiting(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
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

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserData = false;
	FUserData UserData;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FUserData>::CreateLambda([&bGetUserData, &UserData](const FUserData& Result)
	{
		UserData = Result;
		bGetUserData = true;
	}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
	{
		bGetUserData = true;
		UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
	}));

	FlushHttpRequests();
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
		UE_LOG(LogAccelByteUserTest, Log, TEXT("Link Account Failed..! Error: %d | Message: %s | PlatformUserId: %s"), Code, *Message, *MessageVariables.GetStringField("platformUserId"));
		bLinkSteamAcc = true;
	}));

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	bGetUserData = false;
	FUserData UserData2;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FUserData>::CreateLambda([&bGetUserData, &UserData2](const FUserData& Result)
	{
		UserData2 = Result;
		bGetUserData = true;
	}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
	{
		bGetUserData = true;
		UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
	}));

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser

	check(bLoginPlatformSuccessful);
	check(!bLinkSteamSuccess);
	check(bLoginSuccessful);
	check(UserData.UserId != UserData2.UserId);
	check(UserData.EmailAddress != UserData2.EmailAddress);
	check(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLinkSteamAccountForcedSuccess, "AccelByte.Tests.AUser.LinktoSteamAccountForced", AutomationFlagMaskUser);
bool FLinkSteamAccountForcedSuccess::RunTest(const FString & Parameter)
{
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

	FlushHttpRequests();
	Waiting(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
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

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserData = false;
	FUserData UserData;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FUserData>::CreateLambda([&bGetUserData, &UserData](const FUserData& Result)
	{
		UserData = Result;
		bGetUserData = true;
	}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
	{
		bGetUserData = true;
		UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
	}));

	FlushHttpRequests();
	Waiting(bGetUserData, "Waiting for Get User Data...");

	FString SteamUserID;
	//STEAM_USER_ID env var is supposed to be the current user logged in to steam
	SteamUserID = Environment::GetEnvironmentVariable(TEXT("STEAM_USER_ID"), 1000);

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

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bSteamLoginDone, "Waiting for Login...");

	if (!bLoginPlatformSuccessful)
	{
		return false;
	}

	bGetUserData = false;
	FUserData UserData2;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetUserData"));
	FRegistry::User.GetData(THandler<FUserData>::CreateLambda([&bGetUserData, &UserData2](const FUserData& Result)
	{
		UserData2 = Result;
		bGetUserData = true;
	}), FErrorHandler::CreateLambda([&bGetUserData](int32 Code, const FString& Message)
	{
		bGetUserData = true;
		UE_LOG(LogAccelByteUserTest, Error, TEXT("Get User Data Failed..! Error: %d | Message: %s"), Code, *Message);
	}));

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bDeleteDone1, "Waiting for Deletion...");

#pragma endregion DeleteUser

	check(bLoginPlatformSuccessful);
	check(bLinkSteamSuccess);
	check(bLoginSuccessful);
	check(UserData.UserId == UserData2.UserId);
	check(UserData.EmailAddress == UserData2.EmailAddress);
	check(bDeleteSuccessful1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetOtherPublicUserProfileTest, "AccelByte.Tests.AUserProfile.GetOtherPublicUserProfile", AutomationFlagMaskUser);
bool FGetOtherPublicUserProfileTest::RunTest(const FString & Parameter)
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

	FlushHttpRequests();
	Waiting(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithUsernameAndPassword"));

	FRegistry::User.LoginWithUsername(
		EmailAddress,
		Password,
		FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}),
		UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Registered...");

	FAccelByteModelsUserProfileCreateRequest ProfileCreate;
	ProfileCreate.FirstName = "Jirolu";
	ProfileCreate.LastName = "Patmonem";
	ProfileCreate.Language = "en";
	ProfileCreate.Timezone = "Etc/UTC";
	ProfileCreate.DateOfBirth = "1970-01-01";
	ProfileCreate.AvatarSmallUrl = "http://example.com/avatar/small.jpg";
	ProfileCreate.AvatarUrl = "http://example.com/avatar/normal.jpg";
	ProfileCreate.AvatarLargeUrl = "http://example.com/avatar/large.jpg";
	FAccelByteModelsUserProfileInfo CreatedProfile;

	bool bCreateProfileSuccessful = false;
	bool bCreateProfileDone = false;
	FRegistry::UserProfile.CreateUserProfile(
		ProfileCreate,
		THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bCreateProfileSuccessful = true;
			bCreateProfileDone = true;
			CreatedProfile = Result;
		}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			bCreateProfileDone = true;

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

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful, "Waiting for Create Profile...");

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

	FlushHttpRequests();
	Waiting(bGetProfileSuccessful, "Waiting for Get Profile...");

	FRegistry::User.ForgetAllCredentials();

	FRegistry::User.LoginWithUsername(
		EmailAddress,
		Password,
		FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}),
		UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Registered...");

#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	DeleteUserProfile(FRegistry::Credentials.GetUserNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
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

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById
	check(OtherUserProfile.AvatarSmallUrl == CreatedProfile.AvatarSmallUrl);
	check(OtherUserProfile.AvatarUrl == CreatedProfile.AvatarUrl);
	check(OtherUserProfile.AvatarLargeUrl == CreatedProfile.AvatarLargeUrl);
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
	}), UserTestErrorHandler);

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
	}), UserTestErrorHandler);

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bUpdateProfileSuccessful, "Waiting for Update...");

	bool bGetProfileSuccessful2 = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("GetProfile // Second attempt"));
	FRegistry::UserProfile.GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bGetProfileSuccessful2 = true;
		UpdatedDateOfBirth = Result.DateOfBirth;
	}), UserTestErrorHandler);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileCustomAttributesTest, "AccelByte.Tests.AUserProfile.UserProfileCustomAttributes", AutomationFlagMaskUser);
bool FUserProfileCustomAttributesTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	double LastTime = 0;

	bool bDeviceLoginSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("LoginWithDeviceId"));
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		bDeviceLoginSuccessful = true;
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bDeviceLoginSuccessful, "Waiting for Login...");

	//Delete profile regardless if it's exist or not (test cleanup double check)
#pragma region DeleteUserProfile
	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	DeleteUserProfile(FRegistry::Credentials.GetUserNamespace(), FRegistry::Credentials.GetUserId(), 
		FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Unexpected condition: User profile should not exist before the test"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), 
		FErrorHandler::CreateLambda([&bDeleteProfileDone](int32 ErrorCode, const FString& ErrorMessage)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Expected condition: User profile not found"))
			bDeleteProfileDone = true;
		}));

	Waiting(bDeleteProfileDone, "Waiting for Deletion...");
#pragma endregion DeleteUserProfile


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
	FAccelByteModelsUserProfileInfo ProfileCreateResult;

	FRegistry::UserProfile.CreateUserProfile(
		ProfileCreate, 
		THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
			ProfileCreateResult = Result;
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

	FlushHttpRequests();
	Waiting(bCreateProfileSuccessful, "Waiting for Create Profile...");

	bool bUpdateCustomAttributeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpdateCustomAttributeProfile"));

	int32 numberAttribute = 2;
	FString stringAttribute = "Five kuwi lima";
	bool booleanAttribute = true;
	FJsonObject customAttribute;
	customAttribute.SetNumberField("Two", numberAttribute);
	customAttribute.SetStringField("Five", stringAttribute);
	customAttribute.SetBoolField("True", booleanAttribute);
	FJsonObject updatedCustomAttribute;
	FRegistry::UserProfile.UpdateCustomAttributes(
		customAttribute,
		THandler<FJsonObject>::CreateLambda([&updatedCustomAttribute, &bUpdateCustomAttributeSuccessful](const FJsonObject& Result)
	{
		UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
		updatedCustomAttribute = Result;
		bUpdateCustomAttributeSuccessful = true;
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bUpdateCustomAttributeSuccessful, "Waiting for Update Custom Attributes...");

	bool bGetCustomAttributeSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("UpdateCustomAttributeProfile"));

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

	FlushHttpRequests();
	Waiting(bUpdateCustomAttributeSuccessful, "Waiting for Get Custom Attributes...");

	int32 getNumberAttribute;
	getCustomAttribute.TryGetNumberField("Two", getNumberAttribute);
	FString getStringAttribute;
	getCustomAttribute.TryGetStringField("Five", getStringAttribute);
	bool getBooleanAttribute;
	getCustomAttribute.TryGetBoolField("True", getBooleanAttribute);

#pragma region DeleteUserProfile
	bDeleteProfileDone = false;
	bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteUserTest, Log, TEXT("DeleteUserProfile"));
	DeleteUserProfile(FRegistry::Credentials.GetUserNamespace(), FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
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

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	check(bDeviceLoginSuccessful);
	check(bUpdateCustomAttributeSuccessful);
	check(updatedNumberAttribute == numberAttribute);
	check(updatedStringAttribute == stringAttribute);
	check(updatedBooleanAttribute == booleanAttribute);
	check(bGetCustomAttributeSuccessful);
	check(getNumberAttribute == numberAttribute);
	check(getStringAttribute == stringAttribute);
	check(getBooleanAttribute == booleanAttribute);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserBySteamUserIDTest, "AccelByte.Tests.AUser.GetUserBySteamUserID", AutomationFlagMaskUser);
bool FGetUserBySteamUserIDTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString FirstUserId = "";
	double LastTime = 0;

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

	FlushHttpRequests();
	Waiting(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

	FirstUserId = FRegistry::Credentials.GetUserId();

	//STEAM_USER_ID env var is supposed to be the current user logged in to steam
	FString SteamUserID = Environment::GetEnvironmentVariable(TEXT("STEAM_USER_ID"), 100);

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
		UserTestErrorHandler);

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
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	check(bSteamLoginSuccessful1);
	check(bDeleteSuccessful);
	check(ReceivedUserData.UserId.Compare("") != 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBulkGetUserBySteamUserIDTest, "AccelByte.Tests.AUser.BulkGetUserBySteamUserID", AutomationFlagMaskUser);
bool FBulkGetUserBySteamUserIDTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString ABUserId = "";
	double LastTime = 0;

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

	FlushHttpRequests();
	Waiting(bSteamLoginDone1, "Waiting for Login...");

	if (!bSteamLoginSuccessful1)
	{
		return false;
	}

	ABUserId = FRegistry::Credentials.GetUserId();
	FString SteamUserID;

	//STEAM_USER_ID env var is supposed to be the current user logged in to steam
	SteamUserID = Environment::GetEnvironmentVariable(TEXT("STEAM_USER_ID"), 1000);

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
	}), UserTestErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	check(bSteamLoginSuccessful1);
	check(bDeleteSuccessful);
	check(ReceivedUserData.UserIdPlatforms.Num() != 0);
	check(ReceivedUserData.UserIdPlatforms[0].UserId == ABUserId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserByEmailAddressTest, "AccelByte.Tests.AUser.GetUserByEmailAddress", AutomationFlagMaskUser);
bool FGetUserByEmailAddressTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
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

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserDone = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(
		EmailAddress,
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
		UserTestErrorHandler);

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

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	check(bLoginSuccessful);
	check(bGetUserDone);
	check(bDeleteSuccessful);
	check(ReceivedUserData.Data.Num() != 0);
	check(ReceivedUserData.Data[0].EmailAddress == EmailAddress);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserByDisplayNameTest, "AccelByte.Tests.AUser.GetUserByDisplayName", AutomationFlagMaskUser);
bool FGetUserByDisplayNameTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
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
		UserTestErrorHandler);

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

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	check(bLoginSuccessful);
	check(bGetUserDone);
	check(bDeleteSuccessful);
	check(ReceivedUserData.Data.Num() != 0);
	bool bIsDisplayNameFound = false;
	for (int i = 0; i < ReceivedUserData.Data.Num(); i++)
	{
		if (ReceivedUserData.Data[i].DisplayName == DisplayName)
		{
			bIsDisplayNameFound = true;
			break;
		}
	}
	check(bIsDisplayNameFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserByUsernameTest, "AccelByte.Tests.AUser.GetUserByUsername", AutomationFlagMaskUser);
bool FGetUserByUsernameTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString Username = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + Username + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

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

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Login...");

	bool bGetUserDone = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(
		Username,
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
		UserTestErrorHandler);

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

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	check(bLoginSuccessful);
	check(bGetUserDone);
	check(bDeleteSuccessful);
	check(ReceivedUserData.Data.Num() != 0);
	bool bIsUsernameFound = false;
	for (int i = 0; i < ReceivedUserData.Data.Num(); i++)
	{
		if (ReceivedUserData.Data[i].UserName == Username)
		{
			bIsUsernameFound = true;
			break;
		}
	}
	check(bIsUsernameFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserFilterByUsernameTest, "AccelByte.Tests.AUser.GetUserFilterByUsername", AutomationFlagMaskUser);
bool FGetUserFilterByUsernameTest::RunTest(const FString & Parameter)
{
	FRegistry::User.ForgetAllCredentials();
	FString SearchQuery = "accelbyte";
	FString Username = SearchQuery + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + Username + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

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

	FlushHttpRequests();
	Waiting(bRegisterDone, "Waiting for Registered...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	FString Username2 = FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress2 = SearchQuery + "+" + Username + "@game.test";
	bRegisterSuccessful = false;
	bRegisterDone = false;
	FString UserId2 = "";
	UE_LOG(LogAccelByteUserTest, Log, TEXT("CreateEmailAccount2"));
	FRegistry::User.Registerv2(EmailAddress2, Username2, Password, "", Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone, &UserId2](const FRegisterResponse& Result)
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

	FlushHttpRequests();
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

	FlushHttpRequests();
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
		for (auto Data : ReceivedUserData.Data)
		{
			UE_LOG(LogAccelByteUserTest, Log, TEXT("Get User, Email: %s, DisplayName: %s, UserId: %s"), *Data.EmailAddress, *Data.DisplayName, *Data.UserId);
		}
	}),
		UserTestErrorHandler);

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

	FlushHttpRequests();
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

	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for Deletion...");

#pragma endregion DeleteUserById

	check(bLoginSuccessful);
	check(bGetUserDone);
	check(bDeleteSuccessful);
	bool bIsUsernameExpected = true;
	for (int i = 0; i < ReceivedUserData.Data.Num(); i++)
	{
		if (!ReceivedUserData.Data[i].UserName.Contains(SearchQuery))
		{
			bIsUsernameExpected = false;
			break;
		}
	}
	check(bIsUsernameExpected);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetCountryFromIP, "AccelByte.Tests.AUser.GetCountryFromIP", AutomationFlagMaskUser);
bool FGetCountryFromIP::RunTest(const FString & Parameter)
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

	FlushHttpRequests();
	Waiting(bGetCountryDone, "Waiting for CountryInfo...");

	check(bGetCountryDone);
	check(ReceivedCountryInfo.CountryCode != "");
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
	}), UserTestErrorHandler);
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
