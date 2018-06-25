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

TArray<ULinkedPlatform*> UJusticeIdentityFunctions::GetUnlinkedPlatforms(TArray<ULinkedPlatform*> LinkedPlatforms)
{
	TArray<ULinkedPlatform*> UUnlinkedPlatforms = UJusticeIdentityFunctions::GetCompleteListOfPlatforms();
	for (int32 i = 0; i < LinkedPlatforms.Num(); i++)
	{
		for (int32 j = 0; j < UUnlinkedPlatforms.Num(); j++)
		{
			FString LinkedPlatformID = LinkedPlatforms[i]->PlatformID;
			FString CompletePlatformID = UUnlinkedPlatforms[j]->PlatformID;
			if (LinkedPlatformID == CompletePlatformID)
			{
				UUnlinkedPlatforms.RemoveAt(j);
			}
		}
	}
	return UUnlinkedPlatforms;
}

TArray<ULinkedPlatform*> UJusticeIdentityFunctions::GetCompleteListOfPlatforms()
{
	LinkedPlatform DevicePlatform;
	LinkedPlatform GooglePlatform;
	LinkedPlatform FacebookPlatform;
	LinkedPlatform TwitchPlatform;
	LinkedPlatform OculusPlatform;
	DevicePlatform.PlatformID = TEXT("device");
	GooglePlatform.PlatformID = TEXT("google");
	FacebookPlatform.PlatformID = TEXT("facebook");
	TwitchPlatform.PlatformID = TEXT("twitch");
	OculusPlatform.PlatformID = TEXT("twitter");

	TArray<LinkedPlatform> CompletePlatforms;
	CompletePlatforms.Add(DevicePlatform);
	CompletePlatforms.Add(GooglePlatform);
	CompletePlatforms.Add(FacebookPlatform);
	CompletePlatforms.Add(TwitchPlatform);
	CompletePlatforms.Add(OculusPlatform);

	TArray<ULinkedPlatform*> UCompletePlatforms;
	for (int32 i = 0; i < CompletePlatforms.Num(); i++)
	{
		ULinkedPlatform* platform = ULinkedPlatform::Deserialize(CompletePlatforms[i]);
		UCompletePlatforms.Add(platform);
	}

	return UCompletePlatforms;
}

FString UJusticeIdentityFunctions::GetPlatformPageURL(FString PlatformID)
{
	FString PlatformPage = TEXT("");

	if (PlatformID == TEXT("google"))
	{
		PlatformPage = *FGooglePlatformURL;
	} 

	return PlatformPage;
}

FString UJusticeIdentityFunctions::GetPlatformRedirectURL(FString PlatformID)
{
	FString RedirectURL = TEXT("");

	if (PlatformID == TEXT("google"))
	{
		RedirectURL = *FGoogleRedirectURL;
	} 

	return RedirectURL;
}