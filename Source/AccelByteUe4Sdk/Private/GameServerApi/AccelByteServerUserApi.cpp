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

FAccelByteTaskWPtr ServerUser::SearchUserOtherPlatformDisplayName(FString const& DisplayName
	, EAccelBytePlatformType PlatformType
	, THandler<FPagedUserOtherPlatformInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, bool bIsSearchByUniqueDisplayName)
{
	FReport::Log(FString(__FUNCTION__));

	if (PlatformType == EAccelBytePlatformType::Nintendo || PlatformType == EAccelBytePlatformType::Oculus || PlatformType == EAccelBytePlatformType::Apple)
	{
		OnError.ExecuteIfBound(400, TEXT("Cannot search user using this function. Use SearchUserOtherPlatformUserId instead."));
		return nullptr;
	}

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/search")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("by"), bIsSearchByUniqueDisplayName ? TEXT("uniqueDisplayName") : TEXT("thirdPartyPlatform") },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("platformBy"), TEXT("platformDisplayName") },
		{ TEXT("platformId"), *PlatformId },
		{ TEXT("query"), *DisplayName }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerUser::SearchUserOtherPlatformUserId(FString const& PlatformUserId
	, EAccelBytePlatformType PlatformType
	, THandler<FUserOtherPlatformInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/platforms/%s/users/%s")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *PlatformId
		, *PlatformUserId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerUser::BanSingleUser(FString const& UserId
	, FBanUserRequest const& BanUser
	, THandler<FBanUserResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/%s/bans")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, BanUser, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerUser::GetUserBans(FString const& UserId
	, FDateTime const& Before
	, FDateTime const& After
	, THandler<FGetUserBansResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/%s/bans")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("activeOnly"), TEXT("true")},
		{TEXT("after"), *After.ToIso8601()},
		{TEXT("before"), *Before.ToIso8601()}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerUser::GetUserBanInfo(FString const& UserId
	, THandler<FGetUserBansResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/%s/bans?activeOnly=true")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerUser::ListUserByUserId(FListUserDataRequest const& Request
	, THandler<FListUserDataResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/bulk")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef->GetClientNamespace() );
 
	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerUser::BulkGetUserInfo(TArray<FString> const& UserIds
	, THandler<FListBulkUserInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	TArray<FString> CopyUserIds = UserIds;
	if (CopyUserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserIds cannot be empty!"));
		return nullptr;
	}
	if (CopyUserIds.Num() > 100)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("The requested UserIds information list exceed max size 100. UserIds index 0-99 will be sent, and the rest will be truncated."));
		CopyUserIds.SetNum(100, true);
	}

	const FListBulkUserInfoRequest UserList{ CopyUserIds };

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/bulk/basic")
		, *ServerSettingsRef.IamServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(UserList, Content);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}
	
} // Namespace GameServerApi
} // Namespace AccelByte
