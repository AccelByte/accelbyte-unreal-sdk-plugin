// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "JsonUtilities.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "AccelByteUtilities.h"

#define REGEX_BASE_URL_WITH_DOMAIN "https?:\\/\\/(?:www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{1,128}\\.[a-zA-Z0-9()]{1,6}"
#define REGEX_BASE_URL_WITHOUT_DOMAIN "(?:(https?:\\/\\/)?((?:[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)|localhost))"
#define REGEX_OPTIONAL_PORT "(:[0-9]{4})?"
#define REGEX_PATH_AND_PARAM "(\\/?|((\\/)[^\\/\\\\]{1,512}(?:\\7[^\\/\\\\]{1,512})*(\\/?)))"
#define REGEX_URL "^(" REGEX_BASE_URL_WITH_DOMAIN "|" REGEX_BASE_URL_WITHOUT_DOMAIN ")" REGEX_OPTIONAL_PORT REGEX_PATH_AND_PARAM "$"

using namespace AccelByte;

namespace AccelByte
{

	class BaseCredentials;
	class BaseSettings;

	/**
	 * @brief HTTP client for building API
	 */
	class ACCELBYTEUE4SDK_API FHttpClient final
	{
	public:
		FHttpClient(BaseCredentials const& InCredentialsRef, BaseSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
		~FHttpClient();

		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const U& OnSuccess
			, const V& OnError)
		{
			return Request(Verb, Url, {}, FString(), {}, OnSuccess, OnError);
		}
		
		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const U& OnSuccess
			, const V& OnError)
		{
			return Request(Verb, Url, Params, FString(), {}, OnSuccess, OnError);
		}
		
		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{
			return Request(Verb, Url, Params, FString(), Headers, OnSuccess, OnError);
		}
		
		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Content HTTP request content string
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url 
			, const FString& Content
			, const U& OnSuccess
			, const V& OnError)
		{
			return Request(Verb, Url, {}, Content, {}, OnSuccess, OnError);
		}

		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Content HTTP request content binary (implies Content-Type application/octet-stream header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TArray<uint8>& Content
			, const U& OnSuccess
			, const V& OnError)
		{
			return Request(Verb, Url, {}, Content, {}, OnSuccess, OnError);
		}
		
		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Content HTTP request content string
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url 
			, const FString& Content
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{
			return Request(Verb, Url, {}, Content, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Content HTTP request content binary (implies Content-Type application/octet-stream header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TArray<uint8>& Content
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{
			return Request(Verb, Url, {}, Content, Headers, OnSuccess, OnError);
		}
		
		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Content HTTP request content string
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const FString& Content
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET")) && !Content.IsEmpty()) // https://stackoverflow.com/a/14710450
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
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const TArray<uint8>& Content
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET")) && Content.Num() > 0) // https://stackoverflow.com/a/14710450
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
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const TMap<FString, FString>& Content
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
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

		template<typename  U, typename V, typename W>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const TArray<uint8>& Content
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnProgress
			, const W& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET")) && Content.Num() > 0) // https://stackoverflow.com/a/14710450
			{
				if (Request->GetHeader(TEXT("Content-Type")).IsEmpty())
				{
					Request->SetHeader(TEXT("Content-Type"), TEXT("application/octet-stream"));
				}
				Request->SetContent(Content);
			}

			Request->OnRequestProgress() = OnProgress;

			return ProcessRequest(Request, Verb, Url, Params, Headers, OnSuccess, OnError);
		}

		template<typename  U, typename V, typename W>
		FAccelByteTaskPtr Request(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const FString& Content
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnProgress
			, const W& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET")) && !Content.IsEmpty()) // https://stackoverflow.com/a/14710450
			{
				Request->SetContentAsString(Content);
			}

			Request->OnRequestProgress() = OnProgress;

			return ProcessRequest(Request, Verb, Url, Params, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const U& OnSuccess
			, const V& OnError)
		{
			return ApiRequest(Verb, Url, {}, TMap<FString, FString>(), OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const U& OnSuccess
			, const V& OnError)
		{
			return ApiRequest(Verb, Url, Params, TMap<FString, FString>(), OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Data HTTP request content form key-value (implies Content-Type application/x-www-form-urlencoded header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const TMap<FString, FString>& Data
			, const U& OnSuccess
			, const V& OnError)
		{
			TMap<FString, FString> Headers = {
				{TEXT("Accept"), TEXT("application/json")}
			};

			return ApiRequest(Verb, Url, Params, Data, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Json HTTP request content JSON string (implies Content-Type application/json header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const FString& Json
			, const U& OnSuccess
			, const V& OnError)
		{
			TMap<FString, FString> Headers = {
				{TEXT("Content-Type"), TEXT("application/json")},
				{TEXT("Accept"), TEXT("application/json")}
			};

			return ApiRequest(Verb, Url, Params, Json, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param UStruct HTTP request content from a given UStruct (implies Content-Type application/json header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 * @param bOmitBlankValues Flag that will remove blank string values from the JSON string sent to the server.
		 * Defaults to false.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename T, typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const T& UStruct
			, const U& OnSuccess
			, const V& OnError
			, bool bOmitBlankValues = false)
		{
			TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(UStruct);
			FString JsonString = ParseUStructToJsonString(JsonObject, bOmitBlankValues);
			
			return ApiRequest(Verb, Url, Params, JsonString, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param UStruct HTTP request content from a given UStruct (implies Content-Type application/json header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 * @param bOmitBlankValues Flag that will remove blank string values from the JSON string sent to the server.
		 * Defaults to false.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename T, typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const T& UStruct
			, TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError
			, bool bOmitBlankValues = false)
		{
			TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(UStruct);
			FString JsonString = ParseUStructToJsonString(JsonObject, bOmitBlankValues);

			return ApiRequest(Verb, Url, Params, JsonString, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Content HTTP request content byte array (implies Content-Type application/octet-stream header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const TArray<uint8>& Content
			, TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);

			if(!AddApiAuthorizationIfAvailable(Headers))
			{
				ExecuteError(OnError, TEXT("Authorization Bearer is empty"));
				return nullptr;
			}

			return Request(Verb, ApiUrl, Params, Content, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Data HTTP request content form key-value (implies Content-Type application/x-www-form-urlencoded header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const TMap<FString, FString>& Data
			, TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);

			if(!AddApiAuthorizationIfAvailable(Headers))
			{
				ExecuteError(OnError, TEXT("Authorization Bearer is empty"));
				return nullptr;
			}

			return Request(Verb, ApiUrl, Params, Data, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param Params HTTP request query string key-value.
		 * @param Json HTTP request content JSON string (implies Content-Type application/json header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const FString& Json
			, TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);

			if(!AddApiAuthorizationIfAvailable(Headers))
			{
				ExecuteError(OnError, TEXT("Authorization Bearer is empty"));
				return nullptr;
			}

			return Request(Verb, ApiUrl, Params, Json, Headers, OnSuccess, OnError);
		}

	private:
		FHttpRetryScheduler& HttpRef;
		BaseCredentials const& CredentialsRef;
		BaseSettings const& SettingsRef;

		FString FormatApiUrl(const FString& Url) const;

		bool AddApiAuthorizationIfAvailable(TMap<FString, FString>& Headers) const;

		bool IsValidUrl(const FString& Url);

		void ExecuteError(const FErrorHandler& OnError, const FString& ErrorText);

		void ExecuteError(const FCustomErrorHandler& OnError, const FString& ErrorText);

		FString EncodeParamsData(const TMap<FString
			, FString>& ParamsData) const;

		FString ParseUStructToJsonString(TSharedPtr<FJsonObject> JsonObject, bool bOmitBlankValues = false)
		{
			FString JsonString;
			
			if (!JsonObject.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("HttpClient Request UStructToJsonObject failed!"));

				return JsonString;
			}

			// Omit blank JSON string values if opted into by the caller
			if (bOmitBlankValues)
			{
				FAccelByteUtilities::RemoveEmptyStrings(JsonObject);
			}

			// Finally, write the JSON object to a string
			TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&JsonString);
			if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
			{
				UE_LOG(LogTemp, Error, TEXT("HttpClient Request FJsonSerializer::Serialize failed!"));

				return JsonString;
			}

			return JsonString;
		}

		template<typename U, typename V>
		FAccelByteTaskPtr ProcessRequest(FHttpRequestPtr& Request
			, const FString& Verb
			, const FString& Url
			, const TMap<FString, FString>& Params
			, const TMap<FString, FString>& Headers
			, const U& OnSuccess
			, const V& OnError)
		{

			if (!IsValidUrl(Url))
			{
				FString ErrorText = FString::Format(TEXT("Invalid URL format {0}"), { Url });
				ExecuteError(OnError, ErrorText);
				return nullptr;
			}

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

			return HttpRef.ProcessRequest(Request
				, CreateHttpResultHandler(
					OnSuccess,
					OnError,
					&HttpRef
				)
				, FPlatformTime::Seconds());
		}
	};

}
