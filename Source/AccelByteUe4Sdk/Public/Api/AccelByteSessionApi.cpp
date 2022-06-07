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

void Session::CreateParty(FAccelByteModelsV2PartyCreateRequest const& CreateRequest, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/party"), *Settings.SessionServerUrl, *Credentials.GetNamespace());
	FString Verb          = TEXT("POST");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");
	
	FJsonObjectConverter::UStructToJsonObjectString(CreateRequest, Content);
	
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
    
    FJsonObjectConverter::UStructToJsonObjectString(UpdateRequest, Content);
    
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

void Session::QueryParties(FAccelByteModelsV2SessionQueryRequest const& Query, THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url           = FString::Printf(TEXT("%s/v1/public/namespaces/%s/parties"), *Settings.SessionServerUrl, *Credentials.GetNamespace());
	FString Verb          = TEXT("GET");
	FString ContentType   = TEXT("application/json");
	FString Accept        = TEXT("application/json");
	FString Content       = TEXT("");
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
	AppendQueryParam(QueryString, TEXT("joinType"), Query.JoinType);
	AppendQueryParam(QueryString, TEXT("memberStatus"), Query.MemberStatus);
	AppendQueryParam(QueryString, TEXT("leaderID"), Query.LeaderID);
	AppendQueryParam(QueryString, TEXT("memberID"), Query.MemberID);

	if(!QueryString.IsEmpty())
	{
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
