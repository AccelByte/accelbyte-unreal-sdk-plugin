// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "../Models/UserOnlineAccountJustice.h"
#include "Public/JusticeIdentity.h"


const int AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter); 
// Context: EditorContext (run via editor cli) ClientContext (run via external cli)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginSuccessTest, "IntegrationTest.Login.Success", AutomationFlagMask);
bool FLoginSuccessTest::RunTest(const FString & Parameters)
{
	FString LoginID = "hendra@accelbyte.net";
	FString Password = "123456";
	bool isDone = false;
	bool isLoginSuccess = false;
	bool expectedResult = true;

	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update()"));

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token) {
		UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::Login(LoginID, Password, FGrantTypeJustice::PasswordGrant, OnComplete);

	double LastTime = FPlatformTime::Seconds();
	while (!isDone)
	{
		//FHttpModule::Get().GetHttpManager();
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() After Login"));

	return isLoginSuccess == expectedResult;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoginFailedTest, "IntegrationTest.Login.Failed", AutomationFlagMask);
bool FLoginFailedTest::RunTest(const FString & Parameters)
{
	FString LoginID = "hendra@accelbyte.net";
	FString Password = "123456x";
	bool isDone = false;
	bool isLoginSuccess = false;
	bool expectedResult = false;

	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update()"));

	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token) {
		UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
		isDone = true;
		isLoginSuccess = IsSuccess;
	});
	JusticeIdentity::Login(LoginID, Password, FGrantTypeJustice::PasswordGrant, OnComplete);

	double LastTime = FPlatformTime::Seconds();
	while (!isDone)
	{
		//FHttpModule::Get().GetHttpManager();
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() After Login"));

	return isLoginSuccess == expectedResult;
}



//
//
//DEFINE_LATENT_AUTOMATION_COMMAND(FLoginLatentCommand);
//bool FLoginLatentCommand::Update()
//{
//    FString LoginID = "hendra@accelbyte.net";
//    FString Password = "123456";
//    bool isDone = false;
//	bool isLoginSuccess = false;
//
//	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update()"));
//
//	FUserLoginCompleteDelegate OnComplete = FUserLoginCompleteDelegate::CreateLambda([&isDone, &isLoginSuccess](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* token) {
//		UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() Login Result: %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
//		isDone = true;
//		isLoginSuccess = IsSuccess;
//	});
//	JusticeIdentity::Login(LoginID, Password, FGrantTypeJustice::PasswordGrant, OnComplete);
//
//	double LastTime = FPlatformTime::Seconds();
//	while (!isDone) 
//	{
//		//FHttpModule::Get().GetHttpManager();
//		const double AppTime = FPlatformTime::Seconds();
//		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
//		LastTime = AppTime;
//		FPlatformProcess::Sleep(0.5f);
//	}
//
//	UE_LOG(LogTemp, Log, TEXT("FLoginLatentCommand::Update() After Login"));
//
//    return true;
//}
//
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComplexTest, "IntegrationTest.Complex", AutomationFlagMask);
//bool FComplexTest::RunTest(const FString & Parameters)
//{
//	UE_LOG(LogTemp, Log, TEXT("IntegrationTest.Complex - Run Test"));
//
//    ADD_LATENT_AUTOMATION_COMMAND(FLoginLatentCommand);
//    
//    return true;
//}
