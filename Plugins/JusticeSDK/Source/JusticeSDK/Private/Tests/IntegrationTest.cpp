// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Public/Test/IntegrationTest.h"
#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Public/Models/UserOnlineAccountJustice.h"
#include "Public/Services/JusticeIdentity.h"
#include "JusticeSDK.h"
#include "JusticePlatform.h"
#include "stdio.h"

const int32 AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext); 
// Context: EditorContext (run via editor cli) ClientContext (run via external cli)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginSuccessTest, "JusticeTest.User.LoginSuccess", AutomationFlagMask);
bool FLoginSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "hendra@accelbyte.net";
	FString Password = "123456";
	bool isDone = false;
	bool isLoginSuccess = false;

	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update()"));

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token) 
	{
		UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnComplete);

	double LastTime = FPlatformTime::Seconds();
	while (!isDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() After Login"));

	check(isLoginSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginFailedWrongPasswordTest, "JusticeTest.User.LoginFailedWrongPassword", AutomationFlagMask);
bool FLoginFailedWrongPasswordTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDone = false;
	bool bTestSuccessful = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, "test", OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &bTestSuccessful](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		bTestSuccessful = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginFailedUnregisteredEmailTest, "JusticeTest.User.LoginFailedUnregisteredEmail", AutomationFlagMask);
bool FLoginFailedUnregisteredEmailTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDone = false;
	bool bTestSuccessful = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin("sdk@example.com", Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &bTestSuccessful](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		bTestSuccessful = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLogoutSuccessTest, "JusticeTest.User.LogoutSuccess", AutomationFlagMask);
bool FLogoutSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isLogoutDone = false;
	bool isLogoutSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FUserLogoutCompleteDelegate OnUserLogoutResponse = FUserLogoutCompleteDelegate::CreateLambda([&isLogoutDone, &isLogoutSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Logout Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FForgotPasswordSuccessTest, "JusticeTest.User.ForgotPasswordSuccess", AutomationFlagMask);
bool FForgotPasswordSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "hendra@accelbyte.net";
	bool isForgotPasswdDone = false;
	bool isForgotPasswdSuccess = false;
	double LastTime;

	FForgotPasswordCompleteDelegate OnForgotPasswdComplete = FForgotPasswordCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Forgot password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = bSuccessful;
	});
	JusticeIdentity::ForgotPassword(LoginID, OnForgotPasswdComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isForgotPasswdDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogTemp, Log, TEXT("Request forgot password done"));

	check(isForgotPasswdSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FForgotPasswordFailedEmailNotVerifiedTest, "JusticeTestJusticeTest.User.ForgotPasswordFailedEmailNotVerified", AutomationFlagMask);
bool FForgotPasswordFailedEmailNotVerifiedTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isForgotPasswdDone = false;
	bool isForgotPasswdSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	AddExpectedError(TEXT("OnForgotPasswordResponse"), EAutomationExpectedErrorFlags::Contains, 1);
	FForgotPasswordCompleteDelegate OnForgotPasswordResponse = FForgotPasswordCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Forgot password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = bSuccessful;
	});
	JusticeIdentity::ForgotPassword(LoginID, OnForgotPasswordResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isForgotPasswdDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token) 
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr) 
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FForgotPasswordFailedInvalidUserTest, "JusticeTest.User.ForgotPasswordFailedInvalidUser", AutomationFlagMask);
bool FForgotPasswordFailedInvalidUserTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	bool isForgotPasswdDone = false;
	bool isForgotPasswdSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnForgotPasswordResponse"), EAutomationExpectedErrorFlags::Contains, 1);
	FForgotPasswordCompleteDelegate OnForgotPasswordResponse = FForgotPasswordCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Forgot password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = bSuccessful;
	});
	JusticeIdentity::ForgotPassword(LoginID, OnForgotPasswordResponse);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterNewPlayerSuccessTest, "JusticeTest.User.RegisterNewPlayerSuccess", AutomationFlagMask);
bool FRegisterNewPlayerSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName,AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterNewPlayerFailedUserExistTest, "JusticeTest.User.RegisterNewPlayerFailedUserExist", AutomationFlagMask);
bool FRegisterNewPlayerFailedUserExistTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
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
	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FRegisterPlayerCompleteDelegate OnRegisterPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterPlayerDone, &isRegisterPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterPlayerDone = true;
		isRegisterPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterNewPlayerInvalidEmailTest, "JusticeTest.User.RegisterNewPlayerFailedInvalidEmail", AutomationFlagMask);
