// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "MatchmakingTestAdmin.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "TestUtilities.h"

void AdminCreateMatchmakingChannel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable)
{
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 2;
	AllianceRule.max_number = 2;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 1;

	AdminCreateMatchmakingChannel(channel, AllianceRule, OnSuccess, OnError, joinable);
}

void AdminCreateMatchmakingChannel(const FString& channel, FAllianceRule AllianceRule, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable)
{
	AdminCreateMatchmakingChannel(channel, AllianceRule, TArray<FMatchingRule>(), OnSuccess, OnError, joinable);
}

void AdminCreateMatchmakingChannel(const FString& channel, FAllianceRule AllianceRule, TArray<FMatchingRule> MatchingRules, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable, TArray<FSubGameMode> SubGameModes)
{
	FMatchmakingCreateRequest RequestBody;
	RequestBody.description = channel;
	RequestBody.find_match_timeout_seconds = 60;
	RequestBody.game_mode = channel;
	RequestBody.rule_set.alliance = AllianceRule;
	RequestBody.rule_set.matching_rule = MatchingRules;
	for(FSubGameMode gamemode : SubGameModes)
	{
		RequestBody.rule_set.sub_game_modes.Add(gamemode.Name, gamemode);
	}
	if(SubGameModes.Num() > 0)
	{
		RequestBody.use_sub_gamemode = true;
	}
	RequestBody.joinable = joinable;

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(RequestBody, Content);
	UE_LOG(LogAccelByteTest, Log, TEXT("JSON Content: %s"), *Content);
	
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/matchmaking/namespaces/%s/channels"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminDeleteMatchmakingChannel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/matchmaking/namespaces/%s/channels/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *channel);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
