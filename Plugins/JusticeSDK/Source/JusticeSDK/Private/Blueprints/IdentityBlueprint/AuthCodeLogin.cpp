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
	Node->RedirectURI = FJusticeSDKModule::Get().RedirectURI;
	return Node;
}

void UAsyncAuthCodeLogin::Activate()
{
	if (this->AuthCode.Len() > 0)
	{
		JusticeIdentity::AuthCodeLogin(this->AuthCode, this->RedirectURI, FUserLoginCompleteDelegate2::CreateLambda([&](bool bSuccessful, FString ErrorStr, TSharedPtr<FOAuthTokenJustice> Token) {
			if (bSuccessful)
			{
				if (OnSuccess.IsBound())
				{
					OnSuccess.Broadcast(TEXT(""));
				}
			}
			else
			{
				if (OnFailed.IsBound())
				{
					OnFailed.Broadcast(ErrorStr);
				}
			}
		}));
	}
	else
	{
		OnFailed.Broadcast(TEXT("No auth code was provided"));
	}

}