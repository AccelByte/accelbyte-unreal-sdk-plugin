// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "GameServerApi/AccelByteServerUserApi.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerUser::ServerUser(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

ServerUser::~ServerUser()
{
}

void ServerUser::SearchUserOtherPlatformDisplayName(const FString& DisplayName
	, EAccelBytePlatformType PlatformType
	, const THandler<FPagedUserOtherPlatformInfo>& OnSuccess
	, const FErrorHandler& OnError
	, const int32& Limit
	, const int32& Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (PlatformType == EAccelBytePlatformType::Nintendo || PlatformType == EAccelBytePlatformType::Oculus || PlatformType == EAccelBytePlatformType::Apple)
	{
		OnError.ExecuteIfBound(400, TEXT("Cannot search user using this function. Use SearchUserOtherPlatformUserId instead."));
		return;
	}

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/search")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	const TMap<FString, FString> QueryParams = {
		{TEXT("by"), TEXT("thirdPartyPlatform")},
		{TEXT("limit"), FString::FromInt(Limit)},
		{TEXT("offset"), FString::FromInt(Limit)},
		{TEXT("platformBy"), TEXT("platformDisplayName")},
		{TEXT("platformId"), *PlatformId},
		{TEXT("query"), *DisplayName}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerUser::SearchUserOtherPlatformUserId(const FString& PlatformUserId
	, EAccelBytePlatformType PlatformType
	, const THandler<FUserOtherPlatformInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/platforms/%s/users/%s")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *PlatformId
		, *PlatformUserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerUser::BanSingleUser(const FString& UserId
	, const FBanUserRequest& BanUser
	, const THandler<FBanUserResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/%s/bans")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, BanUser, OnSuccess, OnError);
}

void ServerUser::GetUserBans(const FString& UserId
	, const FDateTime& Before
	, const FDateTime& After
	, const THandler<FGetUserBansResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/%s/bans")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	const TMap<FString, FString> QueryParams = {
		{TEXT("activeOnly"), TEXT("true")},
		{TEXT("after"), *After.ToIso8601()},
		{TEXT("before"), *Before.ToIso8601()}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerUser::GetUserBanInfo(const FString& UserId, const THandler<FGetUserBansResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/%s/bans?activeOnly=true")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);
	
	HttpClient.ApiRequest(TEXT("GET"), Url, OnSuccess, OnError);
}
	
}
}
