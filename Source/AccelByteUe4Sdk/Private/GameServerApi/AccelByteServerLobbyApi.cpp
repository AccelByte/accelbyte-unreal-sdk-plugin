// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerLobby.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "Models/AccelByteLobbyModels.h"

namespace AccelByte
{
namespace GameServerApi
{
	ServerLobby::ServerLobby(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef)
		: CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
		, HttpRef{InHttpRef}
	{}

	ServerLobby::~ServerLobby()
	{}

	void ServerLobby::GetPartyDataByUserId(const FString & UserId, const THandler<FAccelByteModelsDataPartyResponse> OnSuccess, const FErrorHandler & OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (UserId.IsEmpty())
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. User Id is empty."));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/users/%s/party"), *SettingsRef.LobbyServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerLobby::WritePartyStorage(const FString& PartyId, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, uint32 RetryAttempt)
	{
		TSharedPtr<PartyStorageWrapper> Wrapper = MakeShared<PartyStorageWrapper>();
		Wrapper->PartyId = PartyId;
		Wrapper->OnSuccess = OnSuccess;
		Wrapper->OnError = OnError;
		Wrapper->RemainingAttempt = RetryAttempt;
		Wrapper->PayloadModifier = PayloadModifier;
		WritePartyStorageRecursive(Wrapper);
	}

	void ServerLobby::GetPartyStorage(const FString & PartyId, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler & OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties/%s"), *SettingsRef.LobbyServerUrl, *CredentialsRef.GetClientNamespace(), *PartyId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerLobby::RequestWritePartyStorage(const FString &PartyId, const FAccelByteModelsPartyDataUpdateRequest& PartyDataRequest, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, FSimpleDelegate OnConflicted)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties/%s/attributes"), *SettingsRef.LobbyServerUrl, *CredentialsRef.GetClientNamespace(), *PartyId);
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FString Contents = "{\n";
		FString CustomAttribute;
		FJsonObjectConverter::UStructToJsonObjectString(PartyDataRequest.Custom_attribute, CustomAttribute);
		FString UpdatedAt = FString::Printf(TEXT("\"updatedAt\": %lld"), PartyDataRequest.UpdatedAt);
		FString CustomString = FString::Printf(TEXT("\"custom_attribute\": %s"), *CustomAttribute);
		Contents += UpdatedAt;
		Contents += ",\n";
		Contents += CustomString;
		Contents += "}";

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Contents);

		FErrorHandler ErrorHandler = AccelByte::FErrorHandler::CreateLambda([OnConflicted](int32 Code, FString Message)
		{
			if (Code == (int32)ErrorCodes::StatusPreconditionFailed || Code == (int32)ErrorCodes::PartyStorageOutdatedUpdateData)
			{
				OnConflicted.ExecuteIfBound();
			}
		});

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, ErrorHandler), FPlatformTime::Seconds());
	}

	void ServerLobby::WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> DataWrapper)
	{
		if (DataWrapper->RemainingAttempt <= 0)
		{
			DataWrapper->OnError.ExecuteIfBound(412, TEXT("Exhaust all retry attempt to modify party storage.."));
			return;
		}


		GetPartyStorage(DataWrapper->PartyId,
			THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([this, DataWrapper](FAccelByteModelsPartyDataNotif Result)
			{
				Result.Custom_attribute = DataWrapper->PayloadModifier(Result.Custom_attribute);

				FAccelByteModelsPartyDataUpdateRequest PartyStorageBodyRequest;

				PartyStorageBodyRequest.UpdatedAt = FCString::Atoi64(*Result.UpdatedAt);
				PartyStorageBodyRequest.Custom_attribute = Result.Custom_attribute;

				RequestWritePartyStorage(DataWrapper->PartyId, PartyStorageBodyRequest, DataWrapper->OnSuccess, DataWrapper->OnError, FSimpleDelegate::CreateLambda([this, DataWrapper]() {
					DataWrapper->RemainingAttempt--;
					WritePartyStorageRecursive(DataWrapper);
				}));
			}),
			FErrorHandler::CreateLambda([DataWrapper](int32 ErrorCode, FString ErrorMessage)
			{
				DataWrapper->OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		);
	}

	void ServerLobby::GetSessionAttributeAll(const FString& UserId, const THandler<FAccelByteModelsGetSessionAttributeAllResponse>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (UserId.IsEmpty())
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. User Id is empty."));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes"), *SettingsRef.LobbyServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerLobby::GetSessionAttribute(const FString& UserId, const FString& Key, const THandler<FAccelByteModelsGetSessionAttributeResponse>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (UserId.IsEmpty())
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. User Id is empty."));
			return;
		}

		if (Key.IsEmpty())
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. Key is empty."));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes/%s"), *SettingsRef.LobbyServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *Key);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerLobby::SetSessionAttribute(const FString& UserId, const TMap<FString, FString>& Attributes, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (UserId.IsEmpty())
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. User Id is empty."));
			return;
		}

		if (Attributes.Num() == 0)
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. Attributes is empty."));
			return;
		}

		FAccelByteModelsSetSessionAttributeRequest Body;
		Body.Attributes = Attributes;

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes"), *SettingsRef.LobbyServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(Body, Content);

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerLobby::SetSessionAttribute(const FString& UserId, const FString& Key, const FString& Value, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		TMap<FString, FString> Attributes = { { Key, Value } };
		SetSessionAttribute(UserId, Attributes, OnSuccess, OnError);
	}

	void ServerLobby::GetListOfBlockedUsers(const FString& UserId, const THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/player/namespaces/%s/users/%s/blocked"), *SettingsRef.BaseUrl, *CredentialsRef.GetClientNamespace(), *UserId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerLobby::GetListOfBlockers(const FString& UserId, const THandler<FAccelByteModelsListBlockerResponse> OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/player/namespaces/%s/users/%s/blocked-by"), *SettingsRef.BaseUrl, *CredentialsRef.GetClientNamespace(), *UserId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

} // namespace GameServerApi
} // namespace AccelByte