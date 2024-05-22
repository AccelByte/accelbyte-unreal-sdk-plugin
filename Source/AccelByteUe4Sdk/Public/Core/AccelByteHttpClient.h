// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "JsonUtilities.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteBaseCredentials.h"
#include "Core/AccelByteBaseSettings.h"

namespace AccelByte
{
	class ACCELBYTEUE4SDK_API FHttpFormData final
	{
	public:
		FHttpFormData();
		FHttpFormData(FHttpFormData const& Other);
		FHttpFormData(TMap<FString, FString> const& InFormData);
		FHttpFormData(TMultiMap<FString, FString> const& InFormData);
		~FHttpFormData();

		bool IsEmpty() const;
		FString ToString() const;
		FString ToUrlEncodedString() const;
	private:
		static bool ConvertMapToMultiMap(TMap<FString, FString> const& In, TMultiMap<FString, FString> & Out);
		static FString CheckThenUrlEncode(FString const& InValue);
		static FString Serialize(TMultiMap<FString, FString> const& InFormData, bool bShouldUrlEncode = false);
		
		TMultiMap<FString, FString> Data;
	};
	
	/**
	 * @brief HTTP client for building API
	 */
	class ACCELBYTEUE4SDK_API FHttpClient final
	{
	public:
		FHttpClient(BaseCredentials const& InCredentialsRef
			, BaseSettings const& InSettingsRef
			, FHttpRetryScheduler & InHttpRef);
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
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, U const& OnSuccess
			, V const& OnError)
		{
			return Request(Verb, Url, {}, FString(), {}, OnSuccess, OnError);
		}
		
		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, U const& OnSuccess
			, V const& OnError)
		{			
			return Request(Verb, Url, QueryParams, FString(), {}, OnSuccess, OnError);
		}
		
		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			return Request(Verb, Url, QueryParams, FString(), Headers, OnSuccess, OnError);
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
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url 
			, FString const& Content
			, U const& OnSuccess
			, V const& OnError)
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
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, TArray<uint8> const& Content
			, U const& OnSuccess
			, V const& OnError)
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
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url 
			, FString const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, V const& OnError)
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
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, TArray<uint8> const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			return Request(Verb, Url, {}, Content, Headers, OnSuccess, OnError);
		}
		
		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Content HTTP request content string
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, FString const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET")) && !Content.IsEmpty()) // https://stackoverflow.com/a/14710450
			{
				Request->SetContentAsString(Content);
			}

			return ProcessRequest(Request, Verb, Url, QueryParams, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Content HTTP request content binary (implies Content-Type application/octet-stream header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TArray<uint8> const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET")) && Content.Num() > 0) // https://stackoverflow.com/a/14710450
			{
				Request->SetHeader(TEXT("Content-Type"), TEXT("application/octet-stream"));
				Request->SetContent(Content);
			}

			return ProcessRequest(Request, Verb, Url, QueryParams, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief Basic HTTP request
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Content HTTP request content form key-value (implies Content-Type application/x-www-form-urlencoded header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TMap<FString, FString> const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET"))) // https://stackoverflow.com/a/14710450
			{
				if (Content.Num() > 0)
				{
					const FHttpFormData EncodedData{ Content };

					Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
					Request->SetContentAsString(EncodedData.ToUrlEncodedString());
				}
			}

			return ProcessRequest(Request, Verb, Url, QueryParams, Headers, OnSuccess, OnError);
		}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
		template<typename  U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TArray<uint8> const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, FHttpRequestProgressDelegate64 const& OnProgress
			, V const& OnError)
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

			Request->OnRequestProgress64() = OnProgress;
			return ProcessRequest(Request, Verb, Url, QueryParams, Headers, OnSuccess, OnError);
		}
#endif
		
		template<typename  U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TArray<uint8> const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, FHttpRequestProgressDelegate const& OnProgress
			, V const& OnError)
		{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
			FHttpRequestProgressDelegate64 OnRequestProgress64 = FHttpRequestProgressDelegate64::CreateLambda([OnProgress](FHttpRequestPtr Request, uint64 BytesSent, uint64 BytesReceived)
				{
					OnProgress.ExecuteIfBound(Request, (int32)BytesSent, (int32)BytesReceived);
				});

			return Request(Verb, Url, QueryParams, Content, Headers, OnSuccess, OnRequestProgress64, OnError);
#else
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
			return ProcessRequest(Request, Verb, Url, QueryParams, Headers, OnSuccess, OnError);
#endif
		}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
		template<typename  U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, FString const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, FHttpRequestProgressDelegate64 const& OnProgress
			, V const& OnError)
		{
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET")) && !Content.IsEmpty()) // https://stackoverflow.com/a/14710450
			{
				Request->SetContentAsString(Content);
			}
			Request->OnRequestProgress64() = OnProgress;
			return ProcessRequest(Request, Verb, Url, QueryParams, Headers, OnSuccess, OnError);
		}
