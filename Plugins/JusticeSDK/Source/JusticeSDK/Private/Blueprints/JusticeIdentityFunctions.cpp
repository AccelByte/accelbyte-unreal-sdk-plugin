// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentityFunctions.h"

void UJusticeIdentityFunctions::UserLogin(FString LoginId, FString Password, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::UserLogin(LoginId, Password, FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, OAuthTokenJustice* Token) {
		UOAuthTokenJustice* UToken = UOAuthTokenJustice::Deserialize(Token);
		check(UToken);
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, UToken);
	}));
}

void UJusticeIdentityFunctions::WebLoginRefresh(FString UserRefreshToken, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::SetRefreshToken(UserRefreshToken);
	JusticeIdentity::UserRefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, OAuthTokenJustice* Token) {
		UOAuthTokenJustice* UToken = UOAuthTokenJustice::Deserialize(Token);
		check(UToken);
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, UToken);
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
		FRegisterPlayerCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr, UserCreateResponse* Response) {
		UUserCreateResponse* UResponse = UUserCreateResponse::Deserialize(Response);
		OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, UResponse);
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
	UOAuthTokenJustice* NewOAuthTokenJustice = UOAuthTokenJustice::Deserialize(FJusticeUserToken);
	check(NewOAuthTokenJustice);
	return NewOAuthTokenJustice; 
}

UOAuthTokenJustice * UJusticeIdentityFunctions::GetClientToken()
{
	UOAuthTokenJustice* NewOAuthTokenJustice = UOAuthTokenJustice::Deserialize(FJusticeGameClientToken);
	check(NewOAuthTokenJustice);
	return NewOAuthTokenJustice; 
}

FString UJusticeIdentityFunctions::GetUserId()
{
	return FJusticeUserID;
}

void UJusticeIdentityFunctions::LinkSteam(FUserLoginCompleteDynamicDelegate OnComplete)
{
}
