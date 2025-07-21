// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerSessionApi.h"

#include "Api/AccelByteSessionApi.h"
#include "Core/AccelByteError.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerSession::ServerSession(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InServerApiClient)
{}

ServerSession::~ServerSession()
{}

FAccelByteTaskWPtr ServerSession::CreateGameSession(FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesession")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const bool bIncludeTeams = CreateRequest.Teams.Num() > 0 &&
		(CreateRequest.Teams.Num() > 1 || CreateRequest.Teams[0].UserIDs.Num() > 0);

	TSharedPtr<FJsonObject> CreateRequestJsonObject;
	Api::Session::SerializeAndRemoveEmptyValues(CreateRequest, CreateRequestJsonObject, bIncludeTeams);

	// manually add TextChat field if value is set in request
	if (CreateRequest.TextChat.IsSet())
	{
		CreateRequestJsonObject->SetBoolField(TEXT("textChat"), CreateRequest.TextChat.GetValue());
	}

	FString Content;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	if (!FJsonSerializer::Serialize(CreateRequestJsonObject.ToSharedRef(), Writer))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to serialize create request body to JSON object"));
		return nullptr;
	}

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::GetGameSessionDetails(FString const& GameSessionID
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::UpdateGameSession(FString const& GameSessionID
	, FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *GameSessionID);

	FAccelByteModelsV2GameSessionUpdateRequest RequestToSend = UpdateRequest;

	// we intentionally wants to clear teams content.
	// backend will only process this field if at least Teams array consist of 1 empty element.
	// so make sure we have 1 teams with no content.
	if(UpdateRequest.bIncludeEmptyTeams && UpdateRequest.Teams.Num() == 0)
	{
		RequestToSend.Teams.AddDefaulted();
	}
	
	const bool bIsTeamsNotEmpty = (RequestToSend.Teams.Num() > 0 && (RequestToSend.Teams.Num() > 1 || RequestToSend.Teams[0].UserIDs.Num() > 0));

	const bool bIncludeTeams = RequestToSend.bIncludeEmptyTeams || bIsTeamsNotEmpty;

	FString Content{};
	Api::Session::SerializeAndRemoveEmptyValues(RequestToSend, Content, bIncludeTeams);

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::DeleteGameSession(FString const& GameSessionID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::UpdateMemberStatus(FString const& GameSessionID
	, FString const& MemberID
	, EAccelByteV2SessionMemberStatus Status
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString StatusString = StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Status));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/gamesessions/%s/members/%s/status/%s")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *GameSessionID
		, *MemberID
		, *StatusString);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::QueryPartySessions(FAccelByteModelsV2QueryPartiesRequest const& RequestContent
	, THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int64 Offset /* = 0 */
	, int64 Limit /* = 20 */)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *ServerCredentialsRef->GetClientAccessToken());

	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/parties"),
								  *ServerSettingsRef.SessionServerUrl, *ServerCredentialsRef->GetClientNamespace());

	FString ContentBody = TEXT("");
	const TSharedPtr<FJsonObject> QueryRequestJsonObject = FJsonObjectConverter::UStructToJsonObject(RequestContent);
	FAccelByteUtilities::RemoveEmptyStrings(QueryRequestJsonObject);

	TMultiMap<FString, FString> QueryParams{};
	for (const TPair<FString, TSharedPtr<FJsonValue>>& KV : QueryRequestJsonObject->Values)
	{
		// Check if the value of this pair is a valid shared instance, if not bail
		if (!KV.Value.IsValid())
		{
			continue;
		}

		// Attempt to get the value pointed to as a string, and make sure that it is not empty if we are able to grab it
		FString ValueStr{};
		if (!KV.Value->TryGetString(ValueStr) || ValueStr.IsEmpty())
		{
			continue;
		}

		// Avoid empty enum values for MemberStatus and JoinType
		if (KV.Key.Equals(TEXT("memberStatus"), ESearchCase::IgnoreCase) || KV.Key.Equals(TEXT("joinType"), ESearchCase::IgnoreCase))
		{
			if (ValueStr.Equals(TEXT("empty"), ESearchCase::IgnoreCase))
			{
				continue;
			}
		}

		if(KV.Key.Equals(TEXT("memberId")))
		{
			QueryParams.Add(TEXT("memberID"), ValueStr);
		}
		else if(KV.Key.Equals(TEXT("leaderId")))
		{
			QueryParams.Add(TEXT("leaderID"), ValueStr);
		}
		else if(KV.Key.Equals(TEXT("joinType")))
		{
			QueryParams.Add(TEXT("joinability"), ValueStr);
		}
		else
		{
			// Add to the mapping of query parameters
			QueryParams.Add(KV.Key, ValueStr);
		}
	}

	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}

	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::QueryPartySessions(FAccelByteModelsV2QueryPartiesRequest const& RequestContent
	, THandler<FAccelByteModelsV2QueryPartiesResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int64 Offset
	, int64 Limit)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("Use ServerSession::QueryPartySessions with paginated support to match backend model"));

	return QueryPartySessions(RequestContent,
		THandler<FAccelByteModelsV2PaginatedPartyQueryResult>::CreateLambda([OnSuccess](const FAccelByteModelsV2PaginatedPartyQueryResult QueryPartiesResponse)
		{
			OnSuccess.ExecuteIfBound(FAccelByteModelsV2QueryPartiesResponse {QueryPartiesResponse.Data});
		}), OnError, Offset, Limit);
}

FAccelByteTaskWPtr ServerSession::GetPartyDetails(FString const& PartyID
	, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *ServerCredentialsRef->GetClientAccessToken());
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s"),
		*ServerSettingsRef.SessionServerUrl, *ServerCredentialsRef->GetClientNamespace(), *PartyID);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::BulkGetPlayerAttributes(TArray<FString> const& UserIds
	, THandler<TArray<FAccelByteModelsV2PlayerAttributes>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/attributes")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	TMultiMap<FString, FString> QueryParams{
		{ TEXT("users"), FString::Join(UserIds, TEXT(",")) }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::GetPlayerAttributes(FString const& UserId
	, THandler<FAccelByteModelsV2PlayerAttributes> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/attributes")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::QueryGameSessions(FAccelByteModelsV2ServerQueryGameSessionsRequest const& RequestContent
	, THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int64 Offset
	, int64 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/gamesessions"),
								  *ServerSettingsRef.SessionServerUrl, *ServerCredentialsRef->GetClientNamespace());

	const TSharedPtr<FJsonObject> QueryRequestJsonObject = FJsonObjectConverter::UStructToJsonObject(RequestContent);
	FAccelByteUtilities::RemoveEmptyStrings(QueryRequestJsonObject);

	TMap<FString, FString> QueryParams{};

	for (const TPair<FString, TSharedPtr<FJsonValue>>& KV : QueryRequestJsonObject->Values)
	{
		// Check if the value of this pair is a valid shared instance, if not bail
		if (!KV.Value.IsValid())
		{
			continue;
		}

		// Attempt to get the value pointed to as a string, and make sure that it is not empty if we are able to grab it
		FString ValueStr{};
		if (!KV.Value->TryGetString(ValueStr) || ValueStr.IsEmpty())
		{
			continue;
		}

		// Avoid adding empty enum and default FDateTime to query params
		if (ValueStr.Equals(TEXT("empty"), ESearchCase::IgnoreCase) || ValueStr.Equals(FDateTime{0}.ToString(), ESearchCase::IgnoreCase))
		{
			continue;
		}

		// Convert FDateTime string representation from UnrealEngine default format to RFC 3339
		if (KV.Key.Equals(TEXT("fromTime")) || KV.Key.Equals(TEXT("toTime")))
		{
			FDateTime OutDateTime;
			if (FDateTime::Parse(ValueStr, OutDateTime))
			{
				ValueStr = OutDateTime.ToString(TEXT("%Y-%m-%dT%H:%M:%SZ"));
			}
			else
			{
				continue;
			}
		}

		// Convert bool value to upper case to match BE convention (e.g. TRUE or FALSE) for these query params
		if (KV.Key.Equals(TEXT("isSoftDeleted")) || KV.Key.Equals(TEXT("isPersistent")))
		{
			ValueStr = ValueStr.ToUpper();
		}

		QueryParams.Add(KV.Key, ValueStr);
	}

	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}

	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::PromoteGameSessionLeader(FString const& GameSessionID
	, FString const& NewLeaderID
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FAccelByteModelsV2GameSessionLeaderPromotionRequest RequestBody;
	RequestBody.LeaderID = NewLeaderID;

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/leader")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestBody, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::GenerateNewGameSessionCode(FString const& GameSessionID
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/code")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::RevokeGameSessionCode(FString const& GameSessionID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/code")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::KickUserFromGameSession(FString const& GameSessionID, FString const& UserID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (GameSessionID.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("GameSessionID is empty!"));
		return nullptr;
	}

	if (!ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserID is invalid"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/gamesessions/%s/members/%s/kick")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetNamespace()
		, *GameSessionID
		, *UserID);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::SendDSSessionReady(FString const& GameSessionID
	, bool bDSSessionReady
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/gamesessions/%s/ds")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *GameSessionID);

	FAccelByteModelsV2DSSessionReadyRequest Request;
	Request.Ready = bDSSessionReady;

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::GetRecentPlayers(FString const& UserId
	, THandler<FAccelByteModelsV2SessionRecentPlayers> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit /* = 20 */)
{
	FReport::Log(FString(__FUNCTION__));

	if(Limit > 200)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Requesting recent player limit with %d will only return 200 items"), Limit);
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/recent-player")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetNamespace());

	TMap<FString, FString> QueryParam;
	QueryParam.Emplace(TEXT("userId"), UserId);
	QueryParam.Emplace(TEXT("limit"), FString::FromInt(Limit));
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParam, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::GetRecentTeamPlayers(FString const& UserId
	, THandler<FAccelByteModelsV2SessionRecentPlayers> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if(Limit > 200)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Requesting recent player limit with %d will only return 200 items"), Limit);
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/recent-team-player")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetNamespace());

	TMap<FString, FString> QueryParam;
	QueryParam.Emplace(TEXT("userId"), UserId);
	QueryParam.Emplace(TEXT("limit"), FString::FromInt(Limit));
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParam, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSession::UpdateDSInformation(FString const& GameSessionID
	, FAccelByteModelsGameSessionUpdateDSInformationRequest const& DSInformation
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/gamesessions/%s/dsinformation")
		, *ServerSettingsRef.SessionServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *GameSessionID);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, DSInformation, OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
