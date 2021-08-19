// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

void AdminGrantSubscriptionFree(const FString& UserId, const FFreeSubscriptionRequest& BodyRequest, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
