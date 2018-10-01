// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Public/Test/IntegrationTest.h"
#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteServicesIdentity.h"
#include "AccelByteServicesOrder.h"
#include "AccelByteServicesCategory.h"
#include "AccelByteServicesItem.h"
#include "AccelByteServicesUserProfile.h"
#include "AccelByteServicesUser.h"
#include <algorithm>

#if 0
DECLARE_LOG_CATEGORY_EXTERN(LogJusticeTest, Log, All);
DEFINE_LOG_CATEGORY(LogJusticeTest);
const int32 AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext); 
// Context: EditorContext (run via editor cli) ClientContext (run via external cli)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginSuccessTest, "JusticeTest.User.LoginSuccess", AutomationFlagMask);
bool FLoginSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "hendra+2@accelbyte.net";
	FString Password = "123456";
	bool isDone = false;
	bool isLoginSuccess = false;
	UE_LOG(LogJusticeTest, Log, TEXT("FLoginLatentCommand::Update()"));

	FUserLoginCompleteDelegate2 OnComplete = FUserLoginCompleteDelegate2::CreateLambda([&isDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("FLoginLatentCommand::Update() Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnComplete);

	double LastTime = FPlatformTime::Seconds();
	while (!isDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogJusticeTest, Log, TEXT("FLoginLatentCommand::Update() After Login"));
	check(isLoginSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginFailedWrongPasswordTest, "Disabled.User.LoginFailedWrongPassword", AutomationFlagMask);
bool FLoginFailedWrongPasswordTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDone = false;
	bool bTestSuccessful = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, "test", OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnComplete = FUserLoginCompleteDelegate2::CreateLambda([&isDone, &bTestSuccessful](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login to get UserId result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		bTestSuccessful = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(!isLoginSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginFailedUnregisteredEmailTest, "Disabled.User.LoginFailedUnregisteredEmail", AutomationFlagMask);
bool FLoginFailedUnregisteredEmailTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDone = false;
	bool bTestSuccessful = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, "sdk@example.com", Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnComplete = FUserLoginCompleteDelegate2::CreateLambda([&isDone, &bTestSuccessful](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login to get UserId result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		bTestSuccessful = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(!isLoginSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLogoutSuccessTest, "Disabled.User.LogoutSuccess", AutomationFlagMask);
bool FLogoutSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isLogoutDone = false;
	bool isLogoutSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDefaultCompleteDelegate OnUserLogoutResponse = FDefaultCompleteDelegate::CreateLambda([&isLogoutDone, &isLogoutSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Logout Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLogoutDone = true;
		isLogoutSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogout(OnUserLogoutResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLogoutDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(isLogoutSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FForgotPasswordSuccessTest, "Disabled.User.ForgotPasswordSuccess", AutomationFlagMask);
bool FForgotPasswordSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "hendra@accelbyte.net";
	bool isForgotPasswdDone = false;
	bool isForgotPasswdSuccess = false;
	double LastTime;

	FDefaultCompleteDelegate OnForgotPasswdComplete = FDefaultCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Forgot password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = bSuccessful;
	});
	JusticeIdentity::SendVerificationCodeForPasswordReset(JusticeGameNamespace, LoginId, OnForgotPasswdComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isForgotPasswdDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogJusticeTest, Log, TEXT("Request forgot password done"));

	check(isForgotPasswdSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FForgotPasswordFailedEmailNotVerifiedTest, "Disabled.User.ForgotPasswordFailedEmailNotVerified", AutomationFlagMask);
bool FForgotPasswordFailedEmailNotVerifiedTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isForgotPasswdDone = false;
	bool isForgotPasswdSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	AddExpectedError(TEXT("OnForgotPasswordResponse"), EAutomationExpectedErrorFlags::Contains, 1);
	FDefaultCompleteDelegate OnForgotPasswordResponse = FDefaultCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Forgot password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = bSuccessful;
	});
	JusticeIdentity::SendVerificationCodeForPasswordReset(JusticeGameNamespace, LoginId, OnForgotPasswordResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isForgotPasswdDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr) 
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(!isForgotPasswdSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FForgotPasswordFailedInvalidUserTest, "Disabled.User.ForgotPasswordFailedInvalidUser", AutomationFlagMask);
bool FForgotPasswordFailedInvalidUserTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	bool isForgotPasswdDone = false;
	bool isForgotPasswdSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnForgotPasswordResponse"), EAutomationExpectedErrorFlags::Contains, 1);
	FDefaultCompleteDelegate OnForgotPasswordResponse = FDefaultCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Forgot password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = bSuccessful;
	});
	JusticeIdentity::SendVerificationCodeForPasswordReset(JusticeGameNamespace, LoginId, OnForgotPasswordResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isForgotPasswdDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(!isForgotPasswdSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterNewPlayerSuccessTest, "Disabled.User.RegisterNewPlayerSuccess", AutomationFlagMask);
bool FRegisterNewPlayerSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName,AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(isRegisterNewPlayerSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterNewPlayerFailedUserExistTest, "Disabled.User.RegisterNewPlayerFailedUserExist", AutomationFlagMask);
bool FRegisterNewPlayerFailedUserExistTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isRegisterPlayerDone = false;
	bool isRegisterPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnRegisterNewPlayerResponse"), EAutomationExpectedErrorFlags::Contains, 1);
	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FRegisterPlayerCompleteDelegate OnRegisterPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterPlayerDone, &isRegisterPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterPlayerDone = true;
		isRegisterPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	
	check(!isRegisterPlayerSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterNewPlayerInvalidEmailTest, "Disabled.User.RegisterNewPlayerFailedInvalidEmail", AutomationFlagMask);
bool FRegisterNewPlayerInvalidEmailTest::RunTest(const FString & Parameters)
{
	FString LoginId = "test";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnRegisterNewPlayerResponse"), EAutomationExpectedErrorFlags::Contains, 1);
	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(!isRegisterNewPlayerSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateDefaultPlayerProfileSuccessTest, "Disabled.User.CreateDefaultPlayerProfileSuccess", AutomationFlagMask);
bool FCreateDefaultPlayerProfileSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDefaultCompleteDelegate OnUpdatePlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, DisplayName, OnUpdatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(isCreateDefaultPlayerProfileSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateDefaultPlayerProfileFailedEmptyDisplayNameTest, "Disabled.Profile.CreateDefaultPlayerProfileFailedEmptyDisplayname", AutomationFlagMask);
bool FCreateDefaultPlayerProfileFailedEmptyDisplayNameTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;
	
	AddExpectedError(TEXT("OnCreateDefaultPlayerProfileComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FDefaultCompleteDelegate OnCreateDefaultPlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, "", OnCreateDefaultPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr) 
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(!isCreateDefaultPlayerProfileSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateDefaultPlayerProfileFailedUnregisteredUserTest, "Disabled.User.CreateDefaultPlayerProfileFailedUnregisteredUser", AutomationFlagMask);
bool FCreateDefaultPlayerProfileFailedUnregisteredUserTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FString AuthType = "EMAILPASSWD";
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnCreateDefaultPlayerProfileComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FDefaultCompleteDelegate OnCreateDefaultPlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, DisplayName, OnCreateDefaultPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(!isCreateDefaultPlayerProfileSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetPlayerProfileSuccessTest, "Disabled.User.GetPlayerProfileSuccess", AutomationFlagMask);
bool FGetPlayerProfileSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isGetPlayerProfileDone = false;
	bool isGetPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDefaultCompleteDelegate OnUpdatePlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, DisplayName, OnUpdatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FRequestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfileComplete = FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool bSuccessful, FString ErrorStr, FAccelByteModelsUserProfileInfo* userProfile)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Current Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = bSuccessful;
	});
	Platform::GetUserProfile(*FJusticeUserToken, OnGetPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isGetPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString CurrentDisplayName = FJusticeSDKModule::Get().UserProfile->DisplayName;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(CurrentDisplayName == DisplayName)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetPlayerProfileSuccessUnregisteredUserTest, "Disabled.User.GetPlayerProfileSuccessUnregisteredUser", AutomationFlagMask);
bool FGetPlayerProfileSuccessUnregisteredUserTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isGetPlayerProfileDone = false;
	bool isGetPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FRequestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfileComplete = FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool bSuccessful, FString ErrorStr, FAccelByteModelsUserProfileInfo* userProfile)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Current Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = bSuccessful;
	});
	Platform::GetUserProfile(*FJusticeUserToken, OnGetPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isGetPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(isGetPlayerProfileSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpdatePlayerProfileSuccessTest, "Disabled.User.UpdatePlayerProfileSuccess", AutomationFlagMask);
bool FUpdatePlayerProfileSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	FString UpdatedDisplayName = "IntegrationTestSDK";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isUpdatePlayerProfileDone = false;
	bool isUpdatePlayerProfileSuccess = false;
	bool isGetPlayerProfileDone = false;
	bool isGetPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> UserCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnUserLoginResponse = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDefaultCompleteDelegate OnCreatePlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, DisplayName, OnCreatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString userId = FJusticeSDKModule::Get().UserToken->UserId;

	FDefaultCompleteDelegate OnUpdatePlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&isUpdatePlayerProfileDone, &isUpdatePlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Update Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isUpdatePlayerProfileDone = true;
		isUpdatePlayerProfileSuccess = bSuccessful;
	});
	FAccelByteModelsUserProfileInfoUpdate newUserProfileinfo;
	newUserProfileinfo.Email = LoginId;
	newUserProfileinfo.DisplayName = UpdatedDisplayName;
	newUserProfileinfo.AvatarLargeUrl = FJusticeSDKModule::Get().UserProfile->AvatarLargeUrl;
	newUserProfileinfo.AvatarUrl = FJusticeSDKModule::Get().UserProfile->AvatarUrl;
	newUserProfileinfo.AvatarSmallUrl = FJusticeSDKModule::Get().UserProfile->AvatarSmallUrl;
	newUserProfileinfo.FirstName = "SDK";
	newUserProfileinfo.LastName = "integration";
	newUserProfileinfo.Language = "en";
	newUserProfileinfo.Timezone = "UTC";
	newUserProfileinfo.DateOfBirth = "2018-05-23";
	newUserProfileinfo.CustomAttributes = FJusticeSDKModule::Get().UserProfile->CustomAttributes;

	Platform::UpdateUserProfile(*FJusticeUserToken, newUserProfileinfo, OnUpdatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isUpdatePlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FRequestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfile1Complete = FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool bSuccessful, FString ErrorStr, FAccelByteModelsUserProfileInfo* userProfile)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Current Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = bSuccessful;
	});
	Platform::GetUserProfile(*FJusticeUserToken, OnGetPlayerProfile1Complete);
	
	LastTime = FPlatformTime::Seconds();
	while (!isGetPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString CurrentDisplayName = FJusticeSDKModule::Get().UserProfile->DisplayName;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(CurrentDisplayName == UpdatedDisplayName)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpdatePlayerProfileFailedInvalidRequestTest, "Disabled.User.UpdatePlayerProfileFailedInvalidRequest", AutomationFlagMask);
