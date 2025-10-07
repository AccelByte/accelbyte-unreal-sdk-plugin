// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpClient.h"
#include "Core/AccelByteBaseCredentials.h"

namespace AccelByte
{
	FHttpFormData::FHttpFormData()
	{
	}

	FHttpFormData::FHttpFormData(FHttpFormData const& Other)
		: Data{ Other.Data }
	{
	}

	FHttpFormData::FHttpFormData(TMap<FString, FString> const& InFormData)
	{
		ConvertMapToMultiMap(InFormData, Data);
	}

	FHttpFormData::FHttpFormData(TMultiMap<FString, FString> const& InFormData)
		: Data{ InFormData }
	{
	}

	FHttpFormData::~FHttpFormData()
	{
		Data.Reset();
	}

	bool FHttpFormData::IsEmpty() const
	{
		return Data.Num() == 0;
	}

	FString FHttpFormData::ToString() const
	{
		return Serialize(Data);
	}

	FString FHttpFormData::ToUrlEncodedString() const
	{
		return Serialize(Data, true);
	}

	bool FHttpFormData::ConvertMapToMultiMap(TMap<FString, FString> const& In, TMultiMap<FString, FString>& Out)
	{
		bool bIsSuccess = false;
		
		for (const auto& Kvp : In)
		{
			if (Kvp.Key.IsEmpty() || Kvp.Value.IsEmpty())
			{
				continue;
			}
			
			Out.Add(Kvp.Key, Kvp.Value);
			bIsSuccess = true;
		}

		return bIsSuccess;
	}

	FString FHttpFormData::CheckThenUrlEncode(FString const& InValue)
	{
		TArray<uint8> KeyBytes;
		int32 const Length = InValue.Len();
		KeyBytes.AddUninitialized(Length);
		StringToBytes(InValue, KeyBytes.GetData(), Length);
		return FGenericPlatformHttp::IsURLEncoded(KeyBytes) ? InValue : FGenericPlatformHttp::UrlEncode(InValue);
	}

	FString FHttpFormData::Serialize(TMultiMap<FString, FString> const& InFormData, bool bShouldUrlEncode)
	{
		FString Result;
		int ParamCount = 0;

		for (auto const& Kvp : InFormData)
		{
			if (Kvp.Key.IsEmpty() || Kvp.Value.IsEmpty())
			{
				continue;
			}

			FString EncodedKey = bShouldUrlEncode ? CheckThenUrlEncode(Kvp.Key) : Kvp.Key;
			FString EncodedValue = bShouldUrlEncode ? CheckThenUrlEncode(Kvp.Value) : Kvp.Value;
			
			Result.Append(FString::Printf(TEXT("%s%s=%s"), ParamCount++ == 0 ? TEXT("") : TEXT("&")
										  , *EncodedKey, *EncodedValue));
		}

		return Result;
	}
	
	FHttpClient::FHttpClient(BaseCredentials const& InCredentialsRef
		, BaseSettings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef)
		: HttpRef{InHttpRef}
		, CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
	{
	}

	FHttpClient::~FHttpClient()
	{
	}

	FString FHttpClient::FormatApiUrl(const FString& Url) const
	{
		FStringFormatNamedArguments UrlArgs;
		if (CredentialsRef.GetSessionState() != BaseCredentials::ESessionState::Invalid)
		{
			UrlArgs = {
				{TEXT("namespace"), FStringFormatArg(CredentialsRef.GetNamespace())},
				{TEXT("userId"), FStringFormatArg(CredentialsRef.GetUserId())},
			};
		}
		else
		{
			UrlArgs = {
				{TEXT("namespace"), FStringFormatArg(SettingsRef.Namespace)}
			};
		}

		return FString::Format(*Url, UrlArgs);
	}

	bool FHttpClient::AddApiAuthorizationIfAvailable(TMap<FString, FString>& Headers) const
	{
		FString const AccessToken = CredentialsRef.GetAccessToken();

		if (!AccessToken.IsEmpty())
		{
			Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *AccessToken));
			return true;
		}
		return false;
	}

	void FHttpClient::AddFlightIdHeader(TMap<FString, FString>& Headers) const
	{
		Headers.Add(TEXT("x-flight-id"), FAccelByteUtilities::GetFlightId());
	}

	bool FHttpClient::IsValidUrl(FString const& Url)
	{
		return FAccelByteNetUtilities::IsValidUrl(Url);
	}

	void FHttpClient::ExecuteError(FErrorHandler const& OnError, FString const& ErrorText)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), ErrorText);
	}

	void FHttpClient::ExecuteError(const FCustomErrorHandler& OnError, const FString& ErrorText)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), ErrorText, FJsonObject{});
	}

	void FHttpClient::ExecuteError(const FCreateMatchmakingTicketErrorHandler& OnError, const FString& ErrorText)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), ErrorText, FErrorCreateMatchmakingTicketV2());
	}
}