#endif

		template<typename  U, typename V>
		FAccelByteTaskPtr Request(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, FString const& Content
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, FHttpRequestProgressDelegate const& OnProgress
			, V const& OnError)
		{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
			FHttpRequestProgressDelegate64 OnRequestProgress64 = FHttpRequestProgressDelegate64::CreateLambda([OnProgress](FHttpRequestPtr Request, uint64 BytesSent, uint64 BytesReceived)
				{
					OnProgress.ExecuteIfBound(Request, (int32)BytesSent, (int32)BytesReceived);
				});

			return Request(Verb, Url, QueryParams, Content, Headers, OnSuccess, OnRequestProgress64, OnError);
#else
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

			if (!Verb.ToUpper().Equals(TEXT("GET")) && !Content.IsEmpty()) // https://stackoverflow.com/a/14710450
			{
				Request->SetContentAsString(Content);
			}
			Request->OnRequestProgress() = OnProgress;
			return ProcessRequest(Request, Verb, Url, QueryParams, Headers, OnSuccess, OnError);
#endif
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
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, U const& OnSuccess
			, V const& OnError)
		{
			return ApiRequest(Verb, Url, {}, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, U const& OnSuccess
			, V const& OnError)
		{
			return ApiRequest(Verb, Url, QueryParams, TMap<FString, FString>(), OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Data HTTP request content form key-value (implies Content-Type application/x-www-form-urlencoded header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TMap<FString, FString> const& Data
			, U const& OnSuccess
			, V const& OnError)
		{
			TMap<FString, FString> Headers = {
				{TEXT("Accept"), TEXT("application/json")}
			};

			return ApiRequest(Verb, Url, QueryParams, Data, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Json HTTP request content JSON string (implies Content-Type application/json header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, FString const& Json
			, U const& OnSuccess
			, V const& OnError)
		{
			TMap<FString, FString> Headers = {
				{TEXT("Content-Type"), TEXT("application/json")},
				{TEXT("Accept"), TEXT("application/json")}
			};

			return ApiRequest(Verb, Url, QueryParams, Json, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Json HTTP request content JSON Object.
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TSharedRef<FJsonObject> const& Json
			, U const& OnSuccess
			, V const& OnError)
		{
			TMap<FString, FString> Headers = {
				{TEXT("Content-Type"), TEXT("application/json")},
				{TEXT("Accept"), TEXT("application/json")}
			};

			FString ContentString{};
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
			if (!FJsonSerializer::Serialize(Json, Writer))
			{
				OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::StatusBadRequest), TEXT("Failed to convert JSON to JSON object string!"));
				return nullptr;
			}

			return ApiRequest(Verb, Url, QueryParams, ContentString, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param UStruct HTTP request content from a given UStruct (implies Content-Type application/json header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 * @param bOmitBlankValues Flag that will remove blank string values from the JSON string sent to the server.
		 * Defaults to false.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename T, typename U, typename V>
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, T const& UStruct
			, U const& OnSuccess
			, V const& OnError
			, bool bOmitBlankValues = false)
		{
			TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(UStruct);
			FString JsonString = ParseUStructToJsonString(JsonObject, bOmitBlankValues);
			
			return ApiRequest(Verb, Url, QueryParams, JsonString, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
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
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, T const& UStruct
			, TMap<FString, FString>& Headers
			, U const& OnSuccess
			, V const& OnError
			, bool bOmitBlankValues = false)
		{
			TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(UStruct);
			FString JsonString = ParseUStructToJsonString(JsonObject, bOmitBlankValues);

			return ApiRequest(Verb, Url, QueryParams, JsonString, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Content HTTP request content byte array (implies Content-Type application/octet-stream header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TArray<uint8> const& Content
			, TMap<FString, FString>& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);

			if(!AddApiAuthorizationIfAvailable(Headers))
			{
				ExecuteError(OnError, TEXT("Authorization Bearer is empty"));
				return nullptr;
			}

			AddFlightIdHeader(Headers);

			return Request(Verb, ApiUrl, QueryParams, Content, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Data HTTP request content form key-value (implies Content-Type application/x-www-form-urlencoded header).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TMap<FString, FString> const& Data
			, TMap<FString, FString>& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);

			if(!AddApiAuthorizationIfAvailable(Headers))
			{
				ExecuteError(OnError, TEXT("Authorization Bearer is empty"));
				return nullptr;
			}
			
			AddFlightIdHeader(Headers);

			return Request(Verb, ApiUrl, QueryParams, Data, Headers, OnSuccess, OnError);
		}

		/**
		 * @brief API request with credentials access token (if available)
		 *
		 * @param Verb HTTP request methods, e.g. GET, POST, PUT, PATCH, DELETE.
		 * @param Url HTTP request URL.
		 * @param QueryParams HTTP request query string key-value.
		 * @param Json HTTP request content JSON string (implies Content-Type application/json header).
		 * @param Headers HTTP request headers key-value (overrides implicit headers).
		 * @param OnSuccess Callback when HTTP call is successful.
		 * @param OnError Callback when HTTP call is error.
		 *
		 * @return FAccelByteTaskPtr.
		 */
		template<typename U, typename V>
		FAccelByteTaskPtr ApiRequest(FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, FString const& Json
			, TMap<FString, FString>& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			FString ApiUrl = FormatApiUrl(Url);

			if (!Headers.Contains(TEXT("Content-Type")))
			{
				Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
			}
			if (!Headers.Contains(TEXT("Accept")))
			{
				Headers.Add(TEXT("Accept"), TEXT("application/json"));
			}

			if(!AddApiAuthorizationIfAvailable(Headers))
			{
				ExecuteError(OnError, TEXT("Authorization Bearer is empty"));
				return nullptr;
			}

			AddFlightIdHeader(Headers);

			return Request(Verb, ApiUrl, QueryParams, Json, Headers, OnSuccess, OnError);
		}

	private:
		FHttpRetryScheduler& HttpRef;
		BaseCredentials const& CredentialsRef;
		BaseSettings const& SettingsRef;

		FString FormatApiUrl(FString const& Url) const;

		bool AddApiAuthorizationIfAvailable(TMap<FString, FString>& Headers) const;

		void AddFlightIdHeader(TMap<FString, FString>& Headers) const;

		static bool IsValidUrl(FString const& Url);

		void ExecuteError(const FErrorHandler& OnError, FString const& ErrorText);

		void ExecuteError(const FCustomErrorHandler& OnError, FString const& ErrorText);

		void ExecuteError(const FCreateMatchmakingTicketErrorHandler& OnError, const FString& ErrorText);

		template<typename T>
		void ExecuteError(const THandler<T>& OnError, FString const& ErrorText)
		{
			OnError.ExecuteIfBound({TEXT("InvalidRequest"), ErrorText});
		}

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
			, FString const& Verb
			, FString const& Url
			, FHttpFormData const& QueryParams
			, TMap<FString, FString> const& Headers
			, U const& OnSuccess
			, V const& OnError)
		{
			if (!IsValidUrl(Url))
			{
				FString ErrorText = FString::Format(TEXT("Invalid URL format {0}"), { Url });
				ExecuteError(OnError, ErrorText);
				return nullptr;
			}

			FString QueryString;

			if (!QueryParams.IsEmpty())
			{
				QueryString.Append("?").Append(QueryParams.ToUrlEncodedString());
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