bool FUpdatePlayerProfileFailedInvalidRequestTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isUpdatePlayerProfileDone = false;
	bool isUpdatePlayerProfileSuccess = false;
	bool isGetPlayerProfileDone = false;
	bool isGetPlayerProfileSuccess = false;
	double LastTime;

	FDefaultCompleteDelegate OnCreateDefaultPlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, DisplayName, OnCreateDefaultPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString userId = FJusticeSDKModule::Get().UserToken->UserId;

	AddExpectedError(TEXT("OnUpdatePlayerProfileComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FDefaultCompleteDelegate OnUpdatePlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&isUpdatePlayerProfileDone, &isUpdatePlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Update Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isUpdatePlayerProfileDone = true;
		isUpdatePlayerProfileSuccess = bSuccessful;
	});
	FAccelByteModelsUserProfileInfoUpdate newUserProfileinfo;
	newUserProfileinfo.Email = LoginId;
	newUserProfileinfo.DisplayName = "updated";

	Platform::UpdateUserProfile(*FJusticeUserToken, newUserProfileinfo, OnUpdatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isUpdatePlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FRequestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfile1Complete = FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool bSuccessful, FString ErrorStr, FAccelByteModelsUserProfileInfo* userProfile)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Current Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = bSuccessful;
	});
	Platform::GetUserProfile(*FJusticeUserToken, OnGetPlayerProfile1Complete);

	LastTime = FPlatformTime::Seconds();
	while (!isGetPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UpdatedDisplayName = FJusticeSDKModule::Get().UserProfile->DisplayName;

	check(UpdatedDisplayName == DisplayName)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserLinkedPlatformTest, "Disabled.User.GetUserLinkedPlatform", AutomationFlagMask);
bool FGetUserLinkedPlatformTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isGetUserLinkedPlatformDone = false;
	bool isGetUserLinkedPlatformSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> userCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnComplete = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login to get UserId result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsLinkedPlatform> Response)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get linked platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = bSuccessful;
	});
	JusticeIdentity::GetLinkedUserAccounts(OnGetLinkedPlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isGetUserLinkedPlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(isGetUserLinkedPlatformSuccess)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLinkDevicePlatformSuccessTest, "Disabled.User.LinkDevicePlatform", AutomationFlagMask);
bool FLinkDevicePlatformSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	FString PlatformId = "device";
	FString DeviceId = "deviceIdTest";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isLinkDevicePlatformDone = false;
	bool isLinkDevicePlatformSuccess = false;
	bool isGetUserLinkedPlatformDone = false;
	bool isGetUserLinkedPlatformSuccess = false;
	bool isDeviceLinkedSuccessfully = false;
	bool isUnlinkDevicePlatformDone = false;
	bool isUnlinkDevicePlatformSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> userCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnComplete = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login to get UserId result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDefaultCompleteDelegate OnLinkDevicePlatformComplete = FDefaultCompleteDelegate::CreateLambda([&isLinkDevicePlatformDone, &isLinkDevicePlatformSuccess](bool bSuccessful, FString ErrorStr) 
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Link device platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLinkDevicePlatformDone = true;
		isLinkDevicePlatformSuccess = bSuccessful;
	});
	JusticeIdentity::LinkUserAccounts(JusticeGameNamespace, PlatformId, DeviceId, OnLinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess, &isDeviceLinkedSuccessfully](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsLinkedPlatform> Response)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get linked platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = bSuccessful;
		if (Response[0].PlatformId.Contains("device"))
		{
			isDeviceLinkedSuccessfully = true;
		}
	});
	JusticeIdentity::GetLinkedUserAccounts(OnGetLinkedPlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isGetUserLinkedPlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDefaultCompleteDelegate OnUnlinkDevicePlatformComplete = FDefaultCompleteDelegate::CreateLambda([&isUnlinkDevicePlatformDone, &isUnlinkDevicePlatformSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Unlink device platform: %s"), isUnlinkDevicePlatformDone ? TEXT("Success") : TEXT("Failed"));
		isUnlinkDevicePlatformDone = true;
		isUnlinkDevicePlatformSuccess = bSuccessful;
	});
	JusticeIdentity::UnlinkUserAccounts(JusticeGameNamespace, "device", OnUnlinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isUnlinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(isLinkDevicePlatformSuccess)
		return true;
	check(isDeviceLinkedSuccessfully)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnlinkDevicePlatformSuccessTest, "Disabled.User.UnlinkDevicePlatform", AutomationFlagMask);
bool FUnlinkDevicePlatformSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginId = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	FString PlatformId = "device";
	FString DeviceId = "deviceIdTest";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isGetUserLinkedPlatformDone = false;
	bool isGetUserLinkedPlatformSuccess = false;
	bool isLinkDevicePlatformDone = false;
	bool isLinkDevicePlatformSuccess = false;
	bool isUnlinkDevicePlatformDone = false;
	bool isUnlinkDevicePlatformSuccess = false;
	bool isDeviceUnlinkedSuccesfully = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> userCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnComplete = FUserLoginCompleteDelegate2::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login to get UserId result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDefaultCompleteDelegate OnLinkDevicePlatformComplete = FDefaultCompleteDelegate::CreateLambda([&isLinkDevicePlatformDone, &isLinkDevicePlatformSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Link device platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLinkDevicePlatformDone = true;
		isLinkDevicePlatformSuccess = bSuccessful;
	});
	JusticeIdentity::LinkUserAccounts(JusticeGameNamespace, PlatformId, DeviceId, OnLinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDefaultCompleteDelegate OnUnlinkDevicePlatformComplete = FDefaultCompleteDelegate::CreateLambda([&isUnlinkDevicePlatformDone, &isUnlinkDevicePlatformSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Unlink device platform: %s"), isUnlinkDevicePlatformDone ? TEXT("Success") : TEXT("Failed"));
		isUnlinkDevicePlatformDone = true;
		isUnlinkDevicePlatformSuccess = bSuccessful;
	});
	JusticeIdentity::UnlinkUserAccounts(JusticeGameNamespace, "device", OnUnlinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isUnlinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess, &isDeviceUnlinkedSuccesfully](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsLinkedPlatform> Response)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get linked platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = bSuccessful;
		if (Response.Num() == 0)
		{
			isDeviceUnlinkedSuccesfully = true;
		}
	});
	JusticeIdentity::GetLinkedUserAccounts(OnGetLinkedPlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isGetUserLinkedPlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(isUnlinkDevicePlatformSuccess)
		return true;
	check(isDeviceUnlinkedSuccesfully)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSendTelemetryEventSuccessTest, "JusticeTest.Telemetry.SendSuccess", AutomationFlagMask);
