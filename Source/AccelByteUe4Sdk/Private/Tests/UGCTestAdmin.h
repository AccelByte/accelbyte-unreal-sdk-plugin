// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteUGCModels.h"

void AdminCreateUGCType(const FString& Type, const TArray<FString>& SubType, const THandler<FAccelByteModelsUGCTypeResponse>& OnSuccess, const FErrorHandler& OnError);

void AdminCreateUGCTags(const FString& Tag, const THandler<FAccelByteModelsUGCTagResponse>& OnSuccess, const FErrorHandler& OnError);

void AdminDeleteUGCType(const FString& TypeId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

void AdminDeleteUGCTag(const FString& TagId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
