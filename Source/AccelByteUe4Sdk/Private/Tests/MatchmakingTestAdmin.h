// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"
#include "MatchmakingTestAdmin.generated.h"

USTRUCT(BlueprintType)
struct FAllianceRule
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceRule")
	int max_number{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceRule")
	int min_number{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceRule")
	int player_max_number{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceRule")
	int player_min_number{};
};

USTRUCT(BlueprintType)
struct FAllianceFlexingRule
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceFlexingRule")
	int duration{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceFlexingRule")
	int max_number{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceFlexingRule")
	int min_number{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceFlexingRule")
	int player_max_number{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceFlexingRule")
	int player_min_number{};
};

USTRUCT(BlueprintType)
struct FFlexingRule
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FFlexingRule")
	FString attribute{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FFlexingRule")
	FString criteria{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FFlexingRule")
	int duration{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FFlexingRule")
	int reference{};
};

USTRUCT(BlueprintType)
struct FMatchingRule
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FMatchingRule")
	FString attribute{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FMatchingRule")
	FString criteria{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FMatchingRule")
	int reference{};
};



USTRUCT(BlueprintType)
struct FSubGameMode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate | SubGameMode")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate | SubGameMode")
	FAllianceRule Alliance{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate | SubGameMode")
	TArray<FAllianceFlexingRule> Alliance_flexing_rule{};
};

USTRUCT(BlueprintType)
struct FMatchmakingRuleSet
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
	FAllianceRule alliance{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
	TArray<FFlexingRule> flexing_rule{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
	TArray<FMatchingRule> matching_rule{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
	TMap<FString, FSubGameMode> sub_game_modes{};
};

USTRUCT(BlueprintType)
struct FMatchmakingCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	FString description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	int find_match_timeout_seconds{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	FString game_mode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	FMatchmakingRuleSet rule_set{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	bool joinable {false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	bool use_sub_gamemode {false};
};

void AdminCreateMatchmakingChannel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false);
void AdminCreateMatchmakingChannel(const FString& channel, FAllianceRule AllianceRule, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false);
void AdminCreateMatchmakingChannel(const FString& channel, FAllianceRule AllianceRule, TArray<FMatchingRule> MatchingRules, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false, TArray<FSubGameMode> SubGameModes = {});
void AdminDeleteMatchmakingChannel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