bool FSendTelemetryEventSuccessTest::RunTest(const FString & Parameters)
{
	FTelemetryEventDataExample TelemetryData;
	TelemetryData.ExampleField1 = 1555;
	TelemetryData.ExampleField2 = "1555";
	TelemetryData.ExampleField3 = 1555;
	FGuid Guid = FGuid::NewGuid();
	FString Uuid = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);
	FAccelByteModelsTelemetryEvent Telemetry;
	Telemetry.AppId = 1555;
	Telemetry.ClientId = FJusticeSDKModule::Get().ClientId;
	Telemetry.Data = TelemetryData.ToJson();
	Telemetry.DeviceId = FPlatformMisc::GetDeviceId();
	Telemetry.EventId = 1555;
	Telemetry.EventLevel = 1555;
	Telemetry.EventTime = FDateTime::Now().ToIso8601();
	Telemetry.EventType = 1555;
	Telemetry.UserId = "exampleUserId";
	Telemetry.Uuid = Uuid;
	Telemetry.Ux = 1555;
	bool bSendTelemetryDone = false;
	bool bSendTelemetrySuccessful = false;
	double LastTime;

	FDefaultCompleteDelegate OnSendTelemetryComplete = FDefaultCompleteDelegate::CreateLambda([&bSendTelemetryDone, &bSendTelemetrySuccessful](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Send Telemetry Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSendTelemetryDone = true;
		bSendTelemetrySuccessful = bSuccessful;
	});
	JusticeTelemetry::SendTelemetryEvent(Telemetry, OnSendTelemetryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bSendTelemetryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bSendTelemetrySuccessful)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSendTelemetryIncompleteDataSuccessTest, "JusticeTest.Telemetry.SendIncompleteDataSuccess", AutomationFlagMask);
bool FSendTelemetryIncompleteDataSuccessTest::RunTest(const FString & Parameters)
{
	FTelemetryEventDataExample TelemetryData;
	FGuid Guid = FGuid::NewGuid();
	FString Uuid = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);
	FAccelByteModelsTelemetryEvent Telemetry;
	Telemetry.AppId = 1555;
	Telemetry.ClientId = FJusticeSDKModule::Get().ClientId;
	Telemetry.Data = TelemetryData.ToJson();
	Telemetry.DeviceId = FPlatformMisc::GetDeviceId();
	Telemetry.EventId = 1555;
	Telemetry.EventLevel = 1555;
	Telemetry.EventTime = FDateTime::Now().ToIso8601();
	Telemetry.EventType = 1555;
	Telemetry.UserId = "exampleUserId";
	Telemetry.Uuid = Uuid;
	Telemetry.Ux = 1555;
	bool bSendTelemetryIncompleteDataDone = false;
	bool bSendTelemetryIncompleteDataSuccessful = false;
	double LastTime;

	FDefaultCompleteDelegate OnSendTelemetryEventComplete = FDefaultCompleteDelegate::CreateLambda([&bSendTelemetryIncompleteDataDone, &bSendTelemetryIncompleteDataSuccessful](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Send Telemetry With Incomplete Telemetry Data Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSendTelemetryIncompleteDataDone = true;
		bSendTelemetryIncompleteDataSuccessful = bSuccessful;
	});
	JusticeTelemetry::SendTelemetryEvent(Telemetry, OnSendTelemetryEventComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bSendTelemetryIncompleteDataDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bSendTelemetryIncompleteDataSuccessful)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSendTelemetryEventRandomPathSuccessTest, "Disabled.User.TelemetrySendToRandomPathSuccess", AutomationFlagMask);
bool FSendTelemetryEventRandomPathSuccessTest::RunTest(const FString & Parameters)
{
	FTelemetryEventDataExample TelemetryData;
	TelemetryData.ExampleField1 = 1555;
	TelemetryData.ExampleField2 = "1555";
	TelemetryData.ExampleField3 = 1555;
	FGuid Guid = FGuid::NewGuid();
	FString Uuid = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);
	FAccelByteModelsTelemetryEvent Telemetry;
	Telemetry.AppId = rand();
	Telemetry.ClientId = FJusticeSDKModule::Get().ClientId;
	Telemetry.Data = TelemetryData.ToJson();
	Telemetry.DeviceId = FPlatformMisc::GetDeviceId();
	Telemetry.EventId = rand();
	Telemetry.EventLevel = rand();
	Telemetry.EventTime = FDateTime::Now().ToIso8601();
	Telemetry.EventType = rand();
	Telemetry.UserId = "exampleUserId";
	Telemetry.Uuid = Uuid;
	Telemetry.Ux = 1555;
	bool bSendTelemetryRandomPathDone = false;
	bool bSendTelemetryRandomPathSuccessful = false;
	double LastTime;

	FDefaultCompleteDelegate OnSendTelemetryEventComplete = FDefaultCompleteDelegate::CreateLambda([&bSendTelemetryRandomPathDone, &bSendTelemetryRandomPathSuccessful](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Send Telemetry To Random Path Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSendTelemetryRandomPathDone = true;
		bSendTelemetryRandomPathSuccessful = bSuccessful;
	});
	JusticeTelemetry::SendTelemetryEvent(Telemetry, OnSendTelemetryEventComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bSendTelemetryRandomPathDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bSendTelemetryRandomPathSuccessful)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetChildCategorySuccess, "JusticeTest.Category.GetChildSuccess", AutomationFlagMask);
