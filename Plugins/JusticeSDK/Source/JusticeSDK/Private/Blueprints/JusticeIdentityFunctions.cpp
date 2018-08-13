// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentityFunctions.h"
#include "WebBrowserModule.h"
#include "IWebBrowserCookieManager.h"
#include "IWebBrowserSingleton.h"

void UJusticeIdentityFunctions::UserLogin(FString LoginID, FString Password, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::UserLogin(LoginID, Password, FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr, FOAuthTokenJustice* Token) {
		UOAuthTokenJustice* UToken = UOAuthTokenJustice::Deserialize(Token);
		check(UToken);
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr, UToken);
	}));
}

void UJusticeIdentityFunctions::WebLoginRefresh(FString UserRefreshToken, FUserLoginCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::SetRefreshToken(UserRefreshToken);
	JusticeIdentity::UserRefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr, FOAuthTokenJustice* Token) {
		UOAuthTokenJustice* UToken = UOAuthTokenJustice::Deserialize(Token);
		check(UToken);
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr, UToken);
	}));
}

void UJusticeIdentityFunctions::UserLogout(FDefaultCompleteDynamicDelegate OnComplete)
{
	FDefaultCompleteDelegate LogoutCompleteDelegate;
	LogoutCompleteDelegate.BindLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr);
	});
	JusticeIdentity::UserLogout(LogoutCompleteDelegate);
}

void UJusticeIdentityFunctions::RegisterNewPlayer(FString UserID, FString Password, FString DisplayName, UUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::RegisterNewPlayer(UserID, Password, DisplayName, (FUserAuthTypeJustice)AuthType,
		FRegisterPlayerCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr, FUserCreateResponse* Response) {
		UUserCreateResponse* UResponse = UUserCreateResponse::Deserialize(Response);
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr, UResponse);
	}));

}

void UJusticeIdentityFunctions::VerifyNewPlayer(FString UserID, FString VerificationCode, UUserAuthTypeJustice AuthType, FDefaultCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::VerifyNewPlayer(UserID, VerificationCode, (FUserAuthTypeJustice)AuthType,
		FDefaultCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr);
	}));
}

void UJusticeIdentityFunctions::ForgotPassword(FString LoginID, FDefaultCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::ForgotPassword(LoginID,
		FDefaultCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
		OnComplete.ExecuteIfBound(bSuccessful, ErrorStr);
	}));
}

void UJusticeIdentityFunctions::ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDynamicDelegate OnComplete)
{
	JusticeIdentity::ResetPassword(UserID, VerificationCode, NewPassword,
		FDefaultCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
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


void UJusticeIdentityFunctions::ClearCacheAndLocalStorage()
{
	IWebBrowserSingleton* WebModule = IWebBrowserModule::Get().GetSingleton();
	TSharedPtr<IWebBrowserCookieManager> cookieMgr = WebModule->GetCookieManager();	
	cookieMgr->DeleteCookies();
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
	FLinkedPlatform DevicePlatform;
	FLinkedPlatform GooglePlatform;
	FLinkedPlatform FacebookPlatform;
	FLinkedPlatform TwitchPlatform;
	FLinkedPlatform OculusPlatform;
	DevicePlatform.PlatformID = TEXT("device");
	GooglePlatform.PlatformID = TEXT("google");
	FacebookPlatform.PlatformID = TEXT("facebook");
	TwitchPlatform.PlatformID = TEXT("twitch");
	OculusPlatform.PlatformID = TEXT("twitter");

	TArray<FLinkedPlatform> CompletePlatforms;
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
	else if (PlatformID == TEXT("facebook"))
	{
		PlatformPage = *FFacebookPlatformURL;
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
	else if (PlatformID == TEXT("facebook"))
	{
		RedirectURL = *FFacebookRedirectURL;
	}

	return RedirectURL;
}