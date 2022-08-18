// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteSessionApi.h"

#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
void Session::RemoveEmptyEnumValue(TSharedPtr<FJsonObject> JsonObjectPtr, const FString& FieldName)
{
	FString FieldValue;
	if(JsonObjectPtr->TryGetStringField(FieldName, FieldValue))
	{
		if(FieldValue.Equals(TEXT("EMPTY")))
		{
			JsonObjectPtr->SetStringField(FieldName, TEXT(""));
		}
	}
}

void Session::RemoveEmptyEnumValuesFromChildren(TSharedPtr<FJsonObject> JsonObjectPtr, const FString& FieldName)
{
	if(JsonObjectPtr->HasTypedField<EJson::Array>(FieldName))
	{
		TArray<TSharedPtr<FJsonValue>> Array = JsonObjectPtr->GetArrayField(FieldName);
		for(auto& Item : Array)
		{
			if(Item->Type == EJson::Object)
			{
				TSharedPtr<FJsonObject> Child = Item->AsObject();
				RemoveEmptyEnumValue(Child, TEXT("status"));
			}
		}
	}
}

Session::Session(
	Credentials const& InCredentialsRef,
	Settings const& InSettingsRef,
	FHttpRetryScheduler& InHttpRef)
	:
	FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Session::~Session()
{}

void Session::CreateGameSession(
	FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest,
	THandler<FAccelByteModelsV2GameSession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesession"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace());

	FString Content = TEXT("");
	SerializeAndRemoveEmptyValues(CreateRequest, Content);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void Session::GetGameSessionDetails(
	FString const& GameSessionID,
	THandler<FAccelByteModelsV2GameSession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *GameSessionID);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::QueryGameSessions(
	FAccelByteModelsV2GameSessionQuery const& QueryObject,
	THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess,
	FErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace());

	// Add pagination information to query object
	QueryObject.JsonWrapper.JsonObject->SetNumberField(TEXT("offset"), Offset);
	QueryObject.JsonWrapper.JsonObject->SetNumberField(TEXT("limit"), Limit);

	// Convert query object to JSON string
	FString RequestString;
	const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&RequestString);
	if (!FJsonSerializer::Serialize(QueryObject.JsonWrapper.JsonObject.ToSharedRef(), JsonWriter))
	{
		OnError.ExecuteIfBound(
			static_cast<int32>(ErrorCodes::InvalidRequest),
			TEXT("Failed to send query game session request as our query JSON object failed to serialize to string!"));
		return;
	}

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestString, OnSuccess, OnError);
}

void Session::UpdateGameSession(
	FString const& GameSessionID,
	FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest,
	THandler<FAccelByteModelsV2GameSession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *GameSessionID);

	FString Content = TEXT("");
	SerializeAndRemoveEmptyValues(UpdateRequest, Content);

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Content, OnSuccess, OnError);
}

void Session::DeleteGameSession(
	FString const& GameSessionID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *GameSessionID);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::SendGameSessionInvite(
	FString const& GameSessionID,
	FString const& UserID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FAccelByteModelsV2SessionInviteRequest RequestBody = {UserID};
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/invite"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *GameSessionID);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

void Session::RejectGameSessionInvite(
	FString const& GameSessionID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/reject"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *GameSessionID);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::JoinGameSession(
	FString const& GameSessionID,
	THandler<FAccelByteModelsV2GameSession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/join"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *GameSessionID);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::LeaveGameSession(
	FString const& GameSessionID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/leave"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *GameSessionID);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::GetMyGameSessions(
	THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess,
	FErrorHandler const& OnError,
	EAccelByteV2SessionMemberStatus Status)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/gamesessions"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace());

	TMap<FString, FString> QueryParams;
	if(Status != EAccelByteV2SessionMemberStatus::EMPTY)
	{
		QueryParams.Add(TEXT("status"),
			StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Status)));
	}
	
	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Session::CreateParty(
	FAccelByteModelsV2PartyCreateRequest const& CreateRequest,
	THandler<FAccelByteModelsV2PartySession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Content = TEXT("");
	SerializeAndRemoveEmptyValues(CreateRequest, Content);

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/party"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace());
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}
	
void Session::GetPartyDetails(
	FString const& PartyID,
	THandler<FAccelByteModelsV2PartySession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *PartyID);
	
	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::UpdateParty(
	const FString& PartyID,
	FAccelByteModelsV2PartyUpdateRequest const& UpdateRequest,
	THandler<FAccelByteModelsV2PartySession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Content = TEXT("");
	SerializeAndRemoveEmptyValues(UpdateRequest, Content);

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *PartyID);

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Content, OnSuccess, OnError);
}
	
void Session::SendPartyInvite(
	FString const& PartyID,
	FString const& InviteeID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FAccelByteModelsV2SessionInviteRequest RequestBody = {InviteeID};
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/invite"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *PartyID);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

void Session::RejectPartyInvite(
	FString const& PartyID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/reject"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *PartyID);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}


void Session::JoinParty(
	FString const& PartyID,
	THandler<FAccelByteModelsV2PartySession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/join"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *PartyID);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::LeaveParty(
	FString const& PartyID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/leave"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *PartyID);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::KickUserFromParty(
	FString const& PartyID,
	FString const& UserID,
	THandler<FAccelByteModelsV2PartySession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/%s/kick"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace(), *PartyID, *UserID);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void Session::GetMyParties(
	THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess,
	FErrorHandler const& OnError,
	EAccelByteV2SessionMemberStatus Status)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url  = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/parties"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetNamespace());

	TMap<FString, FString> QueryParams;
	if(Status != EAccelByteV2SessionMemberStatus::EMPTY)
	{
		QueryParams.Add(TEXT("status"),
			StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Status)));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

}
}