bool FGetChildCategorySuccess::RunTest(const FString & Parameter)
{
	UE_LOG(LogJusticeTest, Log, TEXT("JusticeTest.Category.GetChildSuccess Started..."));
	
	FString LoginId = "hendra+2@accelbyte.net";
	FString Password = "123456";
	FString Language = "en";
	TArray<FAccelByteModelsFullCategoryInfo> RootCategory;
	bool bRegisterNewPlayerDone = false;
	bool bRegisterNewPlayerSuccess = false;
	bool bLoginDone = false;
	bool bGetRootCategoryDone = false;
	bool bGetChildCategoryDone = false;
	bool bDeleteDone = false;
	bool bDeleteSuccess = false;
	double LastTime;

	FUserLoginCompleteDelegate2 OnLoginComplete = FUserLoginCompleteDelegate2::CreateLambda([&bLoginDone](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 1 User Login Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bLoginDone = true;
		check(bSuccessful);		
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 1 User Login Started..."));
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;
	FCategoryDefaultCompleteDelegate OnGetRootCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetRootCategoryDone, &RootCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 2 GetRootCategories Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetRootCategoryDone = true;
		RootCategory = Result;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 2 GetRootCategories Started..."));
	Category::GetRootCategories(Language, OnGetRootCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetRootCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FCategoryDefaultCompleteDelegate OnGetChildCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetChildCategoryDone](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 3 GetChildCategories Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetChildCategoryDone = true;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 3 GetChildCategories Started..."));
	Category::GetChildCategories(Language, RootCategory[0].CategoryPath, OnGetChildCategoryComplete);
	
	LastTime = FPlatformTime::Seconds();
	while (!bGetChildCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	check(bGetChildCategoryDone);
	UE_LOG(LogJusticeTest, Log, TEXT("JusticeTest.Category.GetChildSuccess Result...Done!"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetRootCategorySuccess, "JusticeTest.Category.GetRootSuccess", AutomationFlagMask);
bool FGetRootCategorySuccess::RunTest(const FString & Parameter)
{
	UE_LOG(LogJusticeTest, Log, TEXT("JusticeTest.Category.GetRootSuccess Started..."));

	FString LoginId = "hendra+2@accelbyte.net";
	FString Password = "123456";
	FString Language = "en";
	FString RootCategoryPath;
	bool bRegisterNewPlayerDone = false;
	bool bRegisterNewPlayerSuccess = false;
	bool bLoginDone = false;
	bool bGetRootCategoryDone = false;
	bool bGetRootCategorySuccess = false;
	double LastTime;

	FUserLoginCompleteDelegate2 OnLoginComplete = FUserLoginCompleteDelegate2::CreateLambda([&bLoginDone](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 1 User Login Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bLoginDone = true;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 1 User Login Started..."));
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;
	FCategoryDefaultCompleteDelegate OnGetRootCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 2 GetRootCategories Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetRootCategoryDone = true;
		bGetRootCategorySuccess = bSuccessful;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetChildSuccess] Phase 2 GetRootCategories Started..."));
	Category::GetRootCategories(Language, OnGetRootCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetRootCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bGetRootCategorySuccess);
	UE_LOG(LogJusticeTest, Log, TEXT("JusticeTest.Category.GetRootSuccess Result...Done!"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetDescendantCategorySuccess, "JusticeTest.Category.GetDescendantSuccess", AutomationFlagMask);
bool FGetDescendantCategorySuccess::RunTest(const FString & Parameter)
{
	UE_LOG(LogJusticeTest, Log, TEXT("JusticeTest.Category.GetDescendantSuccess Started..."));

	FString LoginId = "hendra+2@accelbyte.net";
	FString Password = "123456";
	FString Language = "en";
	TArray<FAccelByteModelsFullCategoryInfo> RootCategory;
	bool bRegisterNewPlayerDone = false;
	bool bRegisterNewPlayerSuccess = false;
	bool bLoginDone = false;
	bool bGetRootCategoryDone = false;
	bool bGetRootCategorySuccess = false;
	bool bGetDescendantCategoryDone = false;
	bool bGetDescendantCategorySuccess = false;
	bool bDeleteDone = false;
	bool bDeleteSuccess = false;
	double LastTime;

	FUserLoginCompleteDelegate2 OnLoginComplete = FUserLoginCompleteDelegate2::CreateLambda([&](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetDescendantSuccess] Phase 1 User Login Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bLoginDone = true;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetDescendantSuccess] Phase 1 User Login Started..."));
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FCategoryDefaultCompleteDelegate OnGetRootCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetRootCategoryDone, &bGetRootCategorySuccess, &RootCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetDescendantSuccess] Phase 2 GetRootCategories Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetRootCategoryDone = true;
		bGetRootCategorySuccess = bSuccessful;
		check(bSuccessful);
		if (bSuccessful) { RootCategory = Result; }
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetDescendantSuccess] Phase 2 GetRootCategories Started..."));
	Category::GetRootCategories(Language, OnGetRootCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetRootCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FCategoryDefaultCompleteDelegate OnGetDescendantCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetDescendantCategoryDone, &bGetDescendantCategorySuccess](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetDescendantSuccess] Phase 3 GetDescendantCategories Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetDescendantCategoryDone = true;
		bGetDescendantCategorySuccess = bSuccessful;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetDescendantSuccess] Phase 3 GetDescendantCategories Started..."));
	Category::GetDescendantCategories(Language, RootCategory[0].CategoryPath, OnGetDescendantCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetDescendantCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bGetDescendantCategorySuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetItemSuccess, "Disabled.Category.GetItemSuccess", AutomationFlagMask);
bool FGetItemSuccess::RunTest(const FString & Parameter)
{
	UE_LOG(LogJusticeTest, Log, TEXT("JusticeTest.Category.GetItemSuccess Started..."));

	FString LoginId = "hendra+2@accelbyte.net";
	FString Password = "123456";
	FString Language = "en";
	FString Region = "US";
	TArray<FAccelByteModelsFullCategoryInfo> RootCategory;
	TArray<FAccelByteModelsFullCategoryInfo> ChildCategory;
	TArray<FAccelByteModelsItemInfo> ItemQuery;
	bool bRegisterNewPlayerDone = false;
	bool bRegisterNewPlayerSuccess = false;
	bool bLoginDone = false;
	bool bLoginSuccess = false;
	bool bGetRootCategoryDone = false;
	bool bGetRootCategorySuccess = false;
	bool bGetChildCategoryDone = false;
	bool bGetChildCategorySuccess = false;
	bool bGetItemByQueryDone = false;
	bool bGetItemByQuerySuccess = false;
	bool bGetItemDone = false;
	bool bGetItemSuccess = false;
	bool bDeleteDone = false;
	bool bDeleteSuccess = false;
	double LastTime;

	FUserLoginCompleteDelegate2 OnLoginComplete = FUserLoginCompleteDelegate2::CreateLambda([&bLoginDone, &bLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 1 GetUserAccessTokenWithPasswordGrant Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bLoginDone = true;
		bLoginSuccess = bSuccessful;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 1 GetUserAccessTokenWithPasswordGrant Started..."));
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FCategoryDefaultCompleteDelegate OnGetRootCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetRootCategoryDone, &bGetRootCategorySuccess, &RootCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 2 GetRootCategories Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetRootCategoryDone = true;
		bGetRootCategorySuccess = bSuccessful;
		check(bSuccessful);
		if (bSuccessful) { RootCategory = Result; }
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 2 GetRootCategories Started..."));
	Category::GetRootCategories(Language, OnGetRootCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetRootCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	if (RootCategory.Num() == 0)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 2 GetRootCategories return 0 entries"));
		check(false);
		return false;
	}

	FCategoryDefaultCompleteDelegate OnGetChildCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetChildCategoryDone, &bGetChildCategorySuccess, &ChildCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 3 GetChildCategories Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetChildCategoryDone = true;
		bGetChildCategorySuccess = bSuccessful;
		check(bSuccessful);
		if (bSuccessful) { ChildCategory = Result; }
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 3 GetChildCategories Started..."));
	Category::GetChildCategories(Language, RootCategory[0].CategoryPath, OnGetChildCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetChildCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	if (ChildCategory.Num() == 0)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 3 GetChildCategories return 0 entries"));
		check(false);
		return false;
	}

	FItemCompleteDelegate OnGetItemByQueryComplete = FItemCompleteDelegate::CreateLambda([&bGetItemByQueryDone, &bGetItemByQuerySuccess, &ItemQuery](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsItemInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 4 GetItemsByQuery Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetItemByQueryDone = true;
		bGetItemByQuerySuccess = bSuccessful;
		check(bSuccessful);
		if (bSuccessful) { ItemQuery = Result; }
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 4 GetItemsByQuery Started..."));
	Category::GetItemsByQuery(Language, Region, ChildCategory[0].CategoryPath, TEXT(""), TEXT(""), 0, 1, OnGetItemByQueryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetItemByQueryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FGetItemCompleteDelegate OnGetItemComplete = FGetItemCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FAccelByteModelsItemInfo* Item)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 5 GetItemById Result...: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetItemDone = true;
		bGetItemSuccess = bSuccessful;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemSuccess] Phase 5 GetItemById Started..."));
	Category::GetItemById(ItemQuery[0].ItemId, Region, Language, OnGetItemComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetItemDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bGetItemDone)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetItemByQuerySuccess, "Disabled.Category.GetItemByQuerySuccess", AutomationFlagMask);
bool FGetItemByQuerySuccess::RunTest(const FString & Parameter)
{
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Started..."));

	FString LoginId = "hendra+2@accelbyte.net";
	FString Password = "123456";
	FString Language = "en";
	FString Region = "US";
	TArray<FAccelByteModelsFullCategoryInfo> RootCategory;
	TArray<FAccelByteModelsFullCategoryInfo> ChildCategory;
	bool bRegisterNewPlayerDone = false;
	bool bRegisterNewPlayerSuccess = false;
	bool bLoginDone = false;
	bool bLoginSuccess = false;
	bool bGetRootCategoryDone = false;
	bool bGetRootCategorySuccess = false;
	bool bGetChildCategoryDone = false;
	bool bGetChildCategorySuccess = false;
	bool bGetItemByQueryDone = false;
	bool bGetItemByQuerySuccess = false;
	bool bDeleteDone = false;
	bool bDeleteSuccess = false;
	double LastTime;

	FUserLoginCompleteDelegate2 OnLoginComplete = FUserLoginCompleteDelegate2::CreateLambda([&bLoginDone, &bLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 1 GetUserAccessTokenWithPasswordGrant Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bLoginDone = true;
		bLoginSuccess = bSuccessful;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 1 GetUserAccessTokenWithPasswordGrant Started..."));
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FCategoryDefaultCompleteDelegate OnGetRootCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetRootCategoryDone, &bGetRootCategorySuccess, &RootCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 2 GetRootCategories Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetRootCategoryDone = true;
		bGetRootCategorySuccess = bSuccessful;
		check(bSuccessful);
		if (bSuccessful) { RootCategory = Result; }
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 2 GetRootCategories Started..."));
	Category::GetRootCategories(Language, OnGetRootCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetRootCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	if (RootCategory.Num() == 0)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 2 GetRootCategories return 0 entries"));
		return false;
	}

	FCategoryDefaultCompleteDelegate OnGetChildCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 3 GetChildCategories Result: %s"), bSuccessful && Result.Num() > 0 ? TEXT("Success") : TEXT("Failed"));
		bGetChildCategoryDone = true;
		bGetChildCategorySuccess = bSuccessful;
		check(bSuccessful);
		if (bSuccessful) { ChildCategory = Result; }
		check(Result.Num() > 0);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 3 GetChildCategories, Path=%s Started..."), *RootCategory[0].CategoryPath);
	Category::GetChildCategories(Language, RootCategory[0].CategoryPath, OnGetChildCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetChildCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	if (ChildCategory.Num() == 0)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 3 GetChildCategories return 0 entries"));
		return false;
	}


	FItemCompleteDelegate OnGetItemByQueryComplete = FItemCompleteDelegate::CreateLambda([&bGetItemByQueryDone, &bGetItemByQuerySuccess](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsItemInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 4 GetItemsByQuery Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetItemByQueryDone = true;
		bGetItemByQuerySuccess = bSuccessful;
		check(bSuccessful);
	});
	UE_LOG(LogJusticeTest, Log, TEXT("[JusticeTest.Category.GetItemByQuerySuccess] Phase 4 GetItemsByQuery Started..."));
	Category::GetItemsByQuery(Language, Region, ChildCategory[0].CategoryPath, TEXT(""), TEXT(""), 0, 1, OnGetItemByQueryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetItemByQueryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bGetItemByQuerySuccess)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateOrderSuccess, "Disabled.Purchase.CreateOrderSuccess", AutomationFlagMask);
bool FCreateOrderSuccess::RunTest(const FString & Parameter)
{
	FString LoginId = "SDKtest@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	FString Language = "en";
	FString Region = "US";
	TArray<FAccelByteModelsFullCategoryInfo> RootCategory;
	TArray<FAccelByteModelsFullCategoryInfo> ChildCategory;
	TArray<FAccelByteModelsItemInfo> ItemQuery;
	bool bRegisterNewPlayerDone = false;
	bool bRegisterNewPlayerSuccess = false;
	bool bLoginDone = false;
	bool bLoginSuccess = false;
	bool bGetRootCategoryDone = false;
	bool bGetRootCategorySuccess = false;
	bool bGetChildCategoryDone = false;
	bool bGetChildCategorySuccess = false;
	bool bGetItemByQueryDone = false;
	bool bGetItemByQuerySuccess = false;
	bool bCreateUserProfileDone = false;
	bool bCreateUserProfileSuccess = false;
	bool bCreateOrderDone = false;
	bool bCreateOrderSuccess = false;
	bool bDeleteDone = false;
	bool bDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&bRegisterNewPlayerDone, &bRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> userCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bRegisterNewPlayerDone = true;
		bRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnLoginComplete = FUserLoginCompleteDelegate2::CreateLambda([&bLoginDone, &bLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login to get UserId result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bLoginDone = true;
		bLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FCategoryDefaultCompleteDelegate OnGetRootCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetRootCategoryDone, &bGetRootCategorySuccess, &RootCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Root Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetRootCategoryDone = true;
		bGetRootCategorySuccess = bSuccessful;
		if (bSuccessful) { RootCategory = Result; }
	});
	Category::GetRootCategories(Language, OnGetRootCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetRootCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FCategoryDefaultCompleteDelegate OnGetChildCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetChildCategoryDone, &bGetChildCategorySuccess, &ChildCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Child Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetChildCategoryDone = true;
		bGetChildCategorySuccess = bSuccessful;
		if (bSuccessful) { ChildCategory = Result; }
	});
	Category::GetChildCategories(Language, RootCategory[0].CategoryPath, OnGetChildCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetChildCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FItemCompleteDelegate OnGetItemByQueryComplete = FItemCompleteDelegate::CreateLambda([&bGetItemByQueryDone, &bGetItemByQuerySuccess, &ItemQuery](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsItemInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Item by Query Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetItemByQueryDone = true;
		bGetItemByQuerySuccess = bSuccessful;
		if (bSuccessful) { ItemQuery = Result; }
	});
	Category::GetItemsByQuery(Language, Region, ChildCategory[0].CategoryPath, TEXT(""), TEXT(""), 0, 1, OnGetItemByQueryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetItemByQueryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDefaultCompleteDelegate OnCreateDefaultPlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&bCreateUserProfileDone, &bCreateUserProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default User Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bCreateUserProfileDone = true;
		bCreateUserProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, DisplayName, OnCreateDefaultPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bCreateUserProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	
	FOrderInfoCompleteDelegate OnCreateOrderComplete = FOrderInfoCompleteDelegate::CreateLambda([&bCreateOrderDone, &bCreateOrderSuccess](bool bSuccessful, FString ErrorStr, FAccelByteModelsOrderInfo* Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create New Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bCreateOrderDone = true;
		bCreateOrderSuccess = bSuccessful;
	});
	Purchase::CreateNewOrder(*FJusticeUserToken, 1, ItemQuery[0].ItemId, ItemQuery[0].RegionDatas[0].Price, ItemQuery[0].RegionDatas[0].DiscountedPrice, ItemQuery[0].RegionDatas[0].CurrencyCode, OnCreateOrderComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bCreateOrderDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bCreateOrderSuccess)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserOrderSuccess, "Disabled.Purchase.GetUserOrderSuccess", AutomationFlagMask);
bool FGetUserOrderSuccess::RunTest(const FString & Parameter)
{
	FString LoginId = "SDKtest@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	FString Language = "en";
	FString Region = "US";
    FAccelByteModelsOrderInfo* CreatedOrder = nullptr;
	TArray<FAccelByteModelsFullCategoryInfo> RootCategory;
	TArray<FAccelByteModelsFullCategoryInfo> ChildCategory;
	TArray<FAccelByteModelsItemInfo> ItemQuery;
	bool bRegisterNewPlayerDone = false;
	bool bRegisterNewPlayerSuccess = false;
	bool bLoginDone = false;
	bool bLoginSuccess = false;
	bool bGetRootCategoryDone = false;
	bool bGetRootCategorySuccess = false;
	bool bGetChildCategoryDone = false;
	bool bGetChildCategorySuccess = false;
	bool bGetItemByQueryDone = false;
	bool bGetItemByQuerySuccess = false;
	bool bCreateUserProfileDone = false;
	bool bCreateUserProfileSuccess = false;
	bool bCreateOrderDone = false;
	bool bCreateOrderSuccess = false;
	bool bGetUserOrderDone = false;
	bool bGetUserOrderSuccess = false;
	bool bDeleteDone = false;
	bool bDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&bRegisterNewPlayerDone, &bRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> userCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bRegisterNewPlayerDone = true;
		bRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnLoginComplete = FUserLoginCompleteDelegate2::CreateLambda([&bLoginDone, &bLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login to get UserId result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bLoginDone = true;
		bLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FCategoryDefaultCompleteDelegate OnGetRootCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetRootCategoryDone, &bGetRootCategorySuccess, &RootCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Root Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetRootCategoryDone = true;
		bGetRootCategorySuccess = bSuccessful;
		if (bSuccessful) { RootCategory = Result; }
	});
	Category::GetRootCategories(Language, OnGetRootCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetRootCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FCategoryDefaultCompleteDelegate OnGetChildCategoryComplete = FCategoryDefaultCompleteDelegate::CreateLambda([&bGetChildCategoryDone, &bGetChildCategorySuccess, &ChildCategory](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsFullCategoryInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Child Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetChildCategoryDone = true;
		bGetChildCategorySuccess = bSuccessful;
		if (bSuccessful) { ChildCategory = Result; }
	});
	Category::GetChildCategories(Language, RootCategory[0].CategoryPath, OnGetChildCategoryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetChildCategoryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FItemCompleteDelegate OnGetItemByQueryComplete = FItemCompleteDelegate::CreateLambda([&bGetItemByQueryDone, &bGetItemByQuerySuccess, &ItemQuery](bool bSuccessful, FString ErrorStr, TArray<FAccelByteModelsItemInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get Item by Query Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetItemByQueryDone = true;
		bGetItemByQuerySuccess = bSuccessful;
		if (bSuccessful) { ItemQuery = Result; }
	});
	Category::GetItemsByQuery(Language, Region, ChildCategory[0].CategoryPath, TEXT(""), TEXT(""), 0, 1, OnGetItemByQueryComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetItemByQueryDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDefaultCompleteDelegate OnCreateDefaultPlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&bCreateUserProfileDone, &bCreateUserProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default User Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bCreateUserProfileDone = true;
		bCreateUserProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, DisplayName, OnCreateDefaultPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bCreateUserProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	
	FOrderInfoCompleteDelegate OnCreateOrderComplete = FOrderInfoCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FAccelByteModelsOrderInfo* Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create New Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bCreateOrderDone = true;
		bCreateOrderSuccess = bSuccessful;
		if (bSuccessful) { CreatedOrder = std::move(Result); }
	});
	Purchase::CreateNewOrder(*FJusticeUserToken, 1, ItemQuery[0].ItemId, ItemQuery[0].RegionDatas[0].Price, ItemQuery[0].RegionDatas[0].DiscountedPrice, ItemQuery[0].RegionDatas[0].CurrencyCode, OnCreateOrderComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bCreateOrderDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FOrderInfoCompleteDelegate OnGetUserOrderComplete = FOrderInfoCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErroStr, FAccelByteModelsOrderInfo* Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get User Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetUserOrderDone = true;
		bGetUserOrderSuccess = bSuccessful;
	});
	Purchase::GetUserOrder(CreatedOrder->OrderNo, OnGetUserOrderComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetUserOrderDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bCreateOrderSuccess)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserOrdersSuccess, "Disabled.Purchase.GetUserOrdersSuccess", AutomationFlagMask);
bool FGetUserOrdersSuccess::RunTest(const FString & Parameter)
{
	FString LoginId = "SDKtest@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	EUserAuthTypeJustice AuthType = Email;
	FString Language = "en";
	FString Region = "US";
	int32 Page = 0;
	int32 PageSize = 5;
	TArray<FAccelByteModelsOrderInfo> UserOrders;
	bool bRegisterNewPlayerDone = false;
	bool bRegisterNewPlayerSuccess = false;
	bool bLoginDone = false;
	bool bLoginSuccess = false;
	bool bCreateUserProfileDone = false;
	bool bCreateUserProfileSuccess = false;
	bool bGetUserOrdersDone = false;
	bool bGetUserOrdersSuccess = false;
	bool bDeleteDone = false;
	bool bDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&bRegisterNewPlayerDone, &bRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsUserCreateResponse> userCreateResponse)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bRegisterNewPlayerDone = true;
		bRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::CreateAccount(JusticeGameNamespace, LoginId, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate2 OnLoginComplete = FUserLoginCompleteDelegate2::CreateLambda([&bLoginDone, &bLoginSuccess](bool bSuccessful, FString ErrorStr, TSharedPtr<FAccelByteModelsOAuthToken> token)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login to get UserId result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bLoginDone = true;
		bLoginSuccess = bSuccessful;
	});
	JusticeIdentity::GetUserAccessTokenWithPasswordGrant(JusticeGameNamespace, LoginId, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserId = FJusticeSDKModule::Get().UserToken->UserId;

	FDefaultCompleteDelegate OnCreateDefaultPlayerProfileComplete = FDefaultCompleteDelegate::CreateLambda([&bCreateUserProfileDone, &bCreateUserProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Create Default User Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bCreateUserProfileDone = true;
		bCreateUserProfileSuccess = bSuccessful;
	});
	Platform::CreateDefaultUserProfile(*FJusticeUserToken, DisplayName, OnCreateDefaultPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bCreateUserProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FGetUserOrdersCompleteDelegate OnGetUserOrdersComplete = FGetUserOrdersCompleteDelegate::CreateLambda([&bGetUserOrdersDone, &bGetUserOrdersSuccess, &UserOrders](bool bSuccessful, FString ErroStr, TArray<FAccelByteModelsOrderInfo> Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get User Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetUserOrdersDone = true;
		bGetUserOrdersSuccess = bSuccessful;
		if (bSuccessful) { UserOrders = Result; }
	});
	Purchase::GetUserOrders(Page, PageSize, OnGetUserOrdersComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bGetUserOrdersDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserId, OnDeleteUserComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(UserOrders.Num()==0)
		return true;
	check(bGetUserOrdersDone)
		return true;
}

#pragma region Email Accounts

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserRegisterTest, "JusticeTest.RegisterEmail_ThenLogin_Successful", AutomationFlagMask);
bool FUserRegisterTest::RunTest(const FString & Parameter)
{
	FString LoginID = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@gmail.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	bool bHasDone = false;
	JusticeUser::Register(LoginID, Password, DisplayName, 
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bRegisterSuccessful](bool bSuccessful, FString ErrorStr)
	{
		bRegisterSuccessful = bSuccessful;
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
	JusticeUser::Login(LoginID, Password,
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bLoginSuccessful](bool bSuccessful, FString ErrorStr)
	{
		bLoginSuccessful = bSuccessful;
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

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;

	FDeleteUserDelegate OnDeleteComplete = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccessful = bSuccessful;
	});

	FIntegrationTestModule::DeleteUser(FUserId, OnDeleteComplete);

	LastTime = FPlatformTime::Seconds();

	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bLoginSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserLoginTest, "JusticeTest.LoginEmail_ThenVerify_Successful", AutomationFlagMask);
bool FUserLoginTest::RunTest(const FString & Parameter)
{
	FString LoginID = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@gmail.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	bool bHasDone = false;
	JusticeUser::Register(LoginID, Password, DisplayName,
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bRegisterSuccessful](bool bSuccessful, FString ErrorStr)
	{
		bRegisterSuccessful = bSuccessful;
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
	JusticeUser::Login(LoginID, Password,
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bLoginSuccessful](bool bSuccessful, FString ErrorStr)
	{
		bLoginSuccessful = bSuccessful;
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

	FString VerificationCode = FIntegrationTestModule::GetVerificationCode(LoginID);
	UE_LOG(LogJusticeTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bVerifySuccessful = false;
	bHasDone = false;
	JusticeUser::Verify(VerificationCode, 
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bVerifySuccessful](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Verify result: %s"), bSuccessful ? TEXT("Success") : *ErrorStr);
		bVerifySuccessful = bSuccessful;
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

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;

	FDeleteUserDelegate OnDeleteComplete = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccessful = bSuccessful;
	});

	FIntegrationTestModule::DeleteUser(FUserId, OnDeleteComplete);

	LastTime = FPlatformTime::Seconds();

	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bVerifySuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserResetPasswordTest, "JusticeTest.RegisterEmail_ThenResetPassword_Successful", AutomationFlagMask);
bool FUserResetPasswordTest::RunTest(const FString & Parameter)
{
	FString LoginID = "testeraccelbyte+ue4sdk" + FGuid::NewGuid().ToString(EGuidFormats::Digits) + "@gmail.com";
	FString Password = "old_password";
	FString DisplayName = "testSDK";
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	bool bHasDone = false;
	JusticeUser::Register(LoginID, Password, DisplayName,
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bRegisterSuccessful](bool bSuccessful, FString ErrorStr)
	{
		bRegisterSuccessful = bSuccessful;
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

	bool bForgotPaswordSuccessful = false;
	bHasDone = false;
	JusticeUser::SendVerificationCodeForPasswordReset(LoginID,
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bForgotPaswordSuccessful](bool bSuccessful, FString ErrorStr)
	{
		bForgotPaswordSuccessful = bSuccessful;
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

	check(bForgotPaswordSuccessful);

	FString VerificationCode = FIntegrationTestModule::GetVerificationCode(LoginID);
	UE_LOG(LogJusticeTest, Log, TEXT("Verification code: %s"), *VerificationCode);

	bool bResetPasswordSuccessful = false;
	bHasDone = false;
	Password = "new_password";
	JusticeUser::VerifyPasswordReset(VerificationCode, LoginID, Password,
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bResetPasswordSuccessful](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Reset password result: %s"), bSuccessful ? TEXT("Success") : *ErrorStr);
		bResetPasswordSuccessful = bSuccessful;
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

	check(bResetPasswordSuccessful);

	bool bLoginSuccessful = false;
	bHasDone = false;
	JusticeUser::Login(LoginID, Password,
		FDefaultCompleteDelegate::CreateLambda([&bHasDone, &bLoginSuccessful](bool bSuccessful, FString ErrorStr)
	{
		bLoginSuccessful = bSuccessful;
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

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;

	FDeleteUserDelegate OnDeleteComplete = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete user result: %s"), bSuccessful ? TEXT("Success") : *ErrorStr);
		bDeleteDone = true;
		bDeleteSuccessful = bSuccessful;
	});
	
	FIntegrationTestModule::DeleteUser(FUserId, OnDeleteComplete);

	LastTime = FPlatformTime::Seconds();

	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bResetPasswordSuccessful);

	return true;
}

#pragma endregion

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdSuccess, "Disabled.LoginWithDeviceId.LoginTwiceGetSameUserId", AutomationFlagMask);
bool FLoginWithDeviceIdSuccess::RunTest(const FString & Parameter)
{
	bool bDeviceLoginDone1 = false;
	bool bDeviceLoginSuccessful1 = false;
	bool bDeviceLoginDone2 = false;
	bool bDeviceLoginSuccessful2 = false;
	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	FDefaultCompleteDelegate OnLoginComplete1 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID for the 1st time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeviceLoginDone1 = true;
		bDeviceLoginSuccessful1 = bSuccessful;
	});
	JusticeUser::LoginWithDeviceId(OnLoginComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bDeviceLoginDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = FUserId;
	FJusticeSDKModule::Get().UserToken.Reset();

	FDefaultCompleteDelegate OnLoginComplete2 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID for the 2nd time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeviceLoginDone2 = true;
		bDeviceLoginSuccessful2 = bSuccessful;
	});
	JusticeUser::LoginWithDeviceId(OnLoginComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bDeviceLoginDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = FUserId;

	FDeleteUserDelegate OnDeleteComplete = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccessful = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(SecondUserId, OnDeleteComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bDeviceLoginSuccessful1)
	check(bDeviceLoginSuccessful2)
	check(bDeleteSuccessful)
	check(FirstUserId == SecondUserId && FirstUserId != "" && SecondUserId != "")
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithDeviceIdUniqueIdCreated, "JusticeTest.LoginWithDeviceId.UniqueUserIdCreatedForEachDevice", AutomationFlagMask);
bool FLoginWithDeviceIdUniqueIdCreated::RunTest(const FString & Parameter)
{
	bool bDeviceLoginDone1 = false;
	bool bDeviceLoginSuccessful1 = false;
	bool bDeviceLoginDone2 = false;
	bool bDeviceLoginSuccessful2 = false;
	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	UE_LOG(LogJusticeTest, Log, TEXT("Login first user..."))
	FDefaultCompleteDelegate OnLoginComplete1 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID for the 1st time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeviceLoginDone1 = true;
		bDeviceLoginSuccessful1 = bSuccessful;
	});
	JusticeUser::LoginWithDeviceId(OnLoginComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bDeviceLoginDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = FUserId;

	UE_LOG(LogJusticeTest, Log, TEXT("Deleting first user..."));
	FDeleteUserDelegate OnDeleteComplete1 = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete first user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone1 = true;
		bDeleteSuccessful1 = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(FirstUserId, OnDeleteComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FJusticeSDKModule::Get().UserToken.Reset();
	UE_LOG(LogJusticeTest, Log, TEXT("Logging in second user..."))
	FDefaultCompleteDelegate OnLoginComplete2 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID for the 2nd time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeviceLoginDone2 = true;
		bDeviceLoginSuccessful2 = bSuccessful;
	});
	JusticeUser::LoginWithDeviceId(OnLoginComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bDeviceLoginDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = FUserId;

	UE_LOG(LogJusticeTest, Log, TEXT("Deleting second user..."))
	FDeleteUserDelegate OnDeleteComplete2 = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete second user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone2 = true;
		bDeleteSuccessful2 = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(SecondUserId, OnDeleteComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogJusticeTest, Log, TEXT("Asserting..."))
	check(bDeviceLoginSuccessful1)
	check(bDeviceLoginSuccessful2)
	check(bDeleteSuccessful1)
	check(bDeleteSuccessful2)
	check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "")
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeDeviceAccountSuccess, "JusticeTest.UpgradeHeadlessDeviceAccount.Succes", AutomationFlagMask);
bool FUpgradeDeviceAccountSuccess::RunTest(const FString & Parameter)
{
	FString Email = TEXT("testSDK@example.com");
	FString Password = TEXT("password");
	bool bDeviceLoginDone1 = false;
	bool bDeviceLoginSuccessful1 = false;
	bool bUpgradeDone = false;
	bool bUpgradeSuccessful = false;
	bool bEmailLoginDone = false;
	bool bEmailLoginSuccessful = false;
	bool bUpgradedHeadlessAccountUserIdRemain = false;
	bool bDeviceLoginDone2 = false;
	bool bDeviceLoginSuccessful2 = false;
	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	FString FirstUserId = "";
	FString SecondUserId = "";
	FString ThirdUserId = "";
	double LastTime = 0;

	UE_LOG(LogJusticeTest, Log, TEXT("Logging in with device ID..."));
	FDefaultCompleteDelegate OnDeviceLoginComplete1 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID for the 1st time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeviceLoginDone1 = true;
		bDeviceLoginSuccessful1 = bSuccessful;
	});
	JusticeUser::LoginWithDeviceId(OnDeviceLoginComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bDeviceLoginDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = FUserId;
	
	UE_LOG(LogJusticeTest, Log, TEXT("Upgrading headless account..."));
	FDefaultCompleteDelegate OnUpgradeComplete = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Upgrade headless device account result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bUpgradeDone = true;
		bUpgradeSuccessful = bSuccessful;
	});
	JusticeUser::UpgradeHeadlessUserAccount(Email, Password, OnUpgradeComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bUpgradeDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FJusticeSDKModule::Get().UserToken.Reset();

	UE_LOG(LogJusticeTest, Log, TEXT("Logging in the email account..."));
	FDefaultCompleteDelegate OnLoginEmailComplete = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with upgrade account result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bEmailLoginDone = true;
		bEmailLoginSuccessful = bSuccessful;
	});
	JusticeUser::Login(Email, Password, OnLoginEmailComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bEmailLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bUpgradedHeadlessAccountUserIdRemain = (FirstUserId == FUserId && FUserId != "");
	FJusticeSDKModule::Get().UserToken.Reset();

	UE_LOG(LogJusticeTest, Log, TEXT("Logging in with device account (second attempt)..."));
	FDefaultCompleteDelegate OnDeviceLoginComplete2 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID for the 2nd time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeviceLoginDone2 = true;
		bDeviceLoginSuccessful2 = bSuccessful;
	});
	JusticeUser::LoginWithDeviceId(OnDeviceLoginComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bDeviceLoginDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = FUserId;

	UE_LOG(LogJusticeTest, Log, TEXT("Deleting first user..."));
	FDeleteUserDelegate OnDeleteComplete1 = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete first user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone1 = true;
		bDeleteSuccessful1 = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(FirstUserId, OnDeleteComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogJusticeTest, Log, TEXT("Deleting second user..."));
	FDeleteUserDelegate OnDeleteComplete2 = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete second user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone2 = true;
		bDeleteSuccessful2 = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(SecondUserId, OnDeleteComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamSuccess, "JusticeTest.LoginWithSteam.LoginTwiceGetSameUserId", AutomationFlagMask);
bool FLoginWithSteamSuccess::RunTest(const FString & Parameter)
{
	bool bSteamLoginDone1 = false;
	bool bSteamLoginSuccessful1 = false;
	bool bSteamLoginDone2 = false;
	bool bSteamLoginSuccessful2 = false;
	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	UE_LOG(LogJusticeTest, Log, TEXT("Logging in with your Steam account..."));
	FDefaultCompleteDelegate OnLoginComplete1 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID for the 1st time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSteamLoginDone1 = true;
		bSteamLoginSuccessful1 = bSuccessful;
	});
	JusticeUser::Login(EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), OnLoginComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bSteamLoginDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = FUserId;
	FJusticeSDKModule::Get().UserToken.Reset();

	UE_LOG(LogJusticeTest, Log, TEXT("Logging in with your Steam account (second attempt)..."));
	FDefaultCompleteDelegate OnLoginComplete2 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID for the 2nd time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSteamLoginDone2 = true;
		bSteamLoginSuccessful2 = bSuccessful;
	});
	JusticeUser::Login(EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), OnLoginComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bSteamLoginDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = FUserId;

	UE_LOG(LogJusticeTest, Log, TEXT("Deleting current account..."));
	FDeleteUserDelegate OnDeleteComplete = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccessful = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(SecondUserId, OnDeleteComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bSteamLoginSuccessful1)
	check(bSteamLoginSuccessful2)
	check(bDeleteSuccessful)
	check(FirstUserId == SecondUserId && FirstUserId != "" && SecondUserId != "")
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginWithSteamUniqueIdCreated, "JusticeTest.LoginWithSteam.UniqueUserIdCreatedForSteamAccount", AutomationFlagMask);
bool FLoginWithSteamUniqueIdCreated::RunTest(const FString & Parameter)
{
	bool bSteamLoginDone1 = false;
	bool bSteamLoginSuccessful1 = false;
	bool bSteamLoginDone2 = false;
	bool bSteamLoginSuccessful2 = false;
	bool bDeleteDone1 = false;
	bool bDeleteSuccessful1 = false;
	bool bDeleteDone2 = false;
	bool bDeleteSuccessful2 = false;
	FString FirstUserId = "";
	FString SecondUserId = "";
	double LastTime = 0;

	UE_LOG(LogJusticeTest, Log, TEXT("Logging in first user..."))
		FDefaultCompleteDelegate OnLoginComplete1 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with Steam account for the 1st time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSteamLoginDone1 = true;
		bSteamLoginSuccessful1 = bSuccessful;
	});
	JusticeUser::Login(EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), OnLoginComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bSteamLoginDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FirstUserId = FUserId;

	UE_LOG(LogJusticeTest, Log, TEXT("Deleting first user..."));
	FDeleteUserDelegate OnDeleteComplete1 = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete first user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone1 = true;
		bDeleteSuccessful1 = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(FirstUserId, OnDeleteComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FJusticeSDKModule::Get().UserToken.Reset();
	UE_LOG(LogJusticeTest, Log, TEXT("Logging in second user..."))
		FDefaultCompleteDelegate OnLoginComplete2 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with Steam account for the 2nd time result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSteamLoginDone2 = true;
		bSteamLoginSuccessful2 = bSuccessful;
	});
	JusticeUser::LoginWithDeviceId(OnLoginComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bSteamLoginDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	SecondUserId = FUserId;

	UE_LOG(LogJusticeTest, Log, TEXT("Deleting second user..."))
		FDeleteUserDelegate OnDeleteComplete2 = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete second user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone2 = true;
		bDeleteSuccessful2 = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(SecondUserId, OnDeleteComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogJusticeTest, Log, TEXT("Asserting..."));
	check(bSteamLoginSuccessful1);
	check(bSteamLoginSuccessful2);
	check(bDeleteSuccessful1);
	check(bDeleteSuccessful2);
	check(FirstUserId != SecondUserId && FirstUserId != "" && SecondUserId != "");
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpgradeSteamAccountSuccess, "Disabled.UpgradeHeadlessSteamAccount.Success", AutomationFlagMask);
bool FUpgradeSteamAccountSuccess::RunTest(const FString & Parameter)
{
	FString Email = TEXT("testSDK@example.com");
	FString Password = TEXT("password");
	bool bLoginPlatformDone = false;
	bool bLoginPlatformSuccessful = false;
	bool bUpgradeDone = false;
	bool bUpgradeSuccessful = false;
	bool bLoginEmailDone = false;
	bool bLoginEmailSuccessful = false;
	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	double LastTime = 0;
	FString FirstUserId = TEXT("");

	UE_LOG(LogJusticeTest, Log, TEXT("Logging in Steam account..."))
		FDefaultCompleteDelegate OnLoginPlatformComplete = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with Steam account result: %s"), bSuccessful ? TEXT("Success!") : TEXT("Failed"));
		bLoginPlatformDone = true;
		bLoginPlatformSuccessful = bSuccessful;
	});
	JusticeUser::Login(EPlatformType::Steam, FIntegrationTestModule::GetSteamTicket(), OnLoginPlatformComplete);

	while (!bLoginPlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FirstUserId = FUserId;

	UE_LOG(LogJusticeTest, Log, TEXT("Upgrading headless account..."));
	FDefaultCompleteDelegate OnUpgradeComplete = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Upgrade headless account result: %s"), bSuccessful ? TEXT("Success!") : TEXT("Failed"));
		bUpgradeDone = true;
		bUpgradeSuccessful = bSuccessful;
	});
	JusticeUser::UpgradeHeadlessUserAccount(Email, Password, OnUpgradeComplete);

	while (!bUpgradeDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FJusticeSDKModule::Get().UserToken.Reset();

	UE_LOG(LogJusticeTest, Log, TEXT("Loggin in with upgraded account..."));
	FDefaultCompleteDelegate OnLoginEmailComplete = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with upgrade account result: %s"), bSuccessful ? TEXT("Success!") : TEXT("Failed"));
		bLoginEmailDone = true;
		bLoginEmailSuccessful = bSuccessful;
	});
	JusticeUser::Login(Email, Password, OnLoginEmailComplete);

	while (!bLoginEmailDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogJusticeTest, Log, TEXT("Deleting current account..."));
	FDeleteUserDelegate OnDeleteComplete = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete current account result: %s"), bSuccessful ? TEXT("Success!") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccessful = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(FUserId, OnDeleteComplete);

	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(FirstUserId == FUserId)
		check(bLoginPlatformSuccessful)
		check(bUpgradeSuccessful)
		check(bLoginEmailSuccessful)
		check(bDeleteSuccessful)
		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUserProfileUtilitiesSuccess, "JusticeTest.GetAndUpdateProfile.Success", AutomationFlagMask);
bool FUserProfileUtilitiesSuccess::RunTest(const FString & Parameter)
{
	bool bDeviceLoginDone1 = false;
	bool bDeviceLoginSuccessful1 = false;
	bool bGetProfileDone1 = false;
	bool bUpdateProfileDone = false;
	bool bUpdateProfileSuccessful = false;
	bool bGetProfileSuccessful1 = false;
	bool bGetProfileDone2 = false;
	bool bGetProfileSuccessful2 = false;
	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	
	UserProfileInfoUpdate ProfileUpdate;
	ProfileUpdate.Country = "US";
	ProfileUpdate.Language = "en";
	ProfileUpdate.Timezone = "Asia/Shanghai";
	ProfileUpdate.DateOfBirth = "1999-09-09";
	ProfileUpdate.Email = "test@example.com";
	ProfileUpdate.DisplayName = "TestProfile";
	FUserProfileInfo * UpdatedProfile;
	double LastTime = 0;

	FDefaultCompleteDelegate OnLoginComplete1 = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Login with device ID result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeviceLoginDone1 = true;
		bDeviceLoginSuccessful1 = bSuccessful;
	});
	JusticeUser::LoginWithDeviceId(OnLoginComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bDeviceLoginDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FRequestCurrentPlayerProfileCompleteDelegate OnGetProfileComplete1 = FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FUserProfileInfo * Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get user profile result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetProfileDone1 = true;
		bGetProfileSuccessful1 = bSuccessful;
	});
	JusticeUser::GetProfile(OnGetProfileComplete1);

	LastTime = FPlatformTime::Seconds();
	while (!bGetProfileDone1)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDefaultCompleteDelegate OnUpdateProfileComplete = FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Update user profile result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bUpdateProfileDone = true;
		bUpdateProfileSuccessful = bSuccessful;
	});
	JusticeUser::UpdateProfile(ProfileUpdate, OnUpdateProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bUpdateProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FRequestCurrentPlayerProfileCompleteDelegate OnGetProfileComplete2 = FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FUserProfileInfo * Result)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Get user profile result after updated: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bGetProfileDone2 = true;
		bGetProfileSuccessful2 = bSuccessful;
		UpdatedProfile = Result;
	});
	JusticeUser::GetProfile(OnGetProfileComplete2);

	LastTime = FPlatformTime::Seconds();
	while (!bGetProfileDone2)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteComplete = FDeleteUserDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogJusticeTest, Log, TEXT("Delete user result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bDeleteDone = true;
		bDeleteSuccessful = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(FUserId, OnDeleteComplete);

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bDeviceLoginSuccessful1);
	check(bGetProfileSuccessful2);
	check(bUpdateProfileSuccessful);
	check(UpdatedProfile->DisplayName == ProfileUpdate.DisplayName);
	check(bGetProfileSuccessful2);
	check(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSteamTicket, "Disabled.SteamTicket.Success", AutomationFlagMask);
