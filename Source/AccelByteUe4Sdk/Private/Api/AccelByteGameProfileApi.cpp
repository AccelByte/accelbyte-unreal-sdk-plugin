// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteGameProfileApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

GameProfile::GameProfile(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

GameProfile::~GameProfile()
{}

void GameProfile::BatchGetPublicGameProfiles(TArray<FString> UserIds
	, const THandler<TArray<FAccelByteModelsPublicGameProfile>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserIds.Num() < 1)
	{
		OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, TEXT("Please provide at least one user ID."));
	}
	else
	{
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/profiles")
			, *SettingsRef.GameProfileServerUrl
			, *CredentialsRef->GetNamespace());

		for (int i = 0 ; i < UserIds.Num() ; i++)
		{
			Url.Append((i == 0) ? TEXT("?") : TEXT("&")).Append(TEXT("userIds=")).Append(UserIds[i]);
		}

		HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
	}
}

void GameProfile::GetAllGameProfiles(const THandler<TArray<FAccelByteModelsGameProfile>>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void GameProfile::CreateGameProfile(const FAccelByteModelsGameProfileRequest & GameProfileRequest
	, const THandler<FAccelByteModelsGameProfile>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, GameProfileRequest, OnSuccess, OnError);
}

void GameProfile::GetGameProfile(const FString & ProfileId
	, const THandler<FAccelByteModelsGameProfile>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void GameProfile::UpdateGameProfile(const FString & ProfileId
	, const FAccelByteModelsGameProfileRequest & GameProfileRequest
	, const THandler<FAccelByteModelsGameProfile>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, GameProfileRequest, OnSuccess, OnError);
}

void GameProfile::DeleteGameProfile(const FString & ProfileId
	, const FVoidHandler& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void GameProfile::GetGameProfileAttribute(const FString & ProfileId
	, const FString & AttributeName
	, const THandler<FAccelByteModelsGameProfileAttribute>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s/attributes/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId, *AttributeName);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void GameProfile::UpdateGameProfileAttribute(const FString & ProfileId
	, const FAccelByteModelsGameProfileAttribute& Attribute
	, const THandler<FAccelByteModelsGameProfile>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s/attributes/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId
		, *Attribute.name);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Attribute, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
