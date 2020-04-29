// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteLeaderboardApi.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

	Leaderboard::Leaderboard(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting)
	{}

	Leaderboard::~Leaderboard()
	{}

	void Leaderboard::GetRankings(const FString& LeaderboardCode, const EAccelByteLeaderboardTimeFrame& TimeFrame, uint32 Offset, uint32 Limit, const THandler<FAccelByteModelsLeaderboardRankingResult>& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

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

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/leaderboards/%s/%s"), *Settings.LeaderboardServerUrl, *Settings.Namespace, *LeaderboardCode, *TimeFrameString);
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
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		
		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void Leaderboard::GetUserRanking(const FString& UserId, const FString& LeaderboardCode, const THandler<FAccelByteModelsUserRankingData>& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/leaderboards/%s/users/%s"), *Settings.LeaderboardServerUrl, *Settings.Namespace, *LeaderboardCode, *UserId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		
		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

} // Namespace Api
} // Namespace AccelByte
