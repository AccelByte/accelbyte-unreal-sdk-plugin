// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteSessionHistoryApi.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/Base64.h"

namespace AccelByte
{
namespace Api
{

SessionHistory::SessionHistory(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

SessionHistory::~SessionHistory(){}

// This function is deprecated and will be removed on AGS 2025.4. 
FAccelByteTaskWPtr SessionHistory::QueryGameSessionHistory(THandler<FAccelByteModelsGameSessionHistoriesResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteGeneralSortBy const& SortBy
	, int32 Offset
	, int32 Limit)
{
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("SessionHistory::QueryGameSessionHistory is deprecated and will be removed on AGS 2025.4"));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/gamesessions")
		, *SettingsRef.SessionHistoryServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("order"), FAccelByteUtilities::ConvertAccelByteGeneralSortByToString(SortBy) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte