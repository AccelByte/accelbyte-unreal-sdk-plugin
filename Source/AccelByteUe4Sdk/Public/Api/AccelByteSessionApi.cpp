// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteSessionApi.h"

#include "AccelByteUserApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
static void AppendQueryParam(FString& QueryString, const FString& ParamName, const FString& ParamValue)
{
	if(!ParamValue.IsEmpty())
	{
		if(!QueryString.IsEmpty())
		{
			QueryString.Append(TEXT("&"));
		}

		const FString EncodedName = FGenericPlatformHttp::UrlEncode(ParamName);
		const FString EncodedValue = FGenericPlatformHttp::UrlEncode(ParamValue);

		QueryString.Appendf(TEXT("%s=%s"), *EncodedName, *EncodedValue);
	}
}

static void AppendQueryParams(FString& Url, FAccelByteModelsV2SessionQueryRequest const& Query, int32 const& Offset, int32 const& Limit)
{
	FString QueryString   = TEXT("");

	if(Offset >= 0)
	{
		QueryString.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if(Limit >= 0)
	{
		QueryString.Append(QueryString.IsEmpty() ? TEXT("") : TEXT("&"));
		QueryString.Append(FString::Printf(TEXT("limit=%d"), Limit));
	}

	AppendQueryParam(QueryString, TEXT("key"), Query.Key);
	AppendQueryParam(QueryString, TEXT("value"), Query.Value);
	AppendQueryParam(QueryString, TEXT("leaderID"), Query.LeaderID);
	AppendQueryParam(QueryString, TEXT("memberID"), Query.MemberID);

	if(Query.JoinType != EAccelByteV2SessionJoinability::EMPTY)
	{
		const FString JoinType = StaticEnum<EAccelByteV2SessionJoinability>()->GetNameStringByValue(static_cast<int64>(Query.JoinType));
		AppendQueryParam(QueryString, TEXT("joinType"), JoinType);
	}

	if(Query.MemberStatus != EAccelByteV2SessionMemberStatus::EMPTY)
	{
		const FString MemberStatus = StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Query.MemberStatus));
		AppendQueryParam(QueryString, TEXT("memberStatus"), MemberStatus);
	}

	if(!QueryString.IsEmpty())
	{
		Url.Appendf(TEXT("?%s"), *QueryString);
	}
}

static void RemoveEmptyEnumValue(TSharedPtr<FJsonObject> JsonObjectPtr, const FString& FieldName)
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

static void RemoveEmptyEnumValuesFromChildren(TSharedPtr<FJsonObject> JsonObjectPtr, const FString& FieldName)
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

template <typename DataStruct>
static void SerializeAndRemoveEmptyEnumValues(DataStruct Model, FString& OutputString)
{
	auto JsonObjectPtr = FJsonObjectConverter::UStructToJsonObject(Model);

	RemoveEmptyEnumValue(JsonObjectPtr, TEXT("joinType"));
	RemoveEmptyEnumValuesFromChildren(JsonObjectPtr, TEXT("members"));

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObjectPtr.ToSharedRef(), Writer);
}

Session::Session(
	AccelByte::Credentials const& CredentialsRef,
	AccelByte::Settings const& SettingsRef,
	FHttpRetryScheduler& HttpRef)
	:
	HttpRef{HttpRef},
	Credentials{CredentialsRef},
	Settings{SettingsRef}
{}

Session::~Session()
{}

void Session::CreateGameSession(FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesession"), *Settings.SessionServerUrl, *Credentials.GetNamespace());
	FString Verb          = TEXT("POST");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	SerializeAndRemoveEmptyEnumValues(CreateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::GetGameSessionDetails(FString const& GameSessionID, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *GameSessionID);
	FString Verb          = TEXT("GET");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::QueryGameSessions(FAccelByteModelsV2SessionQueryRequest const& Query, THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/gamesessions"), *Settings.SessionServerUrl, *Credentials.GetNamespace());
	FString Verb          = TEXT("GET");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");
	FString QueryString   = TEXT("");

	AppendQueryParams(Url, Query, Offset, Limit);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::UpdateGameSession(FString const& GameSessionID, FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *GameSessionID);
	FString Verb          = TEXT("PUT");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	SerializeAndRemoveEmptyEnumValues(UpdateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::DeleteGameSession(FString const& GameSessionID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *GameSessionID);
	FString Verb          = TEXT("DELETE");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::JoinGameSession(FString const& GameSessionID, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/join"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *GameSessionID);
	FString Verb          = TEXT("POST");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::GetMyGameSessions(THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess, FErrorHandler const& OnError, EAccelByteV2SessionMemberStatus Status)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/gamesessions"), *Settings.SessionServerUrl, *Credentials.GetNamespace());
	FString Verb          = TEXT("GET");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");
	FString QueryString   = TEXT("");

	if(Status != EAccelByteV2SessionMemberStatus::EMPTY)
	{
		const FString StatusString = StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Status));
		AppendQueryParam(QueryString, TEXT("status"), StatusString);
		Url.Appendf(TEXT("?%s"), *QueryString);
	}

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::CreateParty(FAccelByteModelsV2PartyCreateRequest const& CreateRequest, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/party"), *Settings.SessionServerUrl, *Credentials.GetNamespace());
	FString Verb          = TEXT("POST");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");
	
	SerializeAndRemoveEmptyEnumValues(CreateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Session::GetPartyDetails(FString const& PartyID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *PartyID);
	FString Verb          = TEXT("GET");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::UpdateParty(const FString& PartyID, FAccelByteModelsV2PartyUpdateRequest const& UpdateRequest, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *PartyID);
	FString Verb          = TEXT("PUT");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	SerializeAndRemoveEmptyEnumValues(UpdateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Session::SendPartyInvite(FString const& PartyID, FString const& InviteeID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/invite"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *PartyID);
	FString Verb          = TEXT("POST");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");
	
	const FAccelByteModelsV2PartyInviteRequest RequestBody = {InviteeID};
	FJsonObjectConverter::UStructToJsonObjectString(RequestBody, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::RejectPartyInvite(FString const& PartyID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/reject"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *PartyID);
	FString Verb          = TEXT("DELETE");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}


void Session::JoinParty(FString const& PartyID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/join"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *PartyID);
	FString Verb          = TEXT("POST");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::LeaveParty(FString const& PartyID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/me/leave"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *PartyID);
	FString Verb          = TEXT("DELETE");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::KickUserFromParty(FString const& PartyID, FString const& UserID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties/%s/users/%s/kick"), *Settings.SessionServerUrl, *Credentials.GetNamespace(), *PartyID, *UserID);
	FString Verb          = TEXT("DELETE");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::QueryParties(FAccelByteModelsV2SessionQueryRequest const& Query, THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/parties"), *Settings.SessionServerUrl, *Credentials.GetNamespace());
	FString Verb          = TEXT("GET");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");

	AppendQueryParams(Url, Query, Offset, Limit);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Session::GetMyParties(THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess, FErrorHandler const& OnError, EAccelByteV2SessionMemberStatus Status)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/parties"), *Settings.SessionServerUrl, *Credentials.GetNamespace());
	FString Verb          = TEXT("GET");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");
	FString QueryString   = TEXT("");

	if(Status != EAccelByteV2SessionMemberStatus::EMPTY)
	{
		const FString StatusString = StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Status));
		AppendQueryParam(QueryString, TEXT("status"), StatusString);
		Url.Appendf(TEXT("?%s"), *QueryString);
	}

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}


}
}
