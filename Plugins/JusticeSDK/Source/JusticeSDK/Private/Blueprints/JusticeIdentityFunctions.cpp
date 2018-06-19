// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentityFunctions.h"

void UJusticeIdentityFunctions::UserLogin(FString LoginID, FString Password, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::UserLogin(LoginID, Password, FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* Token) {
		UOAuthTokenJustice* UToken = UOAuthTokenJustice::Deserialize(Token);
		check(UToken);
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr, UToken);
	}));
}

void UJusticeIdentityFunctions::WebLoginRefresh(FString UserRefreshToken, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::SetRefreshToken(UserRefreshToken);
	JusticeIdentity::UserRefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* Token) {
		UOAuthTokenJustice* UToken = UOAuthTokenJustice::Deserialize(Token);
		check(UToken);
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr, UToken);
	}));
}

void UJusticeIdentityFunctions::UserLogout(FUserLogoutCompleteDynamicDelegate OnComplete)
{
	FUserLogoutCompleteDelegate LogoutCompleteDelegate;
	LogoutCompleteDelegate.BindLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr);
	});
	JusticeIdentity::UserLogout(LogoutCompleteDelegate);
}

void UJusticeIdentityFunctions::RegisterNewPlayer(FString UserID, FString Password, FString DisplayName, UUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::RegisterNewPlayer(UserID, Password, DisplayName, (FUserAuthTypeJustice)AuthType,
		FRegisterPlayerCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr, UserCreateResponse* Response) {
		UUserCreateResponse* UResponse = UUserCreateResponse::Deserialize(Response);
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr, UResponse);
	}));

}

void UJusticeIdentityFunctions::VerifyNewPlayer(FString UserID, FString VerificationCode, UUserAuthTypeJustice AuthType, FVerifyNewPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::VerifyNewPlayer(UserID, VerificationCode, (FUserAuthTypeJustice)AuthType,
		FVerifyNewPlayerCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr);
	}));
}

void UJusticeIdentityFunctions::ForgotPassword(FString LoginID, FForgotPasswordCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::ForgotPassword(LoginID,
		FForgotPasswordCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr);
	}));
}

void UJusticeIdentityFunctions::ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::ResetPassword(UserID, VerificationCode, NewPassword,
		FResetPasswordCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr);
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
