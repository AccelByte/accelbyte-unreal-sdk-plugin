// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

void AdminGetStatisticByStatCode(FString statCode, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateStatistic(FStatCreateRequest body, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteStatistic(const FString& statCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteStatisticItem(const FString& userId, const FString& statCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
