// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentityFunctions.h"

void UJusticeIdentityFunctions::UserLogin(FString LoginId, FString Password, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::Login(LoginId, Password, FGrantTypeJustice::PasswordGrant, FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* Token) {
			OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, Token);
	}));
}


void UJusticeIdentityFunctions::WebLoginRefresh(FString RefreshToken, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::SetRefreshToken(RefreshToken);
	JusticeIdentity::Login(TEXT(""), TEXT(""), FGrantTypeJustice::RefreshGrant, FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* Token) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, Token);
	}));



}

void UJusticeIdentityFunctions::UserLogout(FUserLogoutCompleteDynamicDelegate OnComplete)
{
	FUserLogoutCompleteDelegate LogoutCompleteDelegate;
	LogoutCompleteDelegate.BindLambda([OnComplete](bool IsSuccess, FString ErrorStr) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr);
	});
	JusticeIdentity::UserLogout(LogoutCompleteDelegate);
}

void UJusticeIdentityFunctions::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::RegisterNewPlayer(UserId, Password, DisplayName, AuthType, 
		FRegisterPlayerCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, UUserCreateResponse* Response) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, Response);
	}));

}

void UJusticeIdentityFunctions::VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::VerifyNewPlayer(UserId, VerificationCode,
		FVerifyNewPlayerCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr);
	}));

}

void UJusticeIdentityFunctions::ForgotPassword(FString LoginId, FForgotPasswordCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::ForgotPassword(LoginId,
		FForgotPasswordCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr);
	}));
}

void UJusticeIdentityFunctions::ResetPassword(FString UserId, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::ResetPassword(UserId, VerificationCode, NewPassword,
		FResetPasswordCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr);
	}));
}

UOAuthTokenJustice * UJusticeIdentityFunctions::GetUserToken()
{
	//return JusticeIdentity::GetUserToken();
	return NewObject<UOAuthTokenJustice>(); // TODO
}

UOAuthTokenJustice * UJusticeIdentityFunctions::GetClientToken()
{
	//return JusticeIdentity::GetClientToken();
	return NewObject<UOAuthTokenJustice>(); // TODO
}

FString UJusticeIdentityFunctions::GetUserId()
{
	return JusticeIdentity::GetUserId();
}

void UJusticeIdentityFunctions::LinkSteam(FUserLoginCompleteDynamicDelegate OnComplete)
{
}
