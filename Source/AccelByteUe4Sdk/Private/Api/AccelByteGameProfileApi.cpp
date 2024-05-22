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

FAccelByteTaskWPtr GameProfile::BatchGetPublicGameProfiles(TArray<FString> const& UserIds
	, THandler<TArray<FAccelByteModelsPublicGameProfile>> const& OnSuccess
	, FErrorHandler const& OnError)
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

		return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
	}
	return nullptr;
}

FAccelByteTaskWPtr GameProfile::GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr GameProfile::CreateGameProfile(FAccelByteModelsGameProfileRequest const& GameProfileRequest
	, THandler<FAccelByteModelsGameProfile> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, GameProfileRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr GameProfile::GetGameProfile(FString const& ProfileId
	, THandler<FAccelByteModelsGameProfile> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr GameProfile::UpdateGameProfile(FString const& ProfileId
	, FAccelByteModelsGameProfileRequest const& GameProfileRequest
	, THandler<FAccelByteModelsGameProfile> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, GameProfileRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr GameProfile::DeleteGameProfile(FString const& ProfileId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr GameProfile::GetGameProfileAttribute(FString const& ProfileId
	, FString const& AttributeName
	, THandler<FAccelByteModelsGameProfileAttribute> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s/attributes/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId, *AttributeName);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr GameProfile::UpdateGameProfileAttribute(FString const& ProfileId
	, FAccelByteModelsGameProfileAttribute const& Attribute
	, THandler<FAccelByteModelsGameProfile> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s/attributes/%s")
		, *SettingsRef.GameProfileServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *ProfileId
		, *Attribute.name);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Attribute, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