bool FRegisterNewPlayerInvalidEmailTest::RunTest(const FString & Parameters)
{
	FString LoginID = "test";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnRegisterNewPlayerResponse"), EAutomationExpectedErrorFlags::Contains, 1);
	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateDefaultPlayerProfileSuccessTest, "JusticeTest.User.CreateDefaultPlayerProfileSuccess", AutomationFlagMask);
bool FCreateDefaultPlayerProfileSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FUpdatePlayerProfileCompleteDelegate OnUpdatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::CreateDefaultPlayerProfile(LoginID,DisplayName, OnUpdatePlayerProfileComplete);

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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateDefaultPlayerProfileFailedEmptyDisplayNameTest, "JusticeTest.CreateDefaultPlayerProfileFailedEmptyDisplayname", AutomationFlagMask);
bool FCreateDefaultPlayerProfileFailedEmptyDisplayNameTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token) 
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;
	
	AddExpectedError(TEXT("OnCreateDefaultPlayerProfileComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FUpdatePlayerProfileCompleteDelegate OnCreateDefaultPlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::CreateDefaultPlayerProfile(LoginID, "", OnCreateDefaultPlayerProfileComplete);

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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateDefaultPlayerProfileFailedUnregisteredUserTest, "JusticeTest.User.CreateDefaultPlayerProfileFailedUnregisteredUser", AutomationFlagMask);
bool FCreateDefaultPlayerProfileFailedUnregisteredUserTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FString AuthType = "EMAILPASSWD";
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnCreateDefaultPlayerProfileComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FUpdatePlayerProfileCompleteDelegate OnCreateDefaultPlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::CreateDefaultPlayerProfile(LoginID, DisplayName, OnCreateDefaultPlayerProfileComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetPlayerProfileSuccessTest, "JusticeTest.User.GetPlayerProfileSuccess", AutomationFlagMask);
bool FGetPlayerProfileSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
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

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FUpdatePlayerProfileCompleteDelegate OnUpdatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::CreateDefaultPlayerProfile(LoginID, DisplayName, OnUpdatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FReqestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfileComplete = FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool bSuccessful, FString ErrorStr, UserProfileInfo userProfile)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Current Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::RequestCurrentPlayerProfile(OnGetPlayerProfileComplete);

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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetPlayerProfileSuccessUnregisteredUserTest, "JusticeTest.User.GetPlayerProfileSuccessUnregisteredUser", AutomationFlagMask);
bool FGetPlayerProfileSuccessUnregisteredUserTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isGetPlayerProfileDone = false;
	bool isGetPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FReqestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfileComplete = FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool bSuccessful, FString ErrorStr, UserProfileInfo userProfile)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Current Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::RequestCurrentPlayerProfile(OnGetPlayerProfileComplete);

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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpdatePlayerProfileSuccessTest, "JusticeTest.User.UpdatePlayerProfileSuccess", AutomationFlagMask);
bool FUpdatePlayerProfileSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
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

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerResponse = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnUserLoginResponse = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnUserLoginResponse);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FUpdatePlayerProfileCompleteDelegate OnCreatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::CreateDefaultPlayerProfile(LoginID, DisplayName, OnCreatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString userID = FJusticeSDKModule::Get().UserToken->UserID;

	FUpdatePlayerProfileCompleteDelegate OnUpdatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isUpdatePlayerProfileDone, &isUpdatePlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Update Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isUpdatePlayerProfileDone = true;
		isUpdatePlayerProfileSuccess = bSuccessful;
	});
	UserProfileInfo newUserProfileinfo;
	newUserProfileinfo.Email = LoginID;
	newUserProfileinfo.DisplayName = UpdatedDisplayName;
	newUserProfileinfo.UserID = userID;
	newUserProfileinfo.AvatarLargeUrl = FJusticeSDKModule::Get().UserProfile->AvatarLargeUrl;
	newUserProfileinfo.AvatarUrl = FJusticeSDKModule::Get().UserProfile->AvatarUrl;
	newUserProfileinfo.AvatarSmallUrl = FJusticeSDKModule::Get().UserProfile->AvatarSmallUrl;
	newUserProfileinfo.Namespace = FJusticeSDKModule::Get().UserProfile->Namespace;
	newUserProfileinfo.FirstName = "SDK";
	newUserProfileinfo.LastName = "integration";
	newUserProfileinfo.Country = FJusticeSDKModule::Get().UserProfile->Country;
	newUserProfileinfo.Status = FJusticeSDKModule::Get().UserProfile->Status;
	newUserProfileinfo.Language = "en";
	newUserProfileinfo.Timezone = "UTC";
	newUserProfileinfo.DateOfBirth = "2018-05-23";
	newUserProfileinfo.CustomAttributes = FJusticeSDKModule::Get().UserProfile->CustomAttributes;

	JusticePlatform::UpdatePlayerProfile(newUserProfileinfo, OnUpdatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isUpdatePlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FReqestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfile1Complete = FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool bSuccessful, FString ErrorStr, UserProfileInfo userProfile)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Current Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::RequestCurrentPlayerProfile(OnGetPlayerProfile1Complete);
	
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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUpdatePlayerProfileFailedInvalidRequestTest, "JusticeTest.User.UpdatePlayerProfileFailedInvalidRequest", AutomationFlagMask);
bool FUpdatePlayerProfileFailedInvalidRequestTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isUpdatePlayerProfileDone = false;
	bool isUpdatePlayerProfileSuccess = false;
	bool isGetPlayerProfileDone = false;
	bool isGetPlayerProfileSuccess = false;
	double LastTime;

	FUpdatePlayerProfileCompleteDelegate OnCreateDefaultPlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::CreateDefaultPlayerProfile(LoginID, DisplayName, OnCreateDefaultPlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isCreateDefaultPlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString userID = FJusticeSDKModule::Get().UserToken->UserID;

	AddExpectedError(TEXT("OnUpdatePlayerProfileComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FUpdatePlayerProfileCompleteDelegate OnUpdatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isUpdatePlayerProfileDone, &isUpdatePlayerProfileSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Update Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isUpdatePlayerProfileDone = true;
		isUpdatePlayerProfileSuccess = bSuccessful;
	});
	UserProfileInfo newUserProfileinfo;
	newUserProfileinfo.Email = LoginID;
	newUserProfileinfo.DisplayName = "updated";
	newUserProfileinfo.UserID = userID;

	JusticePlatform::UpdatePlayerProfile(newUserProfileinfo, OnUpdatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isUpdatePlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FReqestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfile1Complete = FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool bSuccessful, FString ErrorStr, UserProfileInfo userProfile)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Current Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = bSuccessful;
	});
	JusticePlatform::RequestCurrentPlayerProfile(OnGetPlayerProfile1Complete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUserLinkedPlatformTest, "JusticeTest.User.GetUserLinkedPlatform", AutomationFlagMask);
bool FGetUserLinkedPlatformTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isGetUserLinkedPlatformDone = false;
	bool isGetUserLinkedPlatformSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* userCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess](bool bSuccessful, FString ErrorStr, TArray<LinkedPlatform> Response)
	{
		UE_LOG(LogTemp, Log, TEXT("Get linked platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = bSuccessful;
	});
	JusticeIdentity::GetLinkedPlatform(OnGetLinkedPlatformComplete);

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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLinkDevicePlatformSuccessTest, "JusticeTest.User.LinkDevicePlatform", AutomationFlagMask);
bool FLinkDevicePlatformSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	FString PlatformID = "device";
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

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* userCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FLinkPlatformCompleteDelegate OnLinkDevicePlatformComplete = FLinkPlatformCompleteDelegate::CreateLambda([&isLinkDevicePlatformDone, &isLinkDevicePlatformSuccess](bool bSuccessful, FString ErrorStr) 
	{
		UE_LOG(LogTemp, Log, TEXT("Link device platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLinkDevicePlatformDone = true;
		isLinkDevicePlatformSuccess = bSuccessful;
	});
	JusticeIdentity::LinkPlatform(PlatformID, DeviceId, OnLinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess, &isDeviceLinkedSuccessfully](bool bSuccessful, FString ErrorStr, TArray<LinkedPlatform> Response)
	{
		UE_LOG(LogTemp, Log, TEXT("Get linked platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = bSuccessful;
		if (Response[0].PlatformID.Contains("device"))
		{
			isDeviceLinkedSuccessfully = true;
		}
	});
	JusticeIdentity::GetLinkedPlatform(OnGetLinkedPlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isGetUserLinkedPlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUnlinkPlatformCompleteDelegate OnUnlinkDevicePlatformComplete = FUnlinkPlatformCompleteDelegate::CreateLambda([&isUnlinkDevicePlatformDone, &isUnlinkDevicePlatformSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Unlink device platform: %s"), isUnlinkDevicePlatformDone ? TEXT("Success") : TEXT("Failed"));
		isUnlinkDevicePlatformDone = true;
		isUnlinkDevicePlatformSuccess = bSuccessful;
	});
	JusticeIdentity::UnlinkPlatform("device", OnUnlinkDevicePlatformComplete);

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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnlinkDevicePlatformSuccessTest, "JusticeTest.User.UnlinkDevicePlatform", AutomationFlagMask);
bool FUnlinkDevicePlatformSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FUserAuthTypeJustice AuthType = Email;
	FString PlatformID = "device";
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

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool bSuccessful, FString ErrorStr, UserCreateResponse* userCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = bSuccessful;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = bSuccessful;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserID;

	FLinkPlatformCompleteDelegate OnLinkDevicePlatformComplete = FLinkPlatformCompleteDelegate::CreateLambda([&isLinkDevicePlatformDone, &isLinkDevicePlatformSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Link device platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isLinkDevicePlatformDone = true;
		isLinkDevicePlatformSuccess = bSuccessful;
	});
	JusticeIdentity::LinkPlatform(PlatformID, DeviceId, OnLinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUnlinkPlatformCompleteDelegate OnUnlinkDevicePlatformComplete = FUnlinkPlatformCompleteDelegate::CreateLambda([&isUnlinkDevicePlatformDone, &isUnlinkDevicePlatformSuccess](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Unlink device platform: %s"), isUnlinkDevicePlatformDone ? TEXT("Success") : TEXT("Failed"));
		isUnlinkDevicePlatformDone = true;
		isUnlinkDevicePlatformSuccess = bSuccessful;
	});
	JusticeIdentity::UnlinkPlatform("device", OnUnlinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isUnlinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess, &isDeviceUnlinkedSuccesfully](bool bSuccessful, FString ErrorStr, TArray<LinkedPlatform> Response)
	{
		UE_LOG(LogTemp, Log, TEXT("Get linked platform: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = bSuccessful;
		if (Response.Num() == 0)
		{
			isDeviceUnlinkedSuccesfully = true;
		}
	});
	JusticeIdentity::GetLinkedPlatform(OnGetLinkedPlatformComplete);

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
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = bSuccessful;
	});
	FIntegrationTestModule::DeleteUser(UserID, OnDeleteUserComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSendTelemetrySuccessTest, "JusticeTest.User.TelemetrySendSuccess", AutomationFlagMask);
bool FSendTelemetrySuccessTest::RunTest(const FString & Parameters)
{
	TelemetryEventDataExample TelemetryData;
	TelemetryData.ExampleField1 = 1555;
	TelemetryData.ExampleField2 = "1555";
	TelemetryData.ExampleField3 = 1555;
	FGuid Guid = FGuid::NewGuid();
	FString UUID = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);
	TelemetryEvent Telemetry;
	Telemetry.AppID = 1555;
	Telemetry.ClientID = FJusticeSDKModule::GetModule().ClientID;
	Telemetry.Data = TelemetryData.ToJson();
	Telemetry.DeviceID = FPlatformMisc::GetDeviceId();
	Telemetry.EventID = 1555;
	Telemetry.EventLevel = 1555;
	Telemetry.EventTime = FDateTime::Now().ToIso8601();
	Telemetry.EventType = 1555;
	Telemetry.UserID = "exampleUserID";
	Telemetry.UUID = UUID;
	Telemetry.UX = 1555;
	bool bSendTelemetryDone = false;
	bool bSendTelemetrySuccessful = false;
	double LastTime;

	FSendTelemetryCompleteDelegate OnSendTelemetryComplete = FSendTelemetryCompleteDelegate::CreateLambda([&bSendTelemetryDone, &bSendTelemetrySuccessful](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Send Telemetry Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSendTelemetryDone = true;
		bSendTelemetrySuccessful = bSuccessful;
	});
	JusticeIdentity::SendTelemetry(Telemetry, OnSendTelemetryComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSendTelemetryIncompleteDataSuccessTest, "JusticeTest.User.TelemetrySendIncompleteDataSuccess", AutomationFlagMask);
bool FSendTelemetryIncompleteDataSuccessTest::RunTest(const FString & Parameters)
{
	TelemetryEventDataExample TelemetryData;
	FGuid Guid = FGuid::NewGuid();
	FString UUID = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);
	TelemetryEvent Telemetry;
	Telemetry.AppID = 1555;
	Telemetry.ClientID = FJusticeSDKModule::GetModule().ClientID;
	Telemetry.Data = TelemetryData.ToJson();
	Telemetry.DeviceID = FPlatformMisc::GetDeviceId();
	Telemetry.EventID = 1555;
	Telemetry.EventLevel = 1555;
	Telemetry.EventTime = FDateTime::Now().ToIso8601();
	Telemetry.EventType = 1555;
	Telemetry.UserID = "exampleUserID";
	Telemetry.UUID = UUID;
	Telemetry.UX = 1555;
	bool bSendTelemetryIncompleteDataDone = false;
	bool bSendTelemetryIncompleteDataSuccessful = false;
	double LastTime;

	FSendTelemetryCompleteDelegate OnSendTelemetryComplete = FSendTelemetryCompleteDelegate::CreateLambda([&bSendTelemetryIncompleteDataDone, &bSendTelemetryIncompleteDataSuccessful](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Send Telemetry With Incomplete Telemetry Data Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSendTelemetryIncompleteDataDone = true;
		bSendTelemetryIncompleteDataSuccessful = bSuccessful;
	});
	JusticeIdentity::SendTelemetry(Telemetry, OnSendTelemetryComplete);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSendTelemetryRandomPathSuccessTest, "JusticeTest.User.TelemetrySendToRandomPathSuccess", AutomationFlagMask);
bool FSendTelemetryRandomPathSuccessTest::RunTest(const FString & Parameters)
{
	TelemetryEventDataExample TelemetryData;
	TelemetryData.ExampleField1 = 1555;
	TelemetryData.ExampleField2 = "1555";
	TelemetryData.ExampleField3 = 1555;
	FGuid Guid = FGuid::NewGuid();
	FString UUID = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);
	TelemetryEvent Telemetry;
	Telemetry.AppID = rand();
	Telemetry.ClientID = FJusticeSDKModule::GetModule().ClientID;
	Telemetry.Data = TelemetryData.ToJson();
	Telemetry.DeviceID = FPlatformMisc::GetDeviceId();
	Telemetry.EventID = rand();
	Telemetry.EventLevel = rand();
	Telemetry.EventTime = FDateTime::Now().ToIso8601();
	Telemetry.EventType = rand();
	Telemetry.UserID = "exampleUserID";
	Telemetry.UUID = UUID;
	Telemetry.UX = 1555;
	bool bSendTelemetryRandomPathDone = false;
	bool bSendTelemetryRandomPathSuccessful = false;
	double LastTime;

	FSendTelemetryCompleteDelegate OnSendTelemetryComplete = FSendTelemetryCompleteDelegate::CreateLambda([&bSendTelemetryRandomPathDone, &bSendTelemetryRandomPathSuccessful](bool bSuccessful, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Send Telemetry To Random Path Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
		bSendTelemetryRandomPathDone = true;
		bSendTelemetryRandomPathSuccessful = bSuccessful;
	});
	JusticeIdentity::SendTelemetry(Telemetry, OnSendTelemetryComplete);

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

void FIntegrationTestModule::DeleteUser(FString UserID, FDeleteUserDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	if (BaseURL == "" || Namespace == "") {
		UE_LOG(LogJustice, Warning, TEXT("DeleteUser: URL or Namespace is empty"));
	}

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s"), *BaseURL, *Namespace, *UserID));
	Request->SetHeader(TEXT("Authorization"), FJusticeHTTP::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("DELETE"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->OnProcessRequestComplete().BindStatic(&FIntegrationTestModule::OnDeleteUserComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	//UE_LOG(LogJustice, VeryVerbose, TEXT("DeleteUser Sucessful, XRayID= %s"), *RequestTrace->ToString());
	if (!ErrorStr.IsEmpty())
	{
		//UE_LOG(LogJustice, Warning, TEXT("DeleteUser failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void FIntegrationTestModule::OnDeleteUserComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FDeleteUserDelegate OnComplete)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("OnDeleteUserComplete: request failed");
	}
	else
	{
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
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnDeleteUserComplete Error=%s ReqTime=%.3f"), *ErrorStr, Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}