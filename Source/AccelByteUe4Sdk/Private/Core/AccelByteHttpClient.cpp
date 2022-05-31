// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpClient.h"

namespace AccelByte
{
	FHttpClient::FHttpClient(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef)
		: HttpRef{InHttpRef}
		, CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
	{
	}

	FString FHttpClient::FormatApiUrl(const FString& Url) const
	{
		const FStringFormatNamedArguments UrlArgs = {
			{TEXT("namespace"), FStringFormatArg(CredentialsRef.GetNamespace())},
			{TEXT("userId"), FStringFormatArg(CredentialsRef.GetUserId())},
		};

		return FString::Format(*Url, UrlArgs);
	}

	void FHttpClient::AddApiAuthorizationIfAvailable(TMap<FString, FString>& Headers) const
	{
		const FString AccessToken = CredentialsRef.GetAccessToken();

		if (!AccessToken.IsEmpty())
		{
			Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *AccessToken));
		}
	}

	FString FHttpClient::EncodeParamsData(const TMap<FString, FString>& ParamsData) const
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
	{
	}
}
