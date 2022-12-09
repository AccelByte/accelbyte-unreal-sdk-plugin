// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerSessionApi.h"

#include "Api/AccelByteSessionApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
namespace GameServerApi
{
ServerSession::ServerSession(
	ServerCredentials const& InCredentialsRef,
	ServerSettings const& InSettingsRef,
	FHttpRetryScheduler& InHttpRef)
	: 
	CredentialsRef{ InCredentialsRef },
	SettingsRef{ InSettingsRef },
	HttpRef{ InHttpRef }
{}

ServerSession::~ServerSession()
{}

void ServerSession::CreateGameSession(
	FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest,
	THandler<FAccelByteModelsV2GameSession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesession"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetClientNamespace());

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
		return;
	}

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerSession::GetGameSessionDetails(
	FString const& GameSessionID,
	THandler<FAccelByteModelsV2GameSession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetClientNamespace(), *GameSessionID);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerSession::UpdateGameSession(
	FString const& GameSessionID,
	FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest,
	THandler<FAccelByteModelsV2GameSession> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetClientNamespace(), *GameSessionID);

	const bool bIncludeTeams = UpdateRequest.Teams.Num() > 0 &&
		(UpdateRequest.Teams.Num() > 1 || UpdateRequest.Teams[0].UserIDs.Num() > 0);

	FString Content{};
	Api::Session::SerializeAndRemoveEmptyValues(UpdateRequest, Content, bIncludeTeams);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(TEXT("PATCH"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerSession::DeleteGameSession(
	FString const& GameSessionID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetClientNamespace(), *GameSessionID);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(TEXT("DELETE"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerSession::SendGameSessionInvite(
	FString const& GameSessionID,
	FString const& UserID,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/gamesessions/%s/invite"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetClientNamespace(), *GameSessionID);

	const FAccelByteModelsV2SessionInviteRequest Invite = {UserID};
	FString Content{};
	if (!FJsonObjectConverter::UStructToJsonObjectString(Invite, Content))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to convert request JSON into a string"));
		return;
	}

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(TEXT("POST"));
	Request->SetContentAsString(Content);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerSession::UpdateMemberStatus(
	FString const& GameSessionID,
	FString const& MemberID,
	EAccelByteV2SessionMemberStatus const& Status,
	FVoidHandler const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString StatusString = StaticEnum<EAccelByteV2SessionMemberStatus>()->GetNameStringByValue(static_cast<int64>(Status));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/gamesessions/%s/members/%s/status/%s"),
		*SettingsRef.SessionServerUrl, *CredentialsRef.GetClientNamespace(), *GameSessionID, *MemberID, *StatusString);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(TEXT("PUT"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

}
}