bool FGetSteamTicket::RunTest(const FString & Parameter)
{
	FString Ticket = FIntegrationTestModule::GetSteamTicket();
	UE_LOG(LogJusticeTest, Log, TEXT("Print Steam Ticket :\r\n%s"), *Ticket);
	check(Ticket != TEXT(""))
		return true;
}

void FIntegrationTestModule::DeleteUser(FString UserId, FDeleteUserDelegate OnComplete)
{
	FCriticalSection Mutex;
	FScopeLock Lock(&Mutex);

	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeGameClientToken.AccessToken);
	FString URL = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s"), *ServerBaseUrl, *JusticeGameNamespace, *UserID);
	FString Verb = TEXT("DELETE");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJusticeHTTP::CreateRequest(Authorization, URL, Verb, ContentType, Accept, Content, FWebRequestResponseDelegate::CreateStatic(FIntegrationTestModule::OnDeleteUserComplete, OnComplete));

	//FString ErrorStr;
	//TSharedRef<IJusticeHttpRequest> Request =  HttpRetrySystem.CreateRequest();
	//TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	//FString BaseUrl = FJusticeSDKModule::Get().BaseUrl;
	//FString Namespace = FJusticeSDKModule::Get().Namespace;
	//if (BaseUrl == "" || Namespace == "") {
	//	UE_LOG(LogJustice, Warning, TEXT("DeleteUser: Url or Namespace is empty"));
	//}

	//Request->SetUrl(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s"), *BaseUrl, *Namespace, *UserId));
	//Request->SetHeader(TEXT("Authorization"), FJusticeHTTP::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	//Request->SetVerb(TEXT("DELETE"));
	//Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	//Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceId());
	//Request->OnProcessRequestComplete().BindStatic(&FIntegrationTestModule::OnDeleteUserComplete, RequestTrace, OnComplete);
	//if (!Request->ProcessRequest())
	//{
	//	ErrorStr = FString::Printf(TEXT("request failed. Url=%s"), *Request->GetUrl());
	//}
	//
	//if (!ErrorStr.IsEmpty())
	//{
	//	UE_LOG(LogJustice, Warning, TEXT("DeleteUser failed. Error=%s"), *ErrorStr);
	//}
}

