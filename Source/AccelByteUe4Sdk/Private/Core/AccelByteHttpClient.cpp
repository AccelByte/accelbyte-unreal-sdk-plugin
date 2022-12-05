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
			if (Kvp.Key.IsEmpty() || Kvp.Value.IsEmpty())
			{
				continue;
			}

			TArray<uint8> KeyBytes;
			KeyBytes.AddUninitialized(Kvp.Key.Len());
			StringToBytes(Kvp.Key, KeyBytes.GetData(), Kvp.Key.Len());
			FString EncodedKey = FGenericPlatformHttp::IsURLEncoded(KeyBytes) ? Kvp.Key : FGenericPlatformHttp::UrlEncode(Kvp.Key);

			TArray<uint8> ValueBytes;
			ValueBytes.AddUninitialized(Kvp.Value.Len());
			StringToBytes(Kvp.Value, ValueBytes.GetData(), Kvp.Value.Len());
			FString EncodedValue = FGenericPlatformHttp::IsURLEncoded(ValueBytes) ? Kvp.Value : FGenericPlatformHttp::UrlEncode(Kvp.Value);
			
			Result.Append(FString::Printf(TEXT("%s%s=%s"), i++ == 0 ? TEXT("") : TEXT("&")
			                              , *EncodedKey, *EncodedValue));
		}

		return Result;
	}

	FHttpClient::~FHttpClient()
	{
	}
}
