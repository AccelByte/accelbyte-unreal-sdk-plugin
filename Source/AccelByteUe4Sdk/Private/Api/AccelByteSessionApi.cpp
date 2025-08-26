// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteSessionApi.h"

#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteError.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
	
void Session::RemoveEmptyEnumValue(TSharedPtr<FJsonObject> & JsonObjectPtr
	, FString const& EnumFieldName)
{
	FString FieldValue;
	if(JsonObjectPtr->TryGetStringField(EnumFieldName, FieldValue))
	{
		if(FieldValue.Equals(TEXT("EMPTY")))
		{
			JsonObjectPtr->SetStringField(EnumFieldName, TEXT(""));
		}
	}
}

void Session::RemoveEmptyEnumValuesFromChildren(TSharedPtr<FJsonObject> & JsonObjectPtr
	, FString const& FieldName
	, FString const& EnumFieldName)
{
	if(JsonObjectPtr->HasTypedField<EJson::Array>(FieldName))
	{
		TArray<TSharedPtr<FJsonValue>> Array = JsonObjectPtr->GetArrayField(FieldName);
		for(auto& Item : Array)
		{
			if(Item->Type == EJson::Object)
			{
				TSharedPtr<FJsonObject> Child = Item->AsObject();
				RemoveEmptyEnumValue(Child, EnumFieldName);
			}
		}
	}
}

Session::Session(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

Session::~Session()
{}

FAccelByteTaskWPtr Session::CreateGameSession(FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesession")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	const bool bIncludeTeams = CreateRequest.Teams.Num() > 0 &&
		(CreateRequest.Teams.Num() > 1 || CreateRequest.Teams[0].UserIDs.Num() > 0);

	FString Content{};
	TSharedPtr<FJsonObject> ContentJsonObject;
	SerializeAndRemoveEmptyValues(CreateRequest, ContentJsonObject, bIncludeTeams);

	// manually add TextChat field if value is set in request
	if (CreateRequest.TextChat.IsSet())
	{
		ContentJsonObject->SetBoolField(TEXT("textChat"), CreateRequest.TextChat.GetValue());
	}

	auto Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(ContentJsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetGameSessionDetails(FString const& GameSessionID
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::QueryGameSessions(FAccelByteModelsV2GameSessionQuery const& QueryObject
	, THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

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
		return nullptr;
	}

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestString, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::UpdateGameSession(FString const& GameSessionID
	, FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	const bool bIncludeTeams = UpdateRequest.Teams.Num() > 0 &&
			(UpdateRequest.Teams.Num() > 1 || UpdateRequest.Teams[0].UserIDs.Num() > 0);

	FString Content{};
	SerializeAndRemoveEmptyValues(UpdateRequest, Content, bIncludeTeams);

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::DeleteGameSession(FString const& GameSessionID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::SendGameSessionInvite(FString const& GameSessionID
	, FString const& UserID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FAccelByteModelsV2SessionInviteRequest RequestBody = {UserID};
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/invite")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::SendGameSessionInvitePlatform(FString const& GameSessionID
	, FString const& UserID
	, EAccelByteV2SessionPlatform const& Platform
	, THandler<FAccelByteModelsV2SessionInvitePlatformResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError)
		|| !ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return nullptr;
	}

	if (Platform == EAccelByteV2SessionPlatform::Unknown)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Platform type cannot be unknown when sending invite!"));
		return nullptr;
	}

	const FAccelByteModelsV2SessionInvitePlatformRequest RequestBody =
	{
		FAccelByteUtilities::GetUEnumValueAsString(Platform).ToUpper(),
		UserID
	};

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/invite")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::RejectGameSessionInvite(FString const& GameSessionID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/reject")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::JoinGameSession(FString const& GameSessionID
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/join")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::JoinGameSessionByCode(FString const& Code
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/join/code")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	TSharedRef<FJsonObject> Content = MakeShared<FJsonObject>();
	Content->SetStringField(TEXT("code"), Code);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GenerateNewGameSessionCode(FString const& GameSessionID
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/code")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Session::RevokeGameSessionCode(FString const& GameSessionID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/code")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::LeaveGameSession(FString const& GameSessionID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/leave")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetMyGameSessions(THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteV2SessionMemberStatus Status)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/gamesessions")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	TMultiMap<FString, FString> QueryParams;
	if(Status != EAccelByteV2SessionMemberStatus::EMPTY)
	{
		QueryParams.Add(TEXT("status"),
			StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Status)));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::PromoteGameSessionLeader(FString const& GameSessionID
	, FString const& NewLeaderID
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError))
	{
		return nullptr;
	}

	FAccelByteModelsV2GameSessionLeaderPromotionRequest RequestBody;
	RequestBody.LeaderID = NewLeaderID;

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/leader")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::CancelGameSessionInvitation(FString const& GameSessionID
	, FString const& UserID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError)
		|| !ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/users/%s/cancel")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID
		, *UserID);
	
	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::KickUserFromGameSession(FString const& GameSessionID
	, FString const& UserID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(GameSessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(GameSessionID)
		, OnError)
		|| !ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserID is invalid"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/members/%s/kick")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *GameSessionID
		, *UserID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::CreateParty(FAccelByteModelsV2PartyCreateRequest const& CreateRequest
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Content{};
	TSharedPtr<FJsonObject> ContentJsonObject;
	SerializeAndRemoveEmptyValues(CreateRequest, ContentJsonObject);

	// manually add TextChat field if value is set in request
	if (CreateRequest.TextChat.IsSet())
	{
		ContentJsonObject->SetBoolField("textChat", CreateRequest.TextChat.GetValue());
	}

	auto Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(ContentJsonObject.ToSharedRef(), Writer);

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/party")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetPartyDetails(FString const& PartyID
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::UpdateParty(FString const& PartyID
	, FAccelByteModelsV2PartyUpdateRequest const& UpdateRequest
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	FString Content = TEXT("");
	SerializeAndRemoveEmptyValues(UpdateRequest, Content);

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::PromotePartyLeader(FString const& PartyID
	, FString const& NewLeaderID
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FAccelByteModelsV2PartyLeaderPromotionRequest RequestBody = {NewLeaderID};
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/leader")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::CancelPartyInvitation(FString const& PartyID
	, FString const& UserID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError)
		|| !ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/%s/cancel")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID
		, *UserID);
	
	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::SendPartyInvite(FString const& PartyID
	, FString const& InviteeID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FAccelByteModelsV2SessionInviteRequest RequestBody = {InviteeID};
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/invite")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::SendPartyInvitePlatform(FString const& PartyID
	, FString const& UserID
	, EAccelByteV2SessionPlatform const& Platform
	, THandler<FAccelByteModelsV2SessionInvitePlatformResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError)
		|| !ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return nullptr;
	}

	if (Platform == EAccelByteV2SessionPlatform::Unknown)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Platform type cannot be unknown when sending invite!"));
		return nullptr;
	}

	const FAccelByteModelsV2SessionInvitePlatformRequest RequestBody =
	{
		FAccelByteUtilities::GetUEnumValueAsString(Platform).ToUpper(),
		UserID
	};
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/invite")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::RejectPartyInvite(FString const& PartyID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/reject")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}


