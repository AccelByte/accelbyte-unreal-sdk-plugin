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
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

ServerLobby::~ServerLobby()
{}

void ServerLobby::GetPartyDataByUserId(const FString & UserId
	, const THandler<FAccelByteModelsDataPartyResponse> OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/users/%s/party")
		, *ServerSettingsRef.LobbyServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerLobby::WritePartyStorage(const FString& PartyId
	, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier
	, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess
	, const FErrorHandler& OnError
	, uint32 RetryAttempt)
{
	TSharedPtr<PartyStorageWrapper> Wrapper = MakeShared<PartyStorageWrapper>();
	Wrapper->PartyId = PartyId;
	Wrapper->OnSuccess = OnSuccess;
	Wrapper->OnError = OnError;
	Wrapper->RemainingAttempt = RetryAttempt;
	Wrapper->PayloadModifier = PayloadModifier;
	WritePartyStorageRecursive(Wrapper);
}

void ServerLobby::GetPartyStorage(const FString & PartyId
	, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties/%s")
		, *ServerSettingsRef.LobbyServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *PartyId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerLobby::RequestWritePartyStorage(const FString &PartyId
	, const FAccelByteModelsPartyDataUpdateRequest& PartyDataRequest
	, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess
	, const FErrorHandler& OnError
	, FSimpleDelegate OnConflicted)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties/%s/attributes")
		, *ServerSettingsRef.LobbyServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *PartyId);

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

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Contents, OnSuccess, OnError);
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

void ServerLobby::GetSessionAttributeAll(const FString& UserId
	, const THandler<FAccelByteModelsGetSessionAttributeAllResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes")
		, *ServerSettingsRef.LobbyServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerLobby::GetSessionAttribute(const FString& UserId
	, const FString& Key
	, const THandler<FAccelByteModelsGetSessionAttributeResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Key is empty."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes/%s")
		, *ServerSettingsRef.LobbyServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *Key);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerLobby::SetSessionAttribute(const FString& UserId
	, const TMap<FString, FString>& Attributes
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	if (Attributes.Num() == 0)
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Attributes is empty."));
		return;
	}

	FAccelByteModelsSetSessionAttributeRequest Body;
	Body.Attributes = Attributes;

	const FString Url = FString::Printf(TEXT("%s/v1/admin/player/namespaces/%s/users/%s/attributes")
		, *ServerSettingsRef.LobbyServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Body, OnSuccess, OnError);
}

void ServerLobby::SetSessionAttribute(const FString& UserId
	, const FString& Key
	, const FString& Value
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	TMap<FString, FString> Attributes = { { Key, Value } };
	SetSessionAttribute(UserId, Attributes, OnSuccess, OnError);
}

void ServerLobby::GetListOfBlockedUsers(const FString& UserId
	, const THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/player/namespaces/%s/users/%s/blocked")
		, *ServerSettingsRef.BaseUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerLobby::GetListOfBlockers(const FString& UserId
	, const THandler<FAccelByteModelsListBlockerResponse> OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/player/namespaces/%s/users/%s/blocked-by")
		, *ServerSettingsRef.BaseUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

} // namespace GameServerApi
} // namespace AccelByte