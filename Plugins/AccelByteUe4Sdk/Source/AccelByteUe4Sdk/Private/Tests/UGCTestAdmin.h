// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteUGCModels.h"

void UGCCreateType(const FString& Type, const TArray<FString>& SubType, const THandler<FAccelByteModelsUGCTypeResponse>& OnSuccess, const FErrorHandler& OnError);

void UGCCreateTags(const FString& Tag, const THandler<FAccelByteModelsUGCTagResponse>& OnSuccess, const FErrorHandler& OnError);

void UGCDeleteType(const FString& TypeId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

void UGCDeleteTags(const FString& TagId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
