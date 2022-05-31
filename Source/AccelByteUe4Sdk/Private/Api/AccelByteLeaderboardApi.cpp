// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteLeaderboardApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Leaderboard::Leaderboard(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

	Leaderboard::~Leaderboard()
	{}

	void Leaderboard::GetRankings(FString const& LeaderboardCode, EAccelByteLeaderboardTimeFrame const& TimeFrame, uint32 Offset, uint32 Limit, THandler<FAccelByteModelsLeaderboardRankingResult> const& OnSuccess, FErrorHandler const& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString TimeFrameString = "";
		switch (TimeFrame)
		{
		case EAccelByteLeaderboardTimeFrame::ALL_TIME:
			TimeFrameString = "alltime";
			break;
		case EAccelByteLeaderboardTimeFrame::CURRENT_SEASON:
			TimeFrameString = "season";
			break;
		case EAccelByteLeaderboardTimeFrame::CURRENT_MONTH:
			TimeFrameString = "month";
			break;
		case EAccelByteLeaderboardTimeFrame::CURRENT_WEEK:
			TimeFrameString = "week";
			break;
		case EAccelByteLeaderboardTimeFrame::TODAY:
			TimeFrameString = "today";
			break;
		default:
			break;
		}
			FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/leaderboards/%s/%s"), *SettingsRef.LeaderboardServerUrl, *SettingsRef.Namespace, *LeaderboardCode, *TimeFrameString);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		
		if (Offset > 0 || Limit > 0)
		{
			Url.Append("?");
			if (Offset > 0)
			{
				Url.Append(FString::Printf(TEXT("offset=%d&"), Offset));
			}
			if (Limit > 0)
			{
				Url.Append(FString::Printf(TEXT("limit=%d"), Limit));
			}
		}

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void Leaderboard::GetUserRanking(FString const& UserId, FString const& LeaderboardCode, THandler<FAccelByteModelsUserRankingData> const& OnSuccess, FErrorHandler const& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/leaderboards/%s/users/%s"), *SettingsRef.LeaderboardServerUrl, *SettingsRef.Namespace, *LeaderboardCode, *UserId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

} // Namespace Api
} // Namespace AccelByte
