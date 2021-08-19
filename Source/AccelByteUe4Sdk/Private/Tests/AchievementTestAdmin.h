// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

void AdminCreateAchievement(const FAchievementRequest& AchievementRequest, const THandler<FAchievementResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteAchievement(const FString& AchievementCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