void FIntegrationTestModule::OnDeleteUserComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, FDeleteUserDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}
	switch (Response->GetResponseCode())
	{
		case EHttpResponseCodes::NoContent:
		{
			UE_LOG(LogJustice, Log, TEXT("OnDeleteUserComplete receive success response "));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		}
		case EHttpResponseCodes::Forbidden:
		{
			ErrorStr = FString::Printf(TEXT("OnDeleteUserComplete Expected Error: Forbidden. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::NotFound:
		{
			ErrorStr = FString::Printf(TEXT("OnDeleteUserComplete Expected Error: Data not found. Code=%d"), Response->GetResponseCode());
			break;
		}
		default:
			ErrorStr = FString::Printf(TEXT("OnDeleteUserComplete Error Code=%d"), Response->GetResponseCode());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnDeleteUserComplete Error=%s ReqTime=%.3f"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

FString FIntegrationTestModule::GetVerificationCode(FString Email)
{
	FString VerificationCodeOutput = TEXT("");
	FString CurrentDirectory = TEXT("");
	CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("TestUtilities/justice-test-utilities-windows_amd64.exe"));
	CurrentDirectory.Replace(TEXT("/"), TEXT("\\"));
	UE_LOG(LogJusticeTest, Log, TEXT("%s"), *CurrentDirectory);
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
	UE_LOG(LogJusticeTest, Log, TEXT("%s"), *CurrentDirectory);
	FWindowsPlatformProcess::ExecProcess(CurrentDirectory.GetCharArray().GetData(), nullptr, nullptr, &SteamHelperOutput, nullptr);
	SteamHelperOutput.Split(TEXT("STEAMTICKET\r\n"), nullptr, &SteamTicket);
	return SteamTicket;
}
#endif