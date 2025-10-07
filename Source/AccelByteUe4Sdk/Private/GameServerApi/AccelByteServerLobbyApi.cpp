// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerLobby.h"

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
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InServerApiClient)
{}

ServerLobby::~ServerLobby()
{}

FAccelByteTaskWPtr ServerLobby::GetPartyDataByUserId(FString const& UserId
	, THandler<FAccelByteModelsDataPartyResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/users/%s/party")
		, *ServerSettingsRef.LobbyServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerLobby::WritePartyStorage(FString const& PartyId
	, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier
	, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 RetryAttempt)
{
	TSharedPtr<PartyStorageWrapper> Wrapper = MakeShared<PartyStorageWrapper>();
	Wrapper->PartyId = PartyId;
	Wrapper->OnSuccess = OnSuccess;
	Wrapper->OnError = OnError;
	Wrapper->RemainingAttempt = RetryAttempt;
	Wrapper->PayloadModifier = PayloadModifier;
	WritePartyStorageRecursive(Wrapper);
	return nullptr;
}

FAccelByteTaskWPtr ServerLobby::GetPartyStorage(FString const& PartyId
	, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties/%s")
		, *ServerSettingsRef.LobbyServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(PartyId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerLobby::RequestWritePartyStorage(FString const& PartyId
	, FAccelByteModelsPartyDataUpdateRequest const& PartyDataRequest
	, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
	, FErrorHandler const& OnError
	, FSimpleDelegate OnConflicted)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties/%s/attributes")
		, *ServerSettingsRef.LobbyServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(PartyId));

	FString Contents = "{\n";
	FString CustomAttribute;
	FJsonObjectConverter::UStructToJsonObjectString(PartyDataRequest.Custom_attribute, CustomAttribute);
	FString UpdatedAt = FString::Printf(TEXT("\"updatedAt\": %lld"), PartyDataRequest.UpdatedAt);
	FString CustomString = FString::Printf(TEXT("\"custom_attribute\": %s"), *CustomAttribute);
	Contents += UpdatedAt;
	Contents += ",\n";
	Contents += CustomString;
	Contents += "}";

	const FErrorHandler ErrorHandler = FErrorHandler::CreateLambda(
		[OnConflicted](int32 Code, FString Message)
		{
			if (Code == static_cast<int32>(ErrorCodes::StatusPreconditionFailed) || Code == static_cast<int32>(ErrorCodes::PartyStorageOutdatedUpdateData))
			{
				OnConflicted.ExecuteIfBound();
			}
		});

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Contents, OnSuccess, OnError);
}

void ServerLobby::WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> DataWrapper)
{
	if (DataWrapper->RemainingAttempt <= 0)
	{
		DataWrapper->OnError.ExecuteIfBound(412, TEXT("Exhaust all retry attempt to modify party storage.."));
		return;
	}

	GetPartyStorage(DataWrapper->PartyId
		, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[this, DataWrapper](FAccelByteModelsPartyDataNotif Result)
			{
				Result.Custom_attribute = DataWrapper->PayloadModifier(Result.Custom_attribute);

				FAccelByteModelsPartyDataUpdateRequest PartyStorageBodyRequest;

				PartyStorageBodyRequest.UpdatedAt = FCString::Atoi64(*Result.UpdatedAt);
				PartyStorageBodyRequest.Custom_attribute = Result.Custom_attribute;

				RequestWritePartyStorage(DataWrapper->PartyId, PartyStorageBodyRequest, DataWrapper->OnSuccess, DataWrapper->OnError, FSimpleDelegate::CreateLambda([this, DataWrapper]() {
					DataWrapper->RemainingAttempt--;
					WritePartyStorageRecursive(DataWrapper);
				}));
			})
		, FErrorHandler::CreateLambda(
			[DataWrapper](int32 ErrorCode, FString ErrorMessage)
			{
				DataWrapper->OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		);
}

FAccelByteTaskWPtr ServerLobby::GetSessionAttributeAll(FString const& UserId
	, THandler<FAccelByteModelsGetSessionAttributeAllResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes")
		, *ServerSettingsRef.LobbyServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerLobby::GetSessionAttribute(FString const& UserId
	, FString const& Key
	, THandler<FAccelByteModelsGetSessionAttributeResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Key is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes/%s")
		, *ServerSettingsRef.LobbyServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(Key));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerLobby::SetSessionAttribute(FString const& UserId
	, TMap<FString, FString> const& Attributes
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if (Attributes.Num() == 0)
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Attributes is empty."));
		return nullptr;
	}

	FAccelByteModelsSetSessionAttributeRequest Body;
	Body.Attributes = Attributes;

	const FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes")
		, *ServerSettingsRef.LobbyServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Body, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerLobby::SetSessionAttribute(FString const& UserId
	, FString const& Key
	, FString const& Value
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	TMap<FString, FString> Attributes = { { Key, Value } };
	return SetSessionAttribute(UserId, Attributes, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerLobby::GetListOfBlockedUsers(FString const& UserId
	, THandler<FAccelByteModelsListBlockedUserResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/player/namespaces/%s/users/%s/blocked")
		, *ServerSettingsRef.BaseUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerLobby::GetListOfBlockers(FString const& UserId
	, THandler<FAccelByteModelsListBlockerResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/player/namespaces/%s/users/%s/blocked-by")
		, *ServerSettingsRef.BaseUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

} // namespace GameServerApi
} // namespace AccelByte