FAccelByteTaskWPtr Session::JoinParty(FString const& PartyID
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/join")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::LeaveParty(FString const& PartyID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/leave")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::JoinPartyByCode(FString const& PartyCode
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (PartyCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid Request, Party code is empty"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/users/me/join/code")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	TSharedRef<FJsonObject> Content = MakeShared<FJsonObject>();
	Content->SetStringField(TEXT("code"), PartyCode);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GenerateNewPartyCode(FString const& PartyID
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/code")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::RevokePartyCode(FString const& PartyID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/code")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::KickUserFromParty(FString const& PartyID
	, FString const& UserID
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError)
		|| !ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/%s/kick")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID
		, *UserID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetMyParties(THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteV2SessionMemberStatus Status)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url  = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/parties")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	TMultiMap<FString, FString> QueryParams;
	if (Status != EAccelByteV2SessionMemberStatus::EMPTY)
	{
		QueryParams.Add(TEXT("status"),
			StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Status)));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetPlayerAttributes(THandler<FAccelByteModelsV2PlayerAttributes> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/attributes")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetPartySessionStorage(FString const& PartyID
	, THandler<FAccelByteModelsV2PartySessionStorage> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/storage")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID
	);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::StorePlayerAttributes(const FAccelByteModelsV2StorePlayerAttributesRequest& AttributesRequest
	, THandler<FAccelByteModelsV2PlayerAttributes> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/attributes")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(AttributesRequest);
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(JsonObject);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, JsonObject.ToSharedRef(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::StorePersonalDataToReservedPartySessionStorage(FString const& PartyID
	, FAccelByteModelsV2PartySessionStorageReservedData const& Data
	, THandler<FAccelByteModelsV2PartySessionStorageReservedData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(PartyID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetPartyIdInvalidMessage(PartyID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/storage/users/%s/reserved")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *PartyID
		, *CredentialsRef->GetUserId());

	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Data);

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, JsonObject.ToSharedRef(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::DeletePlayerAttributes(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/attributes")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::UpdateLeaderStorage(FString const& SessionID
	, FJsonObjectWrapper const& Data
	, THandler<FJsonObjectWrapper> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(SessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(SessionID)
		, OnError))
	{
		return nullptr;
	}
	
	if (!Data.JsonObject.IsValid())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Data has invalid json object!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/sessions/%s/storage/leader")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *SessionID);

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Data.JsonObject.ToSharedRef(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::UpdateMemberStorage(FString const& SessionID
	, FJsonObjectWrapper const& Data
	, THandler<FJsonObjectWrapper> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(SessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(SessionID)
		, OnError))
	{
		return nullptr;
	}
	
	if (!Data.JsonObject.IsValid())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Data has invalid json object!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/sessions/%s/storage/users/%s")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *SessionID
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Data.JsonObject.ToSharedRef(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetRecentPlayers(THandler<FAccelByteModelsV2SessionRecentPlayers> const& OnSuccess,
	FErrorHandler const& OnError, const int32 Limit /* = 20 */)
{
	FReport::Log(FString(__FUNCTION__));

	if (Limit > 200)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Requesting recent player limit with %d will only return 200 items"), Limit);
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/recent-player")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> QueryParam;
	QueryParam.Emplace(TEXT("limit"), FString::FromInt(Limit));
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParam, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetRecentTeamPlayers(THandler<FAccelByteModelsV2SessionRecentPlayers> const& OnSuccess,
	FErrorHandler const& OnError
	, const int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (Limit > 200)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Requesting recent player limit with %d will only return 200 items"), Limit);
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/recent-team-player")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> QueryParam;
	QueryParam.Emplace(TEXT("limit"), FString::FromInt(Limit));
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParam, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Session::GetSessionSecret(FString const& SessionID
	, THandler<FAccelByteModelsV2SessionJoinedSecret> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(SessionID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetSessionIdInvalidMessage(SessionID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/secret")
		, *SettingsRef.SessionServerUrl
		, *CredentialsRef->GetNamespace()
		, *SessionID);	

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte