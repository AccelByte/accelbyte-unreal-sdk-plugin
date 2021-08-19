// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

void AdminCreateMatchmakingChannel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false);
void AdminCreateMatchmakingChannel(const FString& channel, FAllianceRule AllianceRule, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false);
void AdminCreateMatchmakingChannel(const FString& channel, FAllianceRule AllianceRule, TArray<FMatchingRule> MatchingRules, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false);
void AdminDeleteMatchmakingChannel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
