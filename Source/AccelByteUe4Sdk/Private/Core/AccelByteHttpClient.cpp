// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpClient.h"

namespace AccelByte
{

FHttpClient::FHttpClient(Credentials const& CredentialsRef, Settings const& SettingsRef, FHttpRetryScheduler& HttpRef) :
	HttpRef{ HttpRef },
	CredentialsRef{ CredentialsRef },
	SettingsRef{ SettingsRef }
{}

void FHttpClient::AddApiAccessTokenIfAvailable(TMap<FString, FString>& Headers)
{
	FString AccessToken = CredentialsRef.GetAccessToken();

	if (!AccessToken.IsEmpty())
	{
		Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *AccessToken));
	}
}

FString FHttpClient::EncodeParamsData(const TMap<FString, FString>& ParamsData)
{
	FString Result;
	int i = 0;

	for (const auto& Kvp : ParamsData)
	{
		Result.Append(FString::Printf(TEXT("%s%s=%s"),
			(i++ == 0 ? TEXT("") : TEXT("&")),
			*FGenericPlatformHttp::UrlEncode(Kvp.Key),
			*FGenericPlatformHttp::UrlEncode(Kvp.Value)));
	}

	return Result;
}

FHttpClient::~FHttpClient()
{}

}
