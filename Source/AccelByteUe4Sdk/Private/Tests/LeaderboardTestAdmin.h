// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

void AdminCreateLeaderboard(const FLeaderboardConfigRequest& request, const THandler<FLeaderboardConfigRequest>& OnSuccess, const FErrorHandler& OnError);
void AdminGetLeaderboardAll(const FString& leaderboardCode, const THandler<FLeaderboardConfigResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteLeaderboard(const FString& leaderboardCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
