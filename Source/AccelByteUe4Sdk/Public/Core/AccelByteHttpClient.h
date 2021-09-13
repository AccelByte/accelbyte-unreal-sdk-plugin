// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "JsonUtilities.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"

using namespace AccelByte;

namespace AccelByte
{

	class Credentials;
	class Settings;

	/**
	 * @brief HTTP client for building API
	 */
	class ACCELBYTEUE4SDK_API FHttpClient final
	{
	public:
		FHttpClient(Credentials const& CredentialsRef, Settings const& SettingsRef, FHttpRetryScheduler& HttpRef);
		~FHttpClient();

		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Content HTTP request content string
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP response is successful.
		 * @param OnError Callback when HTTP response is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb, const FString& Url, const TMap<FString, FString>& Params,
			const FString& Content, const TMap<FString, FString>& Headers,
			const U& OnSuccess, const V& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET"))) // https://stackoverflow.com/a/14710450
			{
				Request->SetContentAsString(Content);
			}

			return ProcessRequest(Request, Verb, Url, Params, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Content HTTP request content binary (implies Content-Type application/octet-stream header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP response is successful.
		 * @param OnError Callback when HTTP response is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb, const FString& Url, const TMap<FString, FString>& Params,
			const TArray<uint8>& Content, const TMap<FString, FString>& Headers,
			const U& OnSuccess, const V& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET"))) // https://stackoverflow.com/a/14710450
			{
				Request->SetHeader(TEXT("Content-Type"), TEXT("application/octet-stream"));
				Request->SetContent(Content);
			}

			return ProcessRequest(Request, Verb, Url, Params, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Content HTTP request content form key-value (implies Content-Type application/x-www-form-urlencoded header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP response is successful.
		 * @param OnError Callback when HTTP response is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb, const FString& Url, const TMap<FString, FString>& Params,
			const TMap<FString, FString>& Content, const TMap<FString, FString>& Headers,
			const U& OnSuccess, const V& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET"))) // https://stackoverflow.com/a/14710450
			{
				if (Content.Num() > 0)
				{
					const FString EncodedData = EncodeParamsData(Content);

					Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
					Request->SetContentAsString(EncodedData);
				}
			}

			return ProcessRequest(Request, Verb, Url, Params, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Data HTTP request content form key-value (implies Content-Type application/x-www-form-urlencoded header).
		 * @param OnSuccess Callback when HTTP response is successful.
		 * @param OnError Callback when HTTP response is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb, const FString& Url, const TMap<FString, FString>& Params,
			const TMap<FString, FString>& Data, const U& OnSuccess, const V& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);
			TMap<FString, FString> Headers = {
				{TEXT("Accept"), TEXT("application/json")}
			};

			AddApiAuthorizationIfAvailable(Headers);

			return Request(Verb, ApiUrl, Params, Data, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Json HTTP request content JSON string (implies Content-Type application/json header).
		 * @param OnSuccess Callback when HTTP response is successful.
		 * @param OnError Callback when HTTP response is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb, const FString& Url, const TMap<FString, FString>& Params,
			const FString& Json, const U& OnSuccess, const V& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);
			TMap<FString, FString> Headers = {
				{TEXT("Content-Type"), TEXT("application/json")},
				{TEXT("Accept"), TEXT("application/json")}
			};

			AddApiAuthorizationIfAvailable(Headers);

			return Request(Verb, ApiUrl, Params, Json, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param UStruct HTTP request content from a given UStruct (implies Content-Type application/json header).
		 * @param OnSuccess Callback when HTTP response is successful.
		 * @param OnError Callback when HTTP response is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename T, typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb, const FString& Url, const TMap<FString, FString>& Params,
			const T& UStruct, const U& OnSuccess, const V& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);
			TMap<FString, FString> Headers = {
				{TEXT("Content-Type"), TEXT("application/json")},
				{TEXT("Accept"), TEXT("application/json")}
			};

			AddApiAuthorizationIfAvailable(Headers);

			FString Json;

			if (!FJsonObjectConverter::UStructToJsonObjectString(UStruct, Json))
			{
				UE_LOG(LogTemp, Error, TEXT("HttpClient Request UStructToJsonObjectString failed!"));
			}

			return Request(Verb, ApiUrl, Params, Json, Headers, OnSuccess, OnError);
		}

	private:
		FHttpRetryScheduler& HttpRef;
		Credentials const& CredentialsRef;
		Settings const& SettingsRef;

		FString FormatApiUrl(const FString& Url) const;

		void AddApiAuthorizationIfAvailable(TMap<FString, FString>& Headers) const;

	    FString EncodeParamsData(const TMap<FString, FString>& ParamsData) const;

		template<typename U, typename V>
		FAccelByteTaskPtr ProcessRequest(FHttpRequestPtr& Request, const FString& Verb, const FString& Url,
			const TMap<FString, FString>& Params, const TMap<FString, FString>& Headers,
			const U& OnSuccess, const V& OnError)
		{
			FString QueryString;

			if (Params.Num() > 0)
			{
				QueryString.Append("?").Append(EncodeParamsData(Params));
			}

			Request->SetURL(FString::Printf(TEXT("%s%s"), *Url, *QueryString));
			Request->SetVerb(Verb.ToUpper());

			for (const auto& Kvp : Headers)
			{
				Request->SetHeader(Kvp.Key, Kvp.Value); // Override existing headers
			}

			return HttpRef.ProcessRequest(
				Request,
				CreateHttpResultHandler(
					OnSuccess,
					OnError
				),
				FPlatformTime::Seconds());
		}
	};

}
