// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerAchievementApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerAchievement::ServerAchievement(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

ServerAchievement::~ServerAchievement()
{}

FAccelByteTaskWPtr ServerAchievement::UnlockAchievement(FString const& UserId
	, FString const& AchievementCode
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Achievement Code is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/achievements/%s/unlock")
		, *ServerSettingsRef.AchievementServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *AchievementCode);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerAchievement::BulkCreatePSNEvent(FAccelByteModelsAchievementBulkCreatePSNEventRequest const& Request
	, THandler<FAccelByteModelsAchievementBulkCreatePSNEventResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Request.Data.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request: Request data array is empty."));
		return nullptr;
	}

	bool bHasEmptyEvents = false;
	for (const FAccelByteModelsAchievementPSNEventCreate& CreateEvent : Request.Data)
	{
		if (CreateEvent.Events.Num() <= 0)
		{
			bHasEmptyEvents = true;
			break;
		}
	}

	if (bHasEmptyEvents)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request: Events array in one of the data structures is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/platforms/psn/bulk")
		, *ServerSettingsRef.AchievementServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
