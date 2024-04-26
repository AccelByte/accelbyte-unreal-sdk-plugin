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

void ServerAchievement::UnlockAchievement(const FString& UserId
	, const FString& AchievementCode
	, const FVoidHandler OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Achievement Code is empty."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/achievements/%s/unlock")
		, *ServerSettingsRef.AchievementServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *AchievementCode);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerAchievement::BulkCreatePSNEvent(const FAccelByteModelsAchievementBulkCreatePSNEventRequest& Request
	, const THandler<FAccelByteModelsAchievementBulkCreatePSNEventResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Request.Data.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request: Request data array is empty."));
		return;
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
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/platforms/psn/bulk")
		, *ServerSettingsRef.AchievementServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
