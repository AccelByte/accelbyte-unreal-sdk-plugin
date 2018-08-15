// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/AuthCodeLogin.h"
#include "Services/JusticeIdentity.h"

UAsyncAuthCodeLogin * UAsyncAuthCodeLogin::AuthCodeLogin()
{
	TCHAR* AuthCode = new TCHAR[512];
#ifdef _WIN32
	FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), AuthCode, 512);
#elif __linux__ || __clang__
	FLinuxPlatformMisc::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), AuthCode, 512);
#endif

	UAsyncAuthCodeLogin* Node = NewObject<UAsyncAuthCodeLogin>();
	Node->AuthCode = FString(AuthCode);
	Node->RedirectURI = TEXT("https://accelbyte.net/");
	return Node;
}

void UAsyncAuthCodeLogin::Activate()
{
	if (this->AuthCode.Len() > 0)
	{
		JusticeIdentity::AuthCodeLogin(this->AuthCode, this->RedirectURI, FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (OnSuccess.IsBound())
				{
					OnSuccess.Broadcast(UOAuthTokenJustice::Deserialize(Token), TEXT(""));
				}
			}
			else
			{
				if (OnFailed.IsBound())
				{
					OnFailed.Broadcast(nullptr, ErrorStr);
				}
			}
		}));
	}
	else
	{
		OnFailed.Broadcast(nullptr, TEXT("No auth code was provided"));
	}

}