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

const int AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext); 
// Context: EditorContext (run via editor cli) ClientContext (run via external cli)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginSuccessTest, "JusticeTest.User.LoginSuccess", AutomationFlagMask);
bool FLoginSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "hendra@accelbyte.net";
	FString Password = "123456";
	bool isDone = false;
	bool isLoginSuccess = false;

	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update()"));

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token) 
	{
		UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		isLoginSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDone = false;
	bool isSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, "test", OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &isSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		isSuccess = IsSuccess;
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
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDone = false;
	bool isSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin("sdk@example.com", Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &isSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		isSuccess = IsSuccess;
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
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isLogoutDone = false;
	bool isLogoutSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FUserLogoutCompleteDelegate OnLogoutComplete = FUserLogoutCompleteDelegate::CreateLambda([&isLogoutDone, &isLogoutSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Logout Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLogoutDone = true;
		isLogoutSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogout(OnLogoutComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLogoutDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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

	FForgotPasswordCompleteDelegate OnForgotPasswdComplete = FForgotPasswordCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Forgot password Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isForgotPasswdDone = false;
	bool isForgotPasswdSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	AddExpectedError(TEXT("OnForgotPasswordComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FForgotPasswordCompleteDelegate OnForgotPasswordComplete = FForgotPasswordCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Forgot password Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = IsSuccess;
	});
	JusticeIdentity::ForgotPassword(LoginID, OnForgotPasswordComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isForgotPasswdDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token) 
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr) 
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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

	AddExpectedError(TEXT("OnForgotPasswordComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FForgotPasswordCompleteDelegate OnForgotPasswordComplete = FForgotPasswordCompleteDelegate::CreateLambda([&isForgotPasswdDone, &isForgotPasswdSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Forgot password Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isForgotPasswdDone = true;
		isForgotPasswdSuccess = IsSuccess;
	});
	JusticeIdentity::ForgotPassword(LoginID, OnForgotPasswordComplete);

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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
	});
	JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName,AuthType, OnRegisterNewPlayerComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isRegisterNewPlayerDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isRegisterPlayerDone = false;
	bool isRegisterPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnRegisterNewPlayerComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FRegisterPlayerCompleteDelegate OnRegisterPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterPlayerDone, &isRegisterPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterPlayerDone = true;
		isRegisterPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	double LastTime;

	AddExpectedError(TEXT("OnRegisterNewPlayerComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	check(!isRegisterNewPlayerSuccess)
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateDefaultPlayerProfileSuccessTest, "JusticeTest.User.CreateDefaultPlayerProfileSuccess", AutomationFlagMask);
bool FCreateDefaultPlayerProfileSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "testSDK@example.com";
	FString Password = "testtest";
	FString DisplayName = "testSDK";
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FUpdatePlayerProfileCompleteDelegate OnUpdatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = IsSuccess;
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

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token) 
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;
	
	AddExpectedError(TEXT("OnCreateDefaultPlayerProfileComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FUpdatePlayerProfileCompleteDelegate OnCreateDefaultPlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = IsSuccess;
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

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr) 
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FUpdatePlayerProfileCompleteDelegate OnCreateDefaultPlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
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

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FUpdatePlayerProfileCompleteDelegate OnUpdatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = IsSuccess;
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

	FReqestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfileComplete = FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool IsSuccess, FString ErrorStr, UserProfileInfo userProfile)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Current Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = IsSuccess;
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

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isGetPlayerProfileDone = false;
	bool isGetPlayerProfileSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FReqestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfileComplete = FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool IsSuccess, FString ErrorStr, UserProfileInfo userProfile)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Current Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = IsSuccess;
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

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
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

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnLoginComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::UserLogin(LoginID, Password, OnLoginComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLoginDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FUpdatePlayerProfileCompleteDelegate OnCreatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = IsSuccess;
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
	FString userID = FJusticeSDKModule::Get().UserToken->UserId;

	FUpdatePlayerProfileCompleteDelegate OnUpdatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isUpdatePlayerProfileDone, &isUpdatePlayerProfileSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Update Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isUpdatePlayerProfileDone = true;
		isUpdatePlayerProfileSuccess = IsSuccess;
	});
	UserProfileInfo newUserProfileinfo;
	newUserProfileinfo.Email = LoginID;
	newUserProfileinfo.DisplayName = UpdatedDisplayName;
	newUserProfileinfo.UserId = userID;
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

	FReqestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfile1Complete = FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool IsSuccess, FString ErrorStr, UserProfileInfo userProfile)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Current Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = IsSuccess;
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

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isCreateDefaultPlayerProfileDone = false;
	bool isCreateDefaultPlayerProfileSuccess = false;
	bool isUpdatePlayerProfileDone = false;
	bool isUpdatePlayerProfileSuccess = false;
	bool isGetPlayerProfileDone = false;
	bool isGetPlayerProfileSuccess = false;
	double LastTime;

	FUpdatePlayerProfileCompleteDelegate OnCreateDefaultPlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isCreateDefaultPlayerProfileDone, &isCreateDefaultPlayerProfileSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Default Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isCreateDefaultPlayerProfileDone = true;
		isCreateDefaultPlayerProfileSuccess = IsSuccess;
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
	FString userID = FJusticeSDKModule::Get().UserToken->UserId;

	AddExpectedError(TEXT("OnUpdatePlayerProfileComplete"), EAutomationExpectedErrorFlags::Contains, 1);
	FUpdatePlayerProfileCompleteDelegate OnUpdatePlayerProfileComplete = FUpdatePlayerProfileCompleteDelegate::CreateLambda([&isUpdatePlayerProfileDone, &isUpdatePlayerProfileSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Update Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isUpdatePlayerProfileDone = true;
		isUpdatePlayerProfileSuccess = IsSuccess;
	});
	UserProfileInfo newUserProfileinfo;
	newUserProfileinfo.Email = LoginID;
	newUserProfileinfo.DisplayName = "updated";
	newUserProfileinfo.UserId = userID;

	JusticePlatform::UpdatePlayerProfile(newUserProfileinfo, OnUpdatePlayerProfileComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isUpdatePlayerProfileDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FReqestCurrentPlayerProfileCompleteDelegate OnGetPlayerProfile1Complete = FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&isGetPlayerProfileDone, &isGetPlayerProfileSuccess](bool IsSuccess, FString ErrorStr, UserProfileInfo userProfile)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Current Player Profile Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isGetPlayerProfileDone = true;
		isGetPlayerProfileSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
	bool isRegisterNewPlayerDone = false;
	bool isRegisterNewPlayerSuccess = false;
	bool isLoginDone = false;
	bool isLoginSuccess = false;
	bool isGetUserLinkedPlatformDone = false;
	bool isGetUserLinkedPlatformSuccess = false;
	bool isDeleteDone = false;
	bool isDeleteSuccess = false;
	double LastTime;

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
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
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess](bool IsSuccess, FString ErrorStr, TArray<LinkedPlatform> Response)
	{
		UE_LOG(LogTemp, Log, TEXT("Get linked platform: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = IsSuccess;
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

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
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

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
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
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FLinkPlatformCompleteDelegate OnLinkDevicePlatformComplete = FLinkPlatformCompleteDelegate::CreateLambda([&isLinkDevicePlatformDone, &isLinkDevicePlatformSuccess](bool IsSuccess, FString ErrorStr) 
	{
		UE_LOG(LogTemp, Log, TEXT("Link device platform: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLinkDevicePlatformDone = true;
		isLinkDevicePlatformSuccess = IsSuccess;
	});
	JusticeIdentity::LinkPlatform(PlatformId, DeviceId, OnLinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess, &isDeviceLinkedSuccessfully](bool IsSuccess, FString ErrorStr, TArray<LinkedPlatform> Response)
	{
		UE_LOG(LogTemp, Log, TEXT("Get linked platform: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = IsSuccess;
		if (Response[0].PlatformId.Contains("device"))
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

	FUnlinkPlatformCompleteDelegate OnUnlinkDevicePlatformComplete = FUnlinkPlatformCompleteDelegate::CreateLambda([&isUnlinkDevicePlatformDone, &isUnlinkDevicePlatformSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Unlink device platform: %s"), isUnlinkDevicePlatformDone ? TEXT("Success") : TEXT("Failed"));
		isUnlinkDevicePlatformDone = true;
		isUnlinkDevicePlatformSuccess = IsSuccess;
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

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	FString AuthType = "EMAILPASSWD";
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

	FRegisterPlayerCompleteDelegate OnRegisterNewPlayerComplete = FRegisterPlayerCompleteDelegate::CreateLambda([&isRegisterNewPlayerDone, &isRegisterNewPlayerSuccess](bool IsSuccess, FString ErrorStr, UUserCreateResponse* UserCreateResponse)
	{
		UE_LOG(LogTemp, Log, TEXT("Register New Player Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isRegisterNewPlayerDone = true;
		isRegisterNewPlayerSuccess = IsSuccess;
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

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isLoginDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token)
	{
		UE_LOG(LogTemp, Log, TEXT("Login to get UserID result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLoginDone = true;
		isLoginSuccess = IsSuccess;
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
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	FLinkPlatformCompleteDelegate OnLinkDevicePlatformComplete = FLinkPlatformCompleteDelegate::CreateLambda([&isLinkDevicePlatformDone, &isLinkDevicePlatformSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Link device platform: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isLinkDevicePlatformDone = true;
		isLinkDevicePlatformSuccess = IsSuccess;
	});
	JusticeIdentity::LinkPlatform(PlatformId, DeviceId, OnLinkDevicePlatformComplete);

	LastTime = FPlatformTime::Seconds();
	while (!isLinkDevicePlatformDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	FUnlinkPlatformCompleteDelegate OnUnlinkDevicePlatformComplete = FUnlinkPlatformCompleteDelegate::CreateLambda([&isUnlinkDevicePlatformDone, &isUnlinkDevicePlatformSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Unlink device platform: %s"), isUnlinkDevicePlatformDone ? TEXT("Success") : TEXT("Failed"));
		isUnlinkDevicePlatformDone = true;
		isUnlinkDevicePlatformSuccess = IsSuccess;
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

	FGetLinkedPlatformCompleteDelegate OnGetLinkedPlatformComplete = FGetLinkedPlatformCompleteDelegate::CreateLambda([&isGetUserLinkedPlatformDone, &isGetUserLinkedPlatformSuccess, &isDeviceUnlinkedSuccesfully](bool IsSuccess, FString ErrorStr, TArray<LinkedPlatform> Response)
	{
		UE_LOG(LogTemp, Log, TEXT("Get linked platform: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isGetUserLinkedPlatformDone = true;
		isGetUserLinkedPlatformSuccess = IsSuccess;
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

	FDeleteUserDelegate OnDeleteUserComplete = FDeleteUserDelegate::CreateLambda([&isDeleteDone, &isDeleteSuccess](bool IsSuccess, FString ErrorStr)
	{
		UE_LOG(LogTemp, Log, TEXT("Delete User Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDeleteDone = true;
		isDeleteSuccess = IsSuccess;
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
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("DELETE"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->OnProcessRequestComplete().BindStatic(&FIntegrationTestModule::OnDeleteUserComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG(LogJustice, VeryVerbose, TEXT("DeleteUser Sucessful, XRayID= %s"), *RequestTrace->ToString());
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("DeleteUser failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
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