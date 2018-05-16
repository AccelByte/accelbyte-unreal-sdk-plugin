// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentityFunctions.h"

void UJusticeIdentityFunctions::UserLogin(FString LoginId, FString Password, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::UserLogin(LoginId, Password, FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* Token) {
			OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, Token);
	}));
}

void UJusticeIdentityFunctions::WebLoginRefresh(FString RefreshToken, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::SetRefreshToken(RefreshToken);
	JusticeIdentity::RefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* Token) {
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

void UJusticeIdentityFunctions::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, UUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::RegisterNewPlayer(UserId, Password, DisplayName, (FUserAuthTypeJustice)AuthType,
		FRegisterPlayerCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, UUserCreateResponse* Response) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, Response);
	}));

}

void UJusticeIdentityFunctions::VerifyNewPlayer(FString UserId, FString VerificationCode, UUserAuthTypeJustice AuthType, FVerifyNewPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::VerifyNewPlayer(UserId, VerificationCode, (FUserAuthTypeJustice)AuthType,
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
	UOAuthTokenJustice* NewOAuthTokenJustice = NewObject<UOAuthTokenJustice>();
	NewOAuthTokenJustice->FromParent(JusticeIdentity::GetUserToken());
	return NewOAuthTokenJustice; 
}

UOAuthTokenJustice * UJusticeIdentityFunctions::GetClientToken()
{
	UOAuthTokenJustice* NewOAuthTokenJustice = NewObject<UOAuthTokenJustice>();
	NewOAuthTokenJustice->FromParent(JusticeIdentity::GetClientToken());
	return NewOAuthTokenJustice; 
}

FString UJusticeIdentityFunctions::GetUserId()
{
	return JusticeIdentity::GetUserId();
}

void UJusticeIdentityFunctions::LinkSteam(FUserLoginCompleteDynamicDelegate OnComplete)
{
}
