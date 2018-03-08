// Fill out your copyright notice in the Description page of Project Settings.

#include "JusticeIdentityFunctions.h"

void UJusticeIdentityFunctions::UserLogin(FString LoginId, FString Password, FUserLoginCompleteDynamicDelegate OnComplete)
{
	FUserLoginCompleteDelegate LoginCompleteDelegate;
	LoginCompleteDelegate.BindLambda([OnComplete](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* Token) {
		OnComplete.Execute(IsSuccess, ErrorStr, Token);
	});
	JusticeIdentity::UserLogin(LoginId, Password, LoginCompleteDelegate);
}

void UJusticeIdentityFunctions::UserLogout(FUserLogoutCompleteDynamicDelegate OnComplete)
{
	FUserLogoutCompleteDelegate LogoutCompleteDelegate;
	LogoutCompleteDelegate.BindLambda([OnComplete](bool IsSuccess, FString ErrorStr) {
		OnComplete.Execute(IsSuccess, ErrorStr);
	});
	JusticeIdentity::UserLogout(LogoutCompleteDelegate);
}

void UJusticeIdentityFunctions::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::RegisterNewPlayer(UserId, Password, DisplayName, AuthType, 
		FRegisterPlayerCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, UUserCreateResponse* Response) {
		OnComplete.Execute(IsSuccess, ErrorStr, Response);
	}));

}

void UJusticeIdentityFunctions::VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::VerifyNewPlayer(UserId, VerificationCode,
		FVerifyNewPlayerCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr) {
		OnComplete.Execute(IsSuccess, ErrorStr);
	}));

}

void UJusticeIdentityFunctions::ForgotPasswordStep1(FString LoginId)
{
}

void UJusticeIdentityFunctions::ForgotPasswordStep2(FString UserId, FString VerificationCode, FString NewPassword)
{
}

void UJusticeIdentityFunctions::UserLoginWithSteam(FUserLoginCompleteDynamicDelegate OnComplete)
{
}